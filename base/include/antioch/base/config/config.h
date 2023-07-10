// Copyright Antioch. All rights reserved.

#pragma once

#include <antioch/transit_base/station.h>

#include <nlohmann/json.hpp>
#include <vector>

#include "antioch/base/types.h"

namespace antioch {
namespace base {

class Config {
 public:
  static void validate_or_throw(const Config* config);

  UserMode user_mode;
  std::vector<antioch::transit_base::Station> stations;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Config, user_mode, stations);

class InvalidConfigException : public std::runtime_error {
 public:
  InvalidConfigException(const std::string& what) : std::runtime_error(what) {}
};

}  // namespace base
}  // namespace antioch
