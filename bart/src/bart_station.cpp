// Copyright Antioch. All rights reserved.

#include "bart_station.h"

namespace antioch::bart {

BartStation::BartStation(StationIdentifier id) {
  agency = antioch::transit_base::TransitAgency::BART;
  station_id = id;
}

bool BartStation::operator==(const BartStation& other) {
  return station_id == other.station_id;
}

}  // namespace antioch::bart
