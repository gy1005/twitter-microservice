//
// Created by yugan on 9/30/18.
//

#include <thrift/protocol/TBinaryProtocol.h>

#include "../include/Utils.h"
#include "TweetService.h"

using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

int main(int argc, char *argv[]) {
  json config_json = load_config_file("../config/service_config.json");

  string tweet_addr = config_json["TweetService"]["addr"];
  int tweet_port = config_json["TweetService"]["port"];
  string tweet_memcached_addr = config_json["TweetMemcached"]["addr"];
  int tweet_memcached_port = config_json["TweetMemcached"]["port"];
  string tweet_mongodb_addr = config_json["TweetMongoDB"]["addr"];
  int tweet_mongodb_port = config_json["TweetMongoDB"]["port"];

  TThreadedServer server(
      stdcxx::make_shared<TweetServiceProcessor>(
          stdcxx::make_shared<TweetServiceHandler>(tweet_memcached_addr,
                                                   tweet_mongodb_addr,
                                                   tweet_memcached_port,
                                                   tweet_mongodb_port)),
      stdcxx::make_shared<TServerSocket>(tweet_addr, tweet_port),
      stdcxx::make_shared<TBufferedTransportFactory>(),
      stdcxx::make_shared<TBinaryProtocolFactory>()
  );

  cout << "Starting the server..." << endl;
  server.serve();
  cout << "Done." << endl;
}