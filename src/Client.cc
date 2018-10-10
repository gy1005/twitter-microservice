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


#define N_RECORDS 999

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace twitter;



std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dist(0, N_RECORDS - 1);

void worker(
    const int tid,
    const string &addr,
    const int port,
    int qps,
    int duration,
    list<int64_t> &reqs_latency,
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
    while (chrono::high_resolution_clock::now() < thread_start_time + chrono::microseconds(duration_us)){
      while (chrono::high_resolution_clock::now() < next_send_time) {
        int64_t sleep_time_us = (next_send_time - chrono::high_resolution_clock::now()).count() / 1000;
        sleep_time_us = max(sleep_time_us, (long)0);
        if (sleep_time_us)
          this_thread::sleep_for(chrono::microseconds(sleep_time_us));
      }
      auto start_time = chrono::high_resolution_clock::now();

      Tweet _return_tweet;
      int id = dist(gen);
      string user_id = "user_" + to_string(id);
      string tweet_id = "tweet_" + to_string(id);
      vector<Timestamp> timestamps;
      client.getTweet(_return_tweet, user_id, tweet_id, timestamps);

      auto end_time = chrono::high_resolution_clock::now();
      auto req_latency = (end_time - start_time).count() / 1000;
      reqs_latency.push_back(req_latency);
      n_reqs_sent++;
      next_send_time = next_send_time + chrono::microseconds(req_inteval_us);

    }

    transport->close();
  } catch (TException &tx) {
    cout << "ERROR: " << tx.what() << endl;
  }
}

void r(list<int> &out) {
  out.push_back(1);
}

double avg(list<int64_t> const& v) {
  return 1.0 * accumulate(v.begin(), v.end(), 0LL) / v.size();
}

double sum(vector<int> const& v) {
  return 1.0 * accumulate(v.begin(), v.end(), 0LL);
}

int main(int argc, char *argv[]) {
  json config_json = load_config_file("../config/service_config.json");

  string compose_addr = config_json["ComposeService"]["addr"];
  int compose_port = config_json["ComposeService"]["port"];

  int qps = 100000;
  int n_threads = 100;
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
  for (int i = 0; i< n_threads; i++) {
    n_reqs_sent.push_back(0);
  }
  int tid = 0;
  int32_t interval = 1000000 / qps;

  auto *reqs_latency = new list<int64_t> [n_threads];

  for (auto &i : t_ptr) {
    i = std::make_shared<thread>(
        worker,
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

  list<int64_t> full_latency;
  for (int i = 0; i < n_threads; i++)
    full_latency.merge(reqs_latency[i]);

  cout<<avg(full_latency)<<endl;
  cout<<sum(n_reqs_sent) / duration<<endl;


}



