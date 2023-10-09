// Copyright Antioch. All rights reserved.
#pragma once

#include <antioch/transit_base/station.h>

#include "station_identifier.h"

namespace sfmtc {
namespace muni {

class MuniStation : public antioch::transit_base::Station {
 public:
  MuniStation(StationIdentifier id);
  MuniStation(const Station& station);

  std::string pretty_name() const;
};

}  // namespace muni
}  // namespace sfmtc
