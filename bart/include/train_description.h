// Copyright Antioch. All rights reserved.
#pragma once

#include "bart_line.h"
#include "bart_station.h"

namespace antioch {
namespace bart {

struct TrainDescription {
  StationIdentifier terminus;
  BartLine line;

  std::string to_string() const {
    return BartLine_Name(line) + ": " + StationIdentifier_pretty_name(terminus);
  }
};

}  // namespace bart
}  // namespace antioch
