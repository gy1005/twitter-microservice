//
// Created by yugan on 9/30/18.
//

#include <thrift/protocol/TBinaryProtocol.h>

#include "../include/Utils.h"
#include "FileService.h"

using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

int main(int argc, char *argv[]) {
  json config_json = load_config_file("../config/service_config.json");

  string file_addr = config_json["FileService"]["addr"];
  int file_port = config_json["FileService"]["port"];
  string file_memcached_addr = config_json["FileMemcached"]["addr"];
  int file_memcached_port = config_json["FileMemcached"]["port"];
  string file_mongodb_addr = config_json["FileMongoDB"]["addr"];
  int file_mongodb_port = config_json["FileMongoDB"]["port"];

  TThreadedServer server(
      stdcxx::make_shared<FileServiceProcessor>(
          stdcxx::make_shared<FileServiceHandler>(file_memcached_addr,
                                                  file_mongodb_addr,
                                                  file_memcached_port,
                                                  file_mongodb_port)),
      stdcxx::make_shared<TServerSocket>(file_addr, file_port),
      stdcxx::make_shared<TBufferedTransportFactory>(),
      stdcxx::make_shared<TBinaryProtocolFactory>()
  );

  cout << "Starting the server..." << endl;
  server.serve();
  cout << "Done." << endl;
}