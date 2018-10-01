//
// Created by yugan on 9/30/18.
//

#ifndef TWITTER_MICROSERVICE_MEMCACHEDCLIENT_H
#define TWITTER_MICROSERVICE_MEMCACHEDCLIENT_H

#include <memory>
#include <iostream>
#include <string>
#include <assert.h>

#include "libmemcached/memcached.hpp"

using namespace std;

class MemcachedClient {
public:
  MemcachedClient(const string &, int, int);

  MemcachedClient(const MemcachedClient &);

  ~MemcachedClient();

  const shared_ptr<memcached_st> &getClient() const;

  int getId() const;

private:
  shared_ptr<memcached_st> client_;
  int id_{};
};

MemcachedClient::MemcachedClient(const string &addr, int port, int id) :
    id_(id) {

  string config_str = "--SERVER=" + addr + ":" + to_string(port);
  client_ = make_shared<memcached_st>(config_str, config_str.length());
  assert(client_.get());
  memcached_behavior_set(client_.get(), MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
  memcached_behavior_set(client_.get(), MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
  memcached_behavior_set(client_.get(), MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);
  memcached_behavior_set(client_.get(), MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, 1);
}

MemcachedClient::MemcachedClient(const MemcachedClient &other) = default;

MemcachedClient::~MemcachedClient() {
  memcached_free(client_.get());
}

const shared_ptr<memcached_st> &MemcachedClient::getClient() const {
  return client_;
}


int MemcachedClient::getId() const {
  return id_;
}

#endif //TWITTER_MICROSERVICE_MEMCACHEDCLIENT_H
