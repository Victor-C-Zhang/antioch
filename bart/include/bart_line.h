// Copyright Antioch. All rights reserved.
#pragma once

#include <string>

namespace antioch {
namespace bart {

enum class BartLine {
  YELLOW,
  BLUE,
  ORANGE,
  GREEN,
  RED,
  YELLOW_PM,
};

const std::string BartLine_Name(const BartLine& line);

}  // namespace bart
}  // namespace antioch
