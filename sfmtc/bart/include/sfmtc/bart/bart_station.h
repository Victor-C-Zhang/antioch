// Copyright Antioch. All rights reserved.
#pragma once

#include <antioch/transit_base/station.h>

#include "station_identifier.h"

namespace sfmtc {
namespace bart {

class BartStation : public antioch::transit_base::Station {
 public:
  BartStation(StationIdentifier id);
  BartStation(const Station& station);

  std::string pretty_name() const;
};

}  // namespace bart
}  // namespace sfmtc
