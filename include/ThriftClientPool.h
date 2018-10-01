/*
 * Thrirt client pool with TThreadedServer
 */

#ifndef TWITTER_MICROSERVICE_THRIFTCLIENTPOOL_H
#define TWITTER_MICROSERVICE_THRIFTCLIENTPOOL_H

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/stdcxx.h>

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>


using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

#define MAX_N_CLIENTS 256
#define MAX_N_RETRY 10

template<class TClient>
class ThriftClientTuple {
public:
  ThriftClientTuple(const string &, int, int);

  ThriftClientTuple(const ThriftClientTuple<TClient> &);

  ~ThriftClientTuple();

  const shared_ptr<TClient> &getClient() const;

  int getId() const;

private:
  stdcxx::shared_ptr<TTransport> socket_;
  stdcxx::shared_ptr<TProtocol> protocol_;
  stdcxx::shared_ptr<TTransport> transport_;
  stdcxx::shared_ptr<TClient> client_;
  int id_;

};

template<class TClient>
ThriftClientTuple<TClient>::ThriftClientTuple(
    const string &addr,
    int port,
    int id
) : id_(id) {

  socket_ = stdcxx::shared_ptr<TTransport>(new TSocket(addr, port));
  transport_ = stdcxx::shared_ptr<TTransport>(
      new TBufferedTransport(socket_));
  protocol_ = stdcxx::shared_ptr<TProtocol>(new TBinaryProtocol(transport_));
  client_ = stdcxx::shared_ptr<TClient>(new TClient(protocol_));

  for (int i = 0; i < MAX_N_RETRY; ++i) {
    try {
      transport_->open();
      break;
    } catch (TException &tx) {
      cout << "ERROR: " << tx.what() << endl;
      cout << "Trying Reconnect" << endl;
      this_thread::sleep_for(chrono::milliseconds(100));
    }
  }
}

template<class TClient>
ThriftClientTuple<TClient>::ThriftClientTuple(
    const ThriftClientTuple<TClient> &other) :
    socket_(other.socket_),
    protocol_(other.protocol_),
    client_(other.client_),
    transport_(other.transport_),
    id_(other.id_) {}

template<class TClient>
ThriftClientTuple<TClient>::~ThriftClientTuple() {
  try {
    transport_->close();
  } catch (TException &tx) {
    // TODO: Resolve closing error
    cout << "ERROR: " << tx.what() << endl;
  }
  client_.reset();
  protocol_.reset();
  transport_.reset();
  socket_.reset();
}

template<class TClient>
const shared_ptr<TClient> &ThriftClientTuple<TClient>::getClient() const {
  return client_;
}

template<class TClient>
int ThriftClientTuple<TClient>::getId() const {
  return id_;
}

template<class TClient>
class ThriftClientPool {
public:
  ThriftClientPool(int pool_size, const string &addr, int port);

  ThriftClientPool(ThriftClientPool<TClient> &other);

  ~ThriftClientPool();

  // Get a client from the connection pool.
  shared_ptr<ThriftClientTuple<TClient>> getClient();

  // Return a client to the free list.
  void returnClient(int client_id);

private:
  int pool_size_{};
  string addr_;
  int port_;
  vector<shared_ptr<ThriftClientTuple<TClient>>> clients_;
  queue<int> free_list_;
  mutex mtx_;
  condition_variable cv_;

  // Create a new client if there is current no client in the free list.
  shared_ptr<ThriftClientTuple<TClient>> createClientAndUse_();

};

template<class TClient>
ThriftClientPool<TClient>::ThriftClientPool(
    int pool_size,
    const string &addr,
    int port): mtx_(), pool_size_(pool_size), addr_(addr), port_(port), cv_() {
  for (int i = 0; i < pool_size_; ++i) {
    auto client_ptr = make_shared<ThriftClientTuple<TClient>>(addr, port, i);

    // Ensure the thread safety when manipulating the queues.
    mtx_.lock();
    clients_.emplace_back(client_ptr);
    free_list_.push(i);
    mtx_.unlock();
  }
}


template<class TClient>
ThriftClientPool<TClient>::ThriftClientPool(
    ThriftClientPool<TClient> &other) :
    pool_size_(other.pool_size_),
    addr_(other.addr_),
    port_(other.port_),
    clients_(other.clients_),
    free_list_(other.free_list_),
    mtx_(),
    cv_() {}


template<class TClient>
ThriftClientPool<TClient>::~ThriftClientPool() {
  assert(free_list_.size() == pool_size_);
  assert(clients_.size() == pool_size_);

  while (!clients_.empty()) {
    auto client_ptr = clients_.back();
    client_ptr.reset();
  }

  clients_.clear();
  swap(free_list_, queue<int>());

}

template<class TClient>
shared_ptr<ThriftClientTuple<TClient>> ThriftClientPool<TClient>::getClient() {
  int client_id;
  shared_ptr<ThriftClientTuple<TClient>> return_ptr;

  unique_lock<mutex> cv_lock(mtx_);
  {
    while (free_list_.size() == 0) {
      if (pool_size_ < MAX_N_CLIENTS) {
        cv_lock.unlock();
        return createClientAndUse_();
      } else {
        cv_.wait(cv_lock, [this] { return free_list_.size() > 0; });
      }
    }
    client_id = free_list_.front();
    free_list_.pop();
    return_ptr = clients_[client_id];
    cv_lock.unlock();
    return return_ptr;

  }
}

template<class TClient>
void ThriftClientPool<TClient>::returnClient(int client_id) {
  unique_lock<mutex> cv_lock(mtx_);
  {
    free_list_.push(client_id);
  }
  cv_lock.unlock();
  cv_.notify_one();
}

template<class TClient>
shared_ptr<ThriftClientTuple<TClient>> ThriftClientPool<TClient>::
createClientAndUse_() {
  shared_ptr<ThriftClientTuple<TClient>> client_ptr;
  mtx_.lock();
  client_ptr = make_shared<ThriftClientTuple<TClient>>(addr_, port_,
                                                       pool_size_);
  clients_.emplace_back(client_ptr);
  pool_size_++;

  mtx_.unlock();
  return client_ptr;
}

#endif //TWITTER_MICROSERVICE_THRIFTCLIENTPOOL_H
