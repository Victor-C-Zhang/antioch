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

std::string BartLine_Name(const BartLine& line) {
  switch (line) {
    case BartLine::YELLOW:
      return "YELLOW";
    case BartLine::BLUE:
      return "BLUE";
    case BartLine::ORANGE:
      return "ORANGE";
    case BartLine::GREEN:
      return "GREEN";
    case BartLine::RED:
      return "RED";
    case BartLine::YELLOW_PM:
      return "YELLOW_PM";
  }
  return "BartLine_INVALID";
}

}  // namespace bart
}  // namespace antioch
