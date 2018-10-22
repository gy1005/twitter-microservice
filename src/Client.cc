#include <thread>
#include <iostream>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/stdcxx.h>
#include <list>
#include <numeric>
#include <getopt.h>
#include <memory>
#include <random>

#include "../gen-cpp/ComposeService.h"
#include "../gen-cpp/twitter_types.h"

#include "../include/Utils.h"


#define N_RECORDS 1000

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace twitter;


std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dist(0, N_RECORDS - 1);

long get_timestamp (
    const vector<Timestamp> &timestamps,
    const string &service_key,
    const string &func_key) {
  for (const auto& t : timestamps) {
    if (t.service_name == service_key && t.function_name == func_key)
      return t.timestamp;
  }
  return -1;
}

void client_worker(
    const int tid,
    const string &addr,
    const int port,
    int qps,
    int duration,
    list<map<string, long>> &reqs_latency,
    int &n_reqs_sent

    ) {
  stdcxx::shared_ptr<TTransport> socket(new TSocket(addr, port));
  stdcxx::shared_ptr<TTransport> transport;
  transport = stdcxx::shared_ptr<TTransport>(new TBufferedTransport(socket));
  stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  ComposeServiceClient client(protocol);
  long req_inteval_us = 1000000 / qps;
  int duration_us = duration * 1000000;
  auto thread_start_time = chrono::high_resolution_clock::now();
  auto next_send_time = thread_start_time;
  try {
    transport->open();
    while (chrono::high_resolution_clock::now() < thread_start_time
        + chrono::microseconds(duration_us)) {
      while (chrono::high_resolution_clock::now() < next_send_time) {
        int64_t sleep_time_us = (next_send_time
            - chrono::high_resolution_clock::now()).count() / 1000;
        sleep_time_us = max(sleep_time_us, (long)0);
        if (sleep_time_us)
          this_thread::sleep_for(chrono::microseconds(sleep_time_us));
      }

      Tweet _return_tweet;
      int id = dist(gen);
      string user_id = "user_" + to_string(id);
      string tweet_id = "tweet_" + to_string(id);
      vector<Timestamp> timestamps;
      map<string, long> latency;

      auto start_time = duration_cast<microseconds>(
          system_clock::now().time_since_epoch()).count();
      client.getTweet(_return_tweet, user_id, tweet_id, timestamps);
      auto end_time = duration_cast<microseconds>(
          system_clock::now().time_since_epoch()).count();

      latency["total"] = end_time - start_time;

      latency["client_send:compose_start"] =
         get_timestamp(_return_tweet.timestamps, "Compose", "compose_start") -
         start_time;
      latency["compose_start:compose_send_tweet"] =
         get_timestamp(_return_tweet.timestamps, "Compose", "getTweet_start") -
         get_timestamp(_return_tweet.timestamps, "Compose", "compose_start");
      latency["compose_start:compose_send_user"] =
         get_timestamp(_return_tweet.timestamps, "Compose", "getUser_start") -
         get_timestamp(_return_tweet.timestamps, "Compose", "compose_start");
      latency["compose_send_tweet:compose_recv_tweet"] =
         get_timestamp(_return_tweet.timestamps, "Compose", "getTweet_end") -
         get_timestamp(_return_tweet.timestamps, "Compose", "getTweet_start");
      latency["compose_start:compose_send_tweet/user"] =
          max(get_timestamp(_return_tweet.timestamps, "Compose", "getTweet_start"),
              get_timestamp(_return_tweet.timestamps, "Compose", "getUser_start")) -
          get_timestamp(_return_tweet.timestamps, "Compose", "compose_start");
      latency["compose_send_user:compose_recv_user"] =
         get_timestamp(_return_tweet.timestamps, "Compose", "getUser_end") -
         get_timestamp(_return_tweet.timestamps, "Compose", "getUser_start");
      latency["compose_recv_tweet:compose_send_file"] =
         get_timestamp(_return_tweet.timestamps, "Compose", "getFile_start") -
         get_timestamp(_return_tweet.timestamps, "Compose", "getTweet_end");
      latency["compose_send_file:compose_recv_file"] =
         get_timestamp(_return_tweet.timestamps, "Compose", "getFile_end") -
         get_timestamp(_return_tweet.timestamps, "Compose", "getFile_start");
      latency["compose_recv_file:compose_end"] =
         get_timestamp(_return_tweet.timestamps, "Compose", "compose_end") -
         get_timestamp(_return_tweet.timestamps, "Compose", "getFile_end");
      latency["compose_start:compose_end"] =
         get_timestamp(_return_tweet.timestamps, "Compose", "compose_end") -
         get_timestamp(_return_tweet.timestamps, "Compose", "compose_start");

      latency["compose_send_tweet:tweet_start"] =
         get_timestamp(_return_tweet.timestamps, "Tweet", "getTweet_start") -
         get_timestamp(_return_tweet.timestamps, "Compose", "getTweet_start");
      latency["tweet_start:tweet_send_get"] =
         get_timestamp(_return_tweet.timestamps, "Tweet", "get_start") -
         get_timestamp(_return_tweet.timestamps, "Tweet", "getTweet_start");
      latency["tweet_send_get:tweet_recv_get"] =
         get_timestamp(_return_tweet.timestamps, "Tweet", "get_end") -
         get_timestamp(_return_tweet.timestamps, "Tweet", "get_start");
      latency["tweet_recv_get:tweet_end"] =
         get_timestamp(_return_tweet.timestamps, "Tweet", "getTweet_end") -
         get_timestamp(_return_tweet.timestamps, "Tweet", "get_end");
      latency["tweet_start:tweet_end"] =
         get_timestamp(_return_tweet.timestamps, "Tweet", "getTweet_end") -
         get_timestamp(_return_tweet.timestamps, "Tweet", "getTweet_start");

      latency["compose_send_user:user_start"] =
         get_timestamp(_return_tweet.timestamps, "User", "getUser_start") -
         get_timestamp(_return_tweet.timestamps, "Compose", "getUser_start");
      latency["user_start:user_send_get"] =
         get_timestamp(_return_tweet.timestamps, "User", "get_start") -
         get_timestamp(_return_tweet.timestamps, "User", "getUser_start");
      latency["user_send_get:user_recv_get"] =
         get_timestamp(_return_tweet.timestamps, "User", "get_end") -
         get_timestamp(_return_tweet.timestamps, "User", "get_start");
      latency["user_recv_get:user_end"] =
         get_timestamp(_return_tweet.timestamps, "User", "getUser_end") -
         get_timestamp(_return_tweet.timestamps, "User", "get_end");
      latency["user_start:user_end"] =
         get_timestamp(_return_tweet.timestamps, "User", "getUser_end") -
         get_timestamp(_return_tweet.timestamps, "User", "getUser_start");

      latency["compose_send_file:file_start"] =
         get_timestamp(_return_tweet.timestamps, "File", "getFile_start") -
         get_timestamp(_return_tweet.timestamps, "Compose", "getFile_start");
      latency["file_start:file_send_get"] =
         get_timestamp(_return_tweet.timestamps, "File", "get_start") -
         get_timestamp(_return_tweet.timestamps, "File", "getFile_start");
      latency["file_send_get:file_recv_get"] =
         get_timestamp(_return_tweet.timestamps, "File", "get_end") -
         get_timestamp(_return_tweet.timestamps, "File", "get_start");
      latency["file_recv_get:file_end"] =
         get_timestamp(_return_tweet.timestamps, "File", "getFile_end") -
         get_timestamp(_return_tweet.timestamps, "File", "get_end");
      latency["file_start:file_end"] =
         get_timestamp(_return_tweet.timestamps, "File", "getFile_end") -
         get_timestamp(_return_tweet.timestamps, "File", "getFile_start");



      reqs_latency.push_back(latency);
      n_reqs_sent++;
      next_send_time = next_send_time + chrono::microseconds(req_inteval_us);

    }

    transport->close();
  } catch (TException &tx) {
    cout << "ERROR: " << tx.what() << endl;
  }
}

double avg(list<map<string, long>> const& v, const string &key) {
  long sum = 0;
  for (auto i : v) {
    sum += i[key];
  }
  return 1.0 * sum / v.size();
}

double sum(vector<map<string, long>> const& v, const string &key) {
  long sum = 0;
  for (auto i : v) {
    sum += i[key];
  }
  return 1.0 * sum;
}

double sum(vector<int> const& v) {
  return 1.0 * accumulate(v.begin(), v.end(), 0LL);
}

int main(int argc, char *argv[]) {
  json config_json = load_config_file("../config/service_config.json");

  string compose_addr = config_json["ComposeService"]["addr"];
  int compose_port = config_json["ComposeService"]["port"];

  int qps = 1000;
  int n_threads = 10;
  int duration = 10;

  int flags, opt;
  flags = 0;
  while ((opt = getopt(argc, argv, "t:d:q:")) != -1) {
    switch (opt) {
      case 't':
        n_threads = atoi(optarg);
        break;
      case 'd':
        duration = atoi(optarg);
        break;
      case 'q':
        qps = atoi(optarg);
        break;
      default:  /* '?' */
        fprintf(stderr, "Usage: %s [-t number of threads] [-s] server type\n",
                argv[0]);
        exit(EXIT_FAILURE);

    }
  }

  std::shared_ptr<thread> t_ptr[n_threads];
  vector<int> n_reqs_sent;
  for (int i = 0; i < n_threads; i++) {
    n_reqs_sent.push_back(0);
  }
  int tid = 0;
  int32_t interval = 1000000 / qps;

  auto *reqs_latency = new list<map<string, long>> [n_threads];

  for (auto &i : t_ptr) {
    i = std::make_shared<thread>(
        client_worker,
        tid,
        compose_addr,
        compose_port,
        qps / n_threads,
        duration,
        ref(reqs_latency[tid]),
        ref(n_reqs_sent[tid]));
    tid++;
    this_thread::sleep_for(chrono::microseconds(interval));
  }

  for (auto &i : t_ptr) {
    i->join();
  }

  list<map<string, long>> full_latency;
  for (int i = 0; i < n_threads; i++)
    full_latency.merge(reqs_latency[i]);



  for (const auto&[key, val] : full_latency.front()) {
    cout<<key<<"(us)"<<"\t"<<avg(full_latency, key)<<endl;
  }
  cout<<"qps: "<<sum(n_reqs_sent) / duration<<endl;


}



