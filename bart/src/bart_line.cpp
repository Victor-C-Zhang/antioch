// Copyright Antioch. All rights reserved.

#include "bart_line.h"

namespace antioch::bart {

const std::string BartLine_Name(const BartLine& line) {
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

}  // namespace antioch::bart
