// Copyright Antioch. All rights reserved.

#include "bart_converter.h"

#include <typeinfo>
#include <gtfs-realtime.pb.h>

namespace antioch::bart {

using antioch::transit_base::StationTrackingException;

void BartConverter::startTracking(const antioch::transit_base::Station& station) {
  try {
    const BartStation& cast_station = dynamic_cast<const BartStation&>(station);
    {
      std::unique_lock<std::mutex> l(stations_mtx);
      stations.push_back({cast_station});
    }
  } catch (const std::bad_cast& e) {
    throw StationTrackingException(e.what());
    // throw StationTrackingException("Requested station is not a BART station");
  }
}

void BartConverter::stopTracking(const antioch::transit_base::Station& station) {
  try {
    const BartStation& cast_station = dynamic_cast<const BartStation&>(station);
    {
      std::unique_lock<std::mutex> l(stations_mtx);
      for (unsigned i = 0; i < stations.size(); ++i) {
        if (stations[i] == cast_station) {
          stations.erase(stations.begin() + i);
        }
      }
    }
  } catch (const std::bad_cast& e) {
    throw StationTrackingException(e.what());
    // throw StationTrackingException("Requested station is not a BART station");
  }
}

std::string BartConverter::convert(const std::vector<std::byte>& data) {
  transit_realtime::FeedMessage fm;
  fm.ParseFromArray(data.data(), data.size());
  return "e";
}

}  // namespace antioch::bart
