// Copyright Antioch. All rights reserved.

#include "sfmtc/muni/util.h"

#include <antioch/transit_base/station.h>

#include "sfmtc/muni/station_identifier.h"

namespace sfmtc {
namespace muni {

MuniLine line_of(const transit_realtime::TripUpdate& tu) {
  const auto& route = tu.trip().route_id();
  auto retval = MuniLine_Parse(route.substr(3));
  if (retval == MuniLine::INVALID) {
    std::string msg = "Unknown Muni route_id " + route;
    throw antioch::transit_base::StationInvariantViolation(msg);
  }
  return retval;
}

}  // namespace muni
}  // namespace sfmtc
