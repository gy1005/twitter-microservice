//
// Created by yugan on 9/30/18.
//

#ifndef TWITTER_MICROSERVICE_CLIENTPOOL_H
#define TWITTER_MICROSERVICE_CLIENTPOOL_H

#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>

#define MAX_N_CLIENTS 256

using namespace std;

template<class TClient>
class ClientPool {
public:
  ClientPool(int pool_size, const string &addr, int port);

  ClientPool(ClientPool<TClient> &other);

  ~ClientPool();

  // Get a client from the connection pool.
  shared_ptr<TClient> getClient();

  // Return a client to the free list.
  void returnClient(int client_id);

//  void addClient(shared_ptr<TClient>);

private:
  string addr_;
  int port_{};
  int pool_size_{};
  vector<shared_ptr<TClient>> clients_;
  queue<int> free_list_;
  mutex mtx_;
  condition_variable cv_;

  // Create a new client if there is current no client in the free list.
  shared_ptr<TClient> createClientAndUse_();

};

template<class TClient>
ClientPool<TClient>::ClientPool(int pool_size, const string &addr, int port) :
    pool_size_(pool_size), addr_(addr), port_(port), mtx_(), cv_() {
  for (int i = 0; i < pool_size_; ++i) {
    auto client_ptr = make_shared<TClient>(addr, port, i);

    // Ensure the thread safety when manipulating the queues.
    mtx_.lock();
    clients_.emplace_back(client_ptr);
    free_list_.push(i);
    mtx_.unlock();
  }

}

template<class TClient>
ClientPool<TClient>::ClientPool(ClientPool<TClient> &other) :
    pool_size_(other.pool_size_),
    clients_(other.clients_),
    free_list_(other.free_list_),
    addr_(other.addr_),
    port_(other.port_),
    mtx_(),
    cv_() {}

template<class TClient>
ClientPool<TClient>::~ClientPool() {
  assert(free_list_.size() == pool_size_);
  assert(clients_.size() == pool_size_);

  while (!clients_.empty()) {
    auto client_ptr = clients_.back();
    client_ptr.reset();
  }

  clients_.clear();
  queue<int> empty;
  swap(free_list_, empty);
};

template<class TClient>
shared_ptr<TClient> ClientPool<TClient>::getClient() {
  int client_id;
  shared_ptr<TClient> return_ptr;
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
void ClientPool<TClient>::returnClient(int client_id) {
  unique_lock<mutex> cv_lock(mtx_);
  {
    free_list_.push(client_id);
  }
  cv_lock.unlock();
  cv_.notify_one();
}

template<class TClient>
shared_ptr<TClient> ClientPool<TClient>::
createClientAndUse_() {
  shared_ptr<TClient> client_ptr;
  mtx_.lock();
  client_ptr = make_shared<TClient>(addr_, port_, pool_size_);
  clients_.emplace_back(client_ptr);
  pool_size_++;

  mtx_.unlock();
  return client_ptr;
}


#endif //TWITTER_MICROSERVICE_CLIENTPOOL_H
