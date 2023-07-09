// Copyright Antioch. All rights reserved.

#include <fstream>

#include "antioch/base/config/config.h"
#include "antioch/base/constants.h"
#include <nlohmann/json.hpp>

namespace antioch::base {

using json = nlohmann::json;
using antioch::base::Config;

void Config::validateOrThrow(const Config* config) {
  if (config->user_mode == UserMode::INVALID) {
    throw InvalidConfigException("Usermode invalid");
  }
  if (config->user_mode == UserMode::HOME_STOP && config->stations.size() == 0) {
    throw InvalidConfigException("No stations set");
  }
  // TODO: validate each stop
}

}  // namespace antioch::base
