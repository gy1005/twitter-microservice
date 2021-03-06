#ifndef TWITTER_MICROSERVICE_COMPOSESERVICE_H
#define TWITTER_MICROSERVICE_COMPOSESERVICE_H

#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "../gen-cpp/FileService.h"
#include "../gen-cpp/UserService.h"
#include "../gen-cpp/TweetService.h"
#include "../gen-cpp/ComposeService.h"
#include "../gen-cpp/twitter_types.h"

#include "../include/ClientPool.h"
#include "../include/ThriftClient.h"
#include "../include/Utils.h"

using namespace std;
using namespace twitter;

class ComposeServiceHandler : public ComposeServiceIf {
public:
  ComposeServiceHandler(
      shared_ptr<ClientPool<ThriftCLient<TweetServiceClient>>> &,
      shared_ptr<ClientPool<ThriftCLient<UserServiceClient>>> &,
      shared_ptr<ClientPool<ThriftCLient<FileServiceClient>>> &
  );

  ~ComposeServiceHandler() override = default;

  void getTweet(Tweet &, const string &, const string &,
      const string &) override;

private:
  shared_ptr<ClientPool<ThriftCLient<TweetServiceClient>>> tweet_client_pool_;
  shared_ptr<ClientPool<ThriftCLient<UserServiceClient>>> user_client_pool_;
  shared_ptr<ClientPool<ThriftCLient<FileServiceClient>>> file_client_pool_;

  void getTweetHelper_(Tweet_ &, const string &, json &, mutex *);

  void getUserHelper_(User_ &, const string &, json &, mutex *);

  void getFileHelper_(File_ &, const string &, json &, mutex *);

};

ComposeServiceHandler::ComposeServiceHandler(
    shared_ptr<ClientPool<ThriftCLient<TweetServiceClient>>> &tweet_client_pool,
    shared_ptr<ClientPool<ThriftCLient<UserServiceClient>>> &user_client_pool,
    shared_ptr<ClientPool<ThriftCLient<FileServiceClient>>> &file_client_pool
) :
    tweet_client_pool_(tweet_client_pool),
    user_client_pool_(user_client_pool),
    file_client_pool_(file_client_pool) {}

void ComposeServiceHandler::getTweet(
    Tweet &_return,
    const string &user_id,
    const string &tweet_id,
    const string &header) {
  json _header_return_json;
  add_timestamp("Compose", "compose_start", _header_return_json, nullptr);

  Tweet_ _tweet;
  User_ _user;
  File_ _file;
  mutex mtx;

  thread tweet_thread(&ComposeServiceHandler::getTweetHelper_, this,
                      ref(_tweet), ref(tweet_id),
                      ref(_header_return_json), &mtx);
  thread user_thread(&ComposeServiceHandler::getUserHelper_, this,
                     ref(_user), ref(user_id),
                     ref(_header_return_json), &mtx);

  tweet_thread.join();
  assert(!_tweet.file_id.empty());

  thread file_thread(&ComposeServiceHandler::getFileHelper_, this,
                     ref(_file), ref(_tweet.file_id),
                     ref(_header_return_json), &mtx);

  user_thread.join();
  file_thread.join();

  assert(user_id == _user.user_id);
  assert(tweet_id == _tweet.tweet_id);
  assert(_tweet.file_id == _file.file_id);

  _return.tweet_id = _tweet.tweet_id;
  _return.text = _tweet.text;
  _return.file = _file;
  _return.user = _user;

  json _user_header_json = json::parse(_user.header);
  json _file_header_json = json::parse(_file.header);
  json _tweet_header_json = json::parse(_tweet.header);
  
  _header_return_json.update(_user_header_json);
  _header_return_json.update(_file_header_json);
  _header_return_json.update(_tweet_header_json);

  _return.file.header = "";
  _return.user.header = "";

  add_timestamp("Compose", "compose_end", _header_return_json, nullptr);
  _return.header = _header_return_json.dump(2);

}

void ComposeServiceHandler::getTweetHelper_(
    Tweet_ &_return,
    const string &tweet_id,
    json &header,
    mutex *mtx) {

  auto tweet_client = this->tweet_client_pool_->getClient();
  add_timestamp("Compose", "getTweet_start", header, mtx);
  tweet_client->getClient()->getTweet_(_return, tweet_id, "{}");
  add_timestamp("Compose", "getTweet_end", header, mtx);
  this->tweet_client_pool_->returnClient(tweet_client->getId());

}

void ComposeServiceHandler::getUserHelper_(
    User_ &_return, const string &user_id,
    json &header,
    mutex *mtx) {

  auto user_client = this->user_client_pool_->getClient();
  add_timestamp("Compose", "getUser_start", header, mtx);
  user_client->getClient()->getUser_(_return, user_id, "{}");
  add_timestamp("Compose", "getUser_end", header, mtx);
  this->user_client_pool_->returnClient(user_client->getId());

}

void ComposeServiceHandler::getFileHelper_(
    File_ &_return, const string &file_id,
    json &header,
    mutex *mtx) {

  auto file_client = this->file_client_pool_->getClient();
  add_timestamp("Compose", "getFile_start", header, mtx);
  file_client->getClient()->getFile_(_return, file_id, "{}");
  add_timestamp("Compose", "getFile_end", header, mtx);
  this->file_client_pool_->returnClient(file_client->getId());

}


#endif //TWITTER_MICROSERVICE_COMPOSESERVICE_H