//
// Created by yugan on 9/30/18.
//

#ifndef TWITTER_MICROSERVICE_UTILS_H
#define TWITTER_MICROSERVICE_UTILS_H

#include <fstream>
#include <iostream>
#include <cassert>
#include <map>
#include <mutex>
#include <chrono>
#include <nlohmann/json.hpp>
#include "../gen-cpp/twitter_types.h"


using json = nlohmann::json;
using namespace std;
using namespace twitter;
using namespace chrono;

json load_config_file(const string &file_name) {
  ifstream json_file;
  json config;
  json_file.open(file_name);
  assert(json_file.is_open());
  json_file >> config;
  json_file.close();
  return config;
}

void add_timestamp(const string &service_name, const string &func_name,
    json &header, mutex *mtx) {
  long now_us = duration_cast<microseconds>(
      system_clock::now().time_since_epoch()).count();
  if (mtx) {
    mtx->lock();
    header[service_name][func_name] = now_us;
    mtx->unlock();
  } else {
    header[service_name][func_name] = now_us;
  }
}

#endif //TWITTER_MICROSERVICE_UTILS_H
