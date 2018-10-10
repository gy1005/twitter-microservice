

//
// Created by yugan on 9/30/18.
//

#ifndef TWITTER_MICROSERVICE_TWEETSERVICE_H
#define TWITTER_MICROSERVICE_TWEETSERVICE_H

#include <iostream>
#include <string>
#include <utility>

#include <mongoc.h>
#include <bson.h>
#include <libmemcached/memcached.h>
#include <nlohmann/json.hpp>

#include "../gen-cpp/TweetService.h"
#include "../gen-cpp/twitter_types.h"
#include "../include/Utils.h"

using namespace std;
using namespace twitter;
using json = nlohmann::json;

class TweetServiceHandler : public TweetServiceIf {
public:
  TweetServiceHandler(string, string, int, int);

  ~TweetServiceHandler() override = default;

  void getTweet_(Tweet_ &, const string &, const vector<Timestamp> &) override;

private:
  string memcached_addr_;
  string mongodb_addr_;
  int memcached_port_;
  int mongodb_port_;
};

TweetServiceHandler::TweetServiceHandler(
    string memcached_addr,
    string mongodb_addr,
    int memcached_port,
    int mongodb_port
) : memcached_addr_(std::move(memcached_addr)),
    mongodb_addr_(std::move(mongodb_addr)),
    memcached_port_(memcached_port),
    mongodb_port_(mongodb_port) {
}

void TweetServiceHandler::getTweet_(Tweet_ &_return, const string &tweet_id,
                                    const vector<Timestamp> &timestamps) {
  vector<Timestamp> timestamps_return_ = timestamps;
  append_timestamp("Tweet", "getTweet_start", timestamps_return_, nullptr);
  
  string memcached_config_str = "--SERVER=" + memcached_addr_ + ":"
                                + to_string(memcached_port_);
  auto memcached_client = memcached(memcached_config_str.c_str(),
                                    memcached_config_str.length());
  memcached_behavior_set(memcached_client, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
  memcached_behavior_set(memcached_client, MEMCACHED_BEHAVIOR_TCP_NODELAY, 1);
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
  append_timestamp("Tweet", "get_start", timestamps_return_, nullptr);
  char *memcached_data = memcached_get(
      memcached_client,
      tweet_id.c_str(),
      tweet_id.length(),
      &memcached_data_size,
      &memcached_flags,
      &memcached_rc);
  append_timestamp("Tweet", "get_end", timestamps_return_, nullptr);

  if (memcached_data) {
    // If the user is in memcached, return the data.
    string memcached_str = memcached_data;
    json data_json = json::parse(memcached_str);
    _return.tweet_id = data_json["tweet_id"];
    _return.user_id = data_json["user_id"];
    _return.file_id = data_json["file_id"];
    _return.text = data_json["text"];
    assert(_return.tweet_id == tweet_id);
  } else {
    // If the user is not in the memcached, find it in mongodb.

    auto collection = mongoc_client_get_collection(mongodb_client,
                                                   "tweet", "tweet");
    assert(collection);
    bson_t *query = bson_new();
    BSON_APPEND_UTF8(query, "tweet_id", tweet_id.c_str());
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(
        collection, query, nullptr, nullptr);
    const bson_t *doc;

    append_timestamp("Tweet", "find_start", timestamps_return_, nullptr);
    bool if_found = mongoc_cursor_next(cursor, &doc);
    append_timestamp("Tweet", "find_end", timestamps_return_, nullptr);
    
    if (if_found) {
      // If found in mongodb, set memcached and return
      json data_json = json::parse(bson_as_json(doc, nullptr));
      _return.tweet_id = data_json["tweet_id"];
      _return.user_id = data_json["user_id"];
      _return.file_id = data_json["file_id"];
      _return.text = data_json["text"];
      assert(_return.tweet_id == tweet_id);

      string doc_str = data_json.dump();
      
      append_timestamp("Tweet", "set_start", timestamps_return_, nullptr);
      memcached_rc = memcached_set(
          memcached_client,
          tweet_id.c_str(),
          tweet_id.length(),
          doc_str.c_str(),
          doc_str.length(),
          (time_t) 0,
          (uint32_t) 0
      );
      append_timestamp("Tweet", "set_end", timestamps_return_, nullptr);
      
      if (memcached_rc != MEMCACHED_SUCCESS)
        cerr << "getUser " << memcached_strerror(memcached_client, memcached_rc)
             << endl;
    }
    mongoc_collection_destroy(collection);
  }
  mongoc_client_destroy(mongodb_client);
  memcached_free(memcached_client);
  append_timestamp("Tweet", "getTweet_end", timestamps_return_, nullptr);
  _return.timestamps = timestamps_return_;
}

#endif //TWITTER_MICROSERVICE_TWEETSERVICE_H
