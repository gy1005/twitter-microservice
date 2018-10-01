//
// Created by yugan on 9/30/18.
//

#ifndef TWITTER_MICROSERVICE_COMPOSESERVICE_H
#define TWITTER_MICROSERVICE_COMPOSESERVICE_H

#include <iostream>
#include <string>
#include <thread>

#include "../gen-cpp/FileService.h"
#include "../gen-cpp/UserService.h"
#include "../gen-cpp/TweetService.h"
#include "../gen-cpp/ComposeService.h"
#include "../gen-cpp/twitter_types.h"

#include "../include/ClientPool.h"
#include "../include/ThriftClient.h"

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

  void getTweet(Tweet &, const string &, const string &) override;

private:
  shared_ptr<ClientPool<ThriftCLient<TweetServiceClient>>> tweet_client_pool_;
  shared_ptr<ClientPool<ThriftCLient<UserServiceClient>>> user_client_pool_;
  shared_ptr<ClientPool<ThriftCLient<FileServiceClient>>> file_client_pool_;

  void getTweetHelper_(Tweet_ &, const string &);

  void getUserHelper_(User_ &, const string &);

  void getFileHelper_(File_ &, const string &);

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
    Tweet &_return, const string &user_id, const string &tweet_id) {

  Tweet_ _tweet;
  User_ _user;
  File_ _file;

  thread tweet_thread(&ComposeServiceHandler::getTweetHelper_, this,
                      ref(_tweet), ref(tweet_id));
  thread user_thread(&ComposeServiceHandler::getUserHelper_, this,
                     ref(_user), ref(user_id));

  tweet_thread.join();
  assert(!_tweet.file_id.empty());

  thread file_thread(&ComposeServiceHandler::getFileHelper_, this,
                     ref(_file), ref(_tweet.file_id));

  user_thread.join();
  file_thread.join();

  assert(user_id == _user.user_id);
  assert(tweet_id == _tweet.tweet_id);
  assert(_tweet.file_id == _file.file_id);

  _return.tweet_id = _tweet.tweet_id;
  _return.text = _tweet.text;
  _return.file = _file;
  _return.user = _user;

}

void ComposeServiceHandler::getTweetHelper_(
    Tweet_ &_return, const string &tweet_id) {
  auto tweet_client = tweet_client_pool_->getClient();
  tweet_client->getClient()->getTweet_(_return, tweet_id);
  tweet_client_pool_->returnClient(tweet_client->getId());
}

void ComposeServiceHandler::getUserHelper_(
    User_ &_return, const string &user_id) {
  auto user_client = user_client_pool_->getClient();
  user_client->getClient()->getUser_(_return, user_id);
  user_client_pool_->returnClient(user_client->getId());
}

void ComposeServiceHandler::getFileHelper_(
    File_ &_return, const string &file_id) {
  auto file_client = file_client_pool_->getClient();
  file_client->getClient()->getFile_(_return, file_id);
  file_client_pool_->returnClient(file_client->getId());
}


#endif //TWITTER_MICROSERVICE_COMPOSESERVICE_H
