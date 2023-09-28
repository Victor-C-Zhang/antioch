// Copyright Antioch. All rights reserved.

#include "sfmtc/bart/util.h"

#include <antioch/transit_base/station.h>

#include "sfmtc/bart/station_identifier.h"

namespace sfmtc {
namespace bart {

BartLine line_of(const transit_realtime::TripUpdate& tu) {
  const auto& route = tu.trip().route_id();
  auto retval = BartLine_Parse(route.substr(3));
  if (retval == BartLine::INVALID_LINE) {
    std::string msg = "Unknown BART route_id " + route;
    throw antioch::transit_base::StationInvariantViolation(msg);
  }
  return retval;
}

}  // namespace bart
}  // namespace sfmtc
