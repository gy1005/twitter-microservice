

//
// Created by yugan on 9/30/18.
//

#ifndef TWITTER_MICROSERVICE_USERSERVICE_H
#define TWITTER_MICROSERVICE_USERSERVICE_H

#include <iostream>
#include <utility>

#include <mongoc.h>
#include <bson.h>
#include <libmemcached/memcached.h>
#include <nlohmann/json.hpp>

#include "../gen-cpp/UserService.h"
#include "../gen-cpp/twitter_types.h"

#include "../include/ThriftClient.h"
#include "../include/ClientPool.h"

using namespace twitter;
using json = nlohmann::json;

class UserServiceHandler : public UserServiceIf {
public:
  UserServiceHandler(string, string, int, int);

//  UserServiceHandler(shared_ptr<ClientPool<MemcachedClient>> &,
//                     shared_ptr<ClientPool<MongodbClient>> &);
  ~UserServiceHandler() override = default;

  void getUser_(User_ &, const string &) override;

private:
//  shared_ptr<ClientPool<MemcachedClient>> memcached_client_pool_;
//  shared_ptr<ClientPool<MongodbClient>> mongodb_client_pool_;
  string memcached_addr_;
  string mongodb_addr_;
  int memcached_port_;
  int mongodb_port_;
};

UserServiceHandler::UserServiceHandler(
    string memcached_addr,
    string mongodb_addr,
    int memcached_port,
    int mongodb_port
) : memcached_addr_(std::move(memcached_addr)),
    mongodb_addr_(std::move(mongodb_addr)),
    memcached_port_(memcached_port),
    mongodb_port_(mongodb_port) {
}

//UserServiceHandler::UserServiceHandler(
//    shared_ptr<ClientPool<MemcachedClient>> &memcached_client_pool,
//    shared_ptr<ClientPool<MongodbClient>> &mongodb_client_pool) :
//    memcached_client_pool_(memcached_client_pool),
//    mongodb_client_pool_(mongodb_client_pool) {}


void UserServiceHandler::getUser_(User_ &_return, const string &user_id) {
  string memcached_config_str = "--SERVER=" + memcached_addr_ + ":"
                                + to_string(memcached_port_);
  auto memcached_client = memcached(memcached_config_str.c_str(),
                                    memcached_config_str.length());

  memcached_behavior_set(memcached_client, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
  memcached_behavior_set(memcached_client, MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
//  memcached_behavior_set(mmc, MEMCACHED_BEHAVIOR_NOREPLY, 1);
  memcached_behavior_set(memcached_client, MEMCACHED_BEHAVIOR_TCP_KEEPALIVE, 1);
  memcached_behavior_set(memcached_client, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL,
                         1);

  string mongodb_config_str = "mongodb://" + mongodb_addr_ + ":"
                              + to_string(mongodb_port_);
  auto mongodb_client = mongoc_client_new(mongodb_config_str.c_str());

  memcached_return_t memcached_rc;
  size_t memcached_data_size;
  uint32_t memcached_flags;

  // Find the user in the memcached
  string memcached_data(
      memcached_get(
          memcached_client,
          user_id.c_str(),
          user_id.length(),
          &memcached_data_size,
          &memcached_flags,
          &memcached_rc)
  );

  if (!memcached_data.empty()) {
    // If the user is in memcached, return the data.

    json data_json = json::parse(memcached_data);
    _return.username = data_json["username"];
    _return.user_id = data_json["user_id"];
    _return.homepage = data_json["homepage"];
    assert(_return.user_id == user_id);
  } else {
    // If the user is not in the memcached, find it in mongodb.

    auto collection = mongoc_client_get_collection(mongodb_client,
                                                   "user", "user");
    assert(collection);
    bson_t *query = bson_new();
    BSON_APPEND_UTF8(query, "user_id", user_id.c_str());
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(
        collection, query, nullptr, nullptr);
    const bson_t *doc;
    if (mongoc_cursor_next(cursor, &doc)) {
      // If found in mongodb, set memcached and return
      json data_json = json::parse(bson_as_json(doc, nullptr));
      _return.username = data_json["username"];
      _return.user_id = data_json["user_id"];
      _return.homepage = data_json["homepage"];
      assert(_return.user_id == user_id);

      string doc_str = data_json.dump();

      memcached_rc = memcached_set(
          memcached_client,
          user_id.c_str(),
          user_id.length(),
          doc_str.c_str(),
          doc_str.length(),
          (time_t) 0,
          (uint32_t) 0
      );
      if (memcached_rc != MEMCACHED_SUCCESS)
        cerr << "getUser " << memcached_strerror(memcached_client, memcached_rc)
             << endl;
    }
    mongoc_collection_destroy(collection);
  }
  mongoc_client_destroy(mongodb_client);
  memcached_free(memcached_client);
}


#endif //TWITTER_MICROSERVICE_USERSERVICE_H
