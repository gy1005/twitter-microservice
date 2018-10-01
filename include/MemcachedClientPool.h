//
// Created by yugan on 9/30/18.
//

#ifndef TWITTER_MICROSERVICE_MEMCACHEDCLIENTPOOL_H
#define TWITTER_MICROSERVICE_MEMCACHEDCLIENTPOOL_H

#include <utility>
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "libmemcached/memcached.hpp"

using namespace std;

class MemcachedClientPool {
public:
  MemcachedClientPool(const string &, int);

  MemcachedClientPool(const MemcachedClientPool &);

  ~MemcachedClientPool();

  // Get a client from the connection pool.
  shared_ptr<memcached_st> getClient();

  // Return a client to the free list.
  void returnClient(int client_id);

private:
  string config_;
  int pool_size_;
  vector<shared_ptr<memcached_st>> clients_;
  queue<int> free_list_;
  mutex mtx_;
  condition_variable cv_;

  shared_ptr<memcached_st> createClientAndUse_();
};

MemcachedClientPool::MemcachedClientPool(
    const string &config, int pool_size) :
    pool_size_(pool_size), config_(config), mtx_(), cv_() {
  for (int i = 0; i < pool_size_; ++i) {
    auto client_ptr = make_shared<memcached_st>(config_, config_.length());
    mtx_.lock();
    clients_.emplace_back(client_ptr);
    free_list_.push(i);
    mtx_.unlock();
  }
}

MemcachedClientPool::MemcachedClientPool(const MemcachedClientPool &other) :
    pool_size_(other.pool_size_),
    config_(other.config_),
    clients_(other.clients_),
    free_list_(other.free_list_),
    mtx_(),
    cv_() {}

MemcachedClientPool::~MemcachedClientPool() {
  assert(free_list_.size() == pool_size_);
  assert(clients_.size() == pool_size_);

  while (!clients_.empty()) {
    auto client_ptr = clients_.back();
    client_ptr.reset();
  }
  clients_.clear();
  swap(free_list_, queue<int>());
}


shared_ptr<memcached_st> ThriftClientPool<TClient>::getClient() {
  int client_id;
  shared_ptr<ThriftClientTuple < TClient>>
  return_ptr;

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


void ThriftClientPool<TClient>::returnClient(int client_id) {
  unique_lock<mutex> cv_lock(mtx_);
  {
    free_list_.push(client_id);
  }
  cv_lock.unlock();
  cv_.notify_one();
}


shared_ptr<ThriftClientTuple < TClient>>

ThriftClientPool<TClient>::
createClientAndUse_() {
  shared_ptr<ThriftClientTuple < TClient>>
  client_ptr;
  mtx_.lock();
  client_ptr = make_shared<ThriftClientTuple < TClient>>
  (addr_, port_, pool_size_);
  clients_.emplace_back(client_ptr);
  pool_size_++;

  mtx_.unlock();
  return client_ptr;
}

#endif //TWITTER_MICROSERVICE_MEMCACHEDCLIENTPOOL_H
