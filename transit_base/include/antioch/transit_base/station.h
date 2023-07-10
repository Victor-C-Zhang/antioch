// Copyright Antioch. All rights reserved.

#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace antioch {
namespace transit_base {

struct Station {
  std::string name;
  std::string pretty_name;
  std::string fetch_url;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Station, name, pretty_name, fetch_url);

}  // namespace transit_base
}  // namespace antioch
