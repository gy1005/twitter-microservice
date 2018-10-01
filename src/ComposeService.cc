//
// Created by yugan on 9/30/18.
//

#include <thrift/protocol/TBinaryProtocol.h>

#include "../include/Utils.h"
#include "ComposeService.h"

#define CLIENT_POOL_SIZE 32

using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

template<class TThriftClient>
int testConnect(const string &addr, int port) noexcept(false) {
  stdcxx::shared_ptr<TTransport> socket(new TSocket(addr, port));
  stdcxx::shared_ptr<TTransport> transport;

  transport = stdcxx::shared_ptr<TTransport>(new TBufferedTransport(socket));
  stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  TThriftClient client(protocol);
  while (true) {
    try {
      transport->open();
      transport->close();
      break;
    } catch (TException &tx) {
      cout << "TestConnect ERROR: " << tx.what() << endl;
      this_thread::sleep_for(chrono::seconds(1));
    }
  }
}

int main(int argc, char *argv[]) {
  json config_json = load_config_file("../config/service_config.json");

  string compose_addr = config_json["ComposeService"]["addr"];
  int compose_port = config_json["ComposeService"]["port"];

  string tweet_addr = config_json["TweetService"]["addr"];
  int tweet_port = config_json["TweetService"]["port"];
  string user_addr = config_json["UserService"]["addr"];
  int user_port = config_json["UserService"]["port"];
  string file_addr = config_json["FileService"]["addr"];
  int file_port = config_json["FileService"]["port"];

  testConnect<TweetServiceClient>(tweet_addr, tweet_port);
  testConnect<UserServiceClient>(user_addr, user_port);
  testConnect<FileServiceClient>(file_addr, file_port);

  auto tweet_client_pool = make_shared<ClientPool<ThriftCLient<
      TweetServiceClient>>>(CLIENT_POOL_SIZE, tweet_addr, tweet_port);
  auto user_client_pool = make_shared<ClientPool<ThriftCLient<
      UserServiceClient>>>(CLIENT_POOL_SIZE, user_addr, user_port);
  auto file_client_pool = make_shared<ClientPool<ThriftCLient<
      FileServiceClient>>>(CLIENT_POOL_SIZE, file_addr, file_port);

  TThreadedServer server(
      stdcxx::make_shared<ComposeServiceProcessor>(
          stdcxx::make_shared<ComposeServiceHandler>(tweet_client_pool,
                                                     user_client_pool,
                                                     file_client_pool)),
      stdcxx::make_shared<TServerSocket>(compose_addr, compose_port),
      stdcxx::make_shared<TBufferedTransportFactory>(),
      stdcxx::make_shared<TBinaryProtocolFactory>()
  );

  cout << "Starting the server..." << endl;
  server.serve();
  cout << "Done." << endl;
}


