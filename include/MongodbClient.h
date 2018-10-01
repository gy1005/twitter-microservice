//
// Created by yugan on 9/30/18.
//

#ifndef TWITTER_MICROSERVICE_MONGODBCLIENT_H
#define TWITTER_MICROSERVICE_MONGODBCLIENT_H

#include <memory>
#include <iostream>
#include <string>
#include <assert.h>

#include "mongoc.h"

using namespace std;

class MongodbClient {
  MongodbClient(const string &, int, int);

  MongodbClient(const MongodbClient &);

  ~MongodbClient();

  const shared_ptr<mongoc_client_t> &getClient() const;

  int getId() const;

private:
  shared_ptr<mongoc_client_t> client_;
  int id_{};
};

MongodbClient::MongodbClient(const string &addr, int port, int id) :
    id_(id) {
  string config_str = "mongodb://" + addr + ":" + to_string(port);
  client_ = make_shared<mongoc_client_t>(mongoc_client_new(config_str.c_str()));
  assert(client_.get());
}

MongodbClient::MongodbClient(const MongodbClient &other) = default;

MongodbClient::~MongodbClient() {
  mongoc_client_destroy(client_.get());
}

const shared_ptr<mongoc_client_t> &MongodbClient::getClient() const {
  return client_;
}


int MongodbClient::getId() const {
  return id_;
}


#endif //TWITTER_MICROSERVICE_MONGODBCLIENT_H
