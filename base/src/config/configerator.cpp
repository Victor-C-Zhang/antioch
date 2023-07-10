// Copyright Antioch. All rights reserved.

#include <cstdlib>
#include <fstream>

#include "antioch/base/config/configerator.h"
#include "antioch/base/constants.h"
#include <nlohmann/json.hpp>

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
  config->stations.push_back({
    .name = "BART_Civic",
    .pretty_name = "Civic Center/UN Plaza",
    .fetch_url = "",
  });
  return std::unique_ptr<Config>(config);
}

}  // namespace antioch::base::Configerator
