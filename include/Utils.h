//
// Created by yugan on 9/30/18.
//

#ifndef TWITTER_MICROSERVICE_UTILS_H
#define TWITTER_MICROSERVICE_UTILS_H

#include <fstream>
#include <cassert>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

json load_config_file(const string &file_name) {
  ifstream json_file;
  json config;
  json_file.open(file_name);
  assert(json_file.is_open());
  json_file >> config;
  json_file.close();
  return config;
}

#endif //TWITTER_MICROSERVICE_UTILS_H
