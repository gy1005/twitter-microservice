//
// Created by yugan on 9/30/18.
//


#include <thrift/server/TThreadedServer.h>
#include <thrift/protocol/TBinaryProtocol.h>

#include "../include/Utils.h"
#include "UserService.h"
#include <thread>
#include <iostream>
#include <fstream>

using namespace::std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

int main(int argc, char *argv[]) {
  json config_json = load_config_file("../config/service_config.json");

  string user_addr = config_json["UserService"]["addr"];
  int user_port = config_json["UserService"]["port"];
  string user_memcached_addr = config_json["UserMemcached"]["addr"];
  int user_memcached_port = config_json["UserMemcached"]["port"];
  string user_mongodb_addr = config_json["UserMongoDB"]["addr"];
  int user_mongodb_port = config_json["UserMongoDB"]["port"];


  TThreadedServer server(
      stdcxx::make_shared<UserServiceProcessor>(
          stdcxx::make_shared<UserServiceHandler>(user_memcached_addr,
                                                  user_mongodb_addr,
                                                  user_memcached_port,
                                                  user_mongodb_port)),
      stdcxx::make_shared<TServerSocket>(user_addr, user_port),
      stdcxx::make_shared<TBufferedTransportFactory>(),
      stdcxx::make_shared<TBinaryProtocolFactory>()
  );


  cout << "Starting the server..." << endl;
  server.serve();
  cout << "Done." << endl;
}
