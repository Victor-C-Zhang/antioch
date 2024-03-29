// Copyright Antioch. All rights reserved.

#include "bart/station_arrivals.h"

#include "bart/bart_line.h"

namespace bart {

StationArrivals::StationArrivals(antioch::transit_base::Station station,
                                 std::vector<TrainArrival>&& arrivals)
    : station_(station), arrivals_(std::move(arrivals)) {}

std::string StationArrivals::bart_to_string() const {
  auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::stringstream res;
  for (auto& arrival : arrivals_) {
    auto time_mins = (arrival.second - now) / 60;
    res << bart::BartLine_pretty_name((bart::BartLine)arrival.first);
    res << ": " << time_mins << " mins\n";
  }
  return res.str();
}

}  // namespace bart
