// Copyright Antioch. All rights reserved.

#include "sfmtc/muni/muni_station.h"

namespace sfmtc::muni {

MuniStation::MuniStation(StationIdentifier id)
    : Station(antioch::transit_base::TransitAgency::SF_MUNI, id) {}

MuniStation::MuniStation(const Station& station) : MuniStation((StationIdentifier)station.id()) {
  if (station.agency() != antioch::transit_base::TransitAgency::SF_MUNI) {
    throw antioch::transit_base::StationInvariantViolation("Station is not a Muni station");
  }
}

std::string MuniStation::pretty_name() const {
  return StationIdentifier_pretty_name((StationIdentifier)id());
}

}  // namespace sfmtc::muni
