// Copyright Antioch. All rights reserved.

#include "antioch/base/config/configerator.h"

#include <cstdlib>
#include <fstream>
#include <nlohmann/json.hpp>

#include "antioch/base/constants.h"

namespace antioch::base::Configerator {

using json = nlohmann::json;
using antioch::base::Config;

std::unique_ptr<Config> read_or_exception() {
  const char* home_dir = getenv("HOME");
  const std::string path = std::string(home_dir) + CONFIGERATOR_FILE_PATH_FROM_HOME;
  std::ifstream in(path);
  json j;
  in >> j;
  in.close();
  if (j.type() != json::value_t::object) {
    throw ConfigeratorReadException("Wrong json type");
  }
  auto config = new Config();
  j.get_to(*config);

  Config::validate_or_throw(config);
  return std::unique_ptr<Config>(config);
}

void write_or_exception(const Config& config) {
  const char* home_dir = getenv("HOME");
  const std::string path = std::string(home_dir) + CONFIGERATOR_FILE_PATH_FROM_HOME;
  std::fstream out;
  out.open(path, std::ios::out);
  json j = config;
  out << j;
  out.close();
}

std::unique_ptr<Config> default_config() {
  auto config = new Config();
  config->user_mode = UserMode::HOME_STOP;
  antioch::transit_base::Station civic(antioch::transit_base::TransitAgency::BART, 31);
  config->stations.push_back(civic);
  return std::unique_ptr<Config>(config);
}

}  // namespace antioch::base::Configerator
