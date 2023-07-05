// Copyright Antioch. All rights reserved.

#pragma once

#include <antioch/transit_base/station.h>

#include <vector>

namespace antioch {
namespace base {

enum class UserMode {
  HOME_STOP,
  JOURNEY,
};

struct Config {
  UserMode user_mode;
  std::vector<antioch::transit_base::Station> stations;
};

}  // namespace base
}  // namespace antioch
