// Copyright Antioch. All rights reserved.

#include "sfmtc/station_arrivals.h"

#include <sfmtc/bart/bart_line.h>
#include <sfmtc/muni/muni_line.h>

namespace sfmtc {

StationArrivals::StationArrivals(antioch::transit_base::Station station,
                                 std::vector<TrainArrival>&& arrivals)
    : station_(station), arrivals_(std::move(arrivals)) {}

std::string StationArrivals::bart_to_string() const {
  auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::stringstream res;
  for (auto& arrival : arrivals_) {
    auto time_mins = (arrival.second - now) / 60;
    res << sfmtc::bart::BartLine_pretty_name((sfmtc::bart::BartLine)arrival.first);
    res << ": " << time_mins << " mins\n";
  }
  return res.str();
}

std::string StationArrivals::muni_to_string() const {
  auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::stringstream res;
  for (auto& arrival : arrivals_) {
    auto time_mins = (arrival.second - now) / 60;
    res << sfmtc::muni::MuniLine_pretty_name((sfmtc::muni::MuniLine)arrival.first);
    res << ": " << time_mins << " mins\n";
  }
  return res.str();
}

}  // namespace sfmtc
