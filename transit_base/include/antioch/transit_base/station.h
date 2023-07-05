// Copyright Antioch. All rights reserved.

#pragma once

#include <string>

namespace antioch {
namespace transit_base {

struct Station {
  std::string name;
  std::string pretty_name;
  std::string fetch_url;
};

}  // namespace transit_base
}  // namespace antioch
