// Copyright Antioch. All rights reserved.

#pragma once

#include <antioch/transit_base/station.h>

#include <nlohmann/json.hpp>
#include <vector>

namespace antioch {
namespace base {

enum class UserMode {
  INVALID,
  HOME_STOP,
  JOURNEY,
};
NLOHMANN_JSON_SERIALIZE_ENUM(UserMode, {
                                           {UserMode::INVALID, nullptr},
                                           {UserMode::HOME_STOP, "home_stop"},
                                           {UserMode::JOURNEY, "journey"},
                                       });

}  // namespace base
}  // namespace antioch
