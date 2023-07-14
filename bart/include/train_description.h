// Copyright Antioch. All rights reserved.
#pragma once

#include "bart_line.h"
#include "bart_station.h"

namespace antioch {
namespace bart {

struct TrainDescription {
  StationIdentifier terminus;
  BartLine line;
};

}  // namespace bart
}  // namespace antioch
