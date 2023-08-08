// Copyright Antioch. All rights reserved.

#include "station_arrivals.h"

namespace antioch::bart {

StationArrivals::StationArrivals(antioch::transit_base::Station station,
                                 std::vector<TrainArrival>&& arrivals)
    : station_(station), arrivals_(std::move(arrivals)) {}

std::string StationArrivals::to_string() const {
  auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::stringstream res;
  for (auto& arrival : arrivals_) {
    res << arrival.first.to_string();
    res << ": " << arrival.second - now << " mins\n";
  }
  return res.str();
}

}  // namespace antioch::bart
