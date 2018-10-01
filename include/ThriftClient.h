//
// Created by yugan on 9/30/18.
//

#ifndef TWITTER_MICROSERVICE_THRIFTCLIENT_H
#define TWITTER_MICROSERVICE_THRIFTCLIENT_H

#include <string>
#include <thread>
#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/stdcxx.h>

#define MAX_N_RETRY 10

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

template<class TThriftClient>
class ThriftCLient {
public:
  ThriftCLient(const string &, int, int);

  ThriftCLient(const ThriftCLient<TThriftClient> &);

  ~ThriftCLient();

  const shared_ptr<TThriftClient> &getClient() const;

  int getId() const;

private:
  stdcxx::shared_ptr<TTransport> socket_;
  stdcxx::shared_ptr<TProtocol> protocol_;
  stdcxx::shared_ptr<TTransport> transport_;
  stdcxx::shared_ptr<TThriftClient> client_;
  int id_;
};

template<class TThriftClient>
ThriftCLient<TThriftClient>::ThriftCLient(
    const string &addr,
    int port,
    int id
) : id_(id) {

  socket_ = stdcxx::shared_ptr<TTransport>(new TSocket(addr, port));
  transport_ = stdcxx::shared_ptr<TTransport>(
      new TBufferedTransport(socket_));
  protocol_ = stdcxx::shared_ptr<TProtocol>(new TBinaryProtocol(transport_));
  client_ = stdcxx::shared_ptr<TThriftClient>(new TThriftClient(protocol_));

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

template<class TThriftClient>
ThriftCLient<TThriftClient>::ThriftCLient(
    const ThriftCLient<TThriftClient> &other) :
    socket_(other.socket_),
    protocol_(other.protocol_),
    client_(other.client_),
    transport_(other.transport_),
    id_(other.id_) {}

template<class TThriftClient>
ThriftCLient<TThriftClient>::~ThriftCLient() {
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

template<class TThriftClient>
const shared_ptr<TThriftClient> &ThriftCLient<TThriftClient>::
getClient() const {
  return client_;
}

template<class TThriftClient>
int ThriftCLient<TThriftClient>::getId() const {
  return id_;
}

#endif //TWITTER_MICROSERVICE_THRIFTCLIENT_H
