// Copyright Antioch. All rights reserved.

#include "sfmtc/bart/bart_station.h"

#include "sfmtc/bart/bart_converter.h"

namespace sfmtc::bart {

BartStation::BartStation(StationIdentifier id)
    : Station(antioch::transit_base::TransitAgency::BART, id) {}

BartStation::BartStation(const Station& station) : BartStation((StationIdentifier)station.id()) {
  if (station.agency() != antioch::transit_base::TransitAgency::BART) {
    throw InvariantViolation("Station is not a BART station");
  }
}

std::string BartStation::pretty_name() const {
  return StationIdentifier_pretty_name((StationIdentifier)id());
}

}  // namespace sfmtc::bart
