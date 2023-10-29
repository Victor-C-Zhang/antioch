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
  int32_t line_id = -1;
  bool direction = false;
  for (auto& arrival : arrivals_) {
    auto time_mins = (arrival.second - now) / 60;
    // collate arrival times by line
    if (line_id != arrival.first.first || direction != arrival.first.second) {
      if (line_id != -1) {
        res << " mins\n";
      }
      res << sfmtc::bart::BartLine_pretty_name((sfmtc::bart::BartLine)arrival.first.first) << ": ";
      line_id = arrival.first.first;
      direction = arrival.first.second;
      res << time_mins;
    } else {
      res << ", " << time_mins;
    }
  }
  res << " mins";
  return res.str();
}

std::string StationArrivals::muni_to_string() const {
  auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::stringstream res;
  int32_t line_id = -1;
  bool direction = false;
  for (auto& arrival : arrivals_) {
    auto time_mins = (arrival.second - now) / 60;
    // collate arrival times by line
    if (line_id != arrival.first.first || direction != arrival.first.second) {
      if (line_id != -1) {
        res << " mins\n";
      }
      res << sfmtc::muni::MuniLine_pretty_name((sfmtc::muni::MuniLine)arrival.first.first)
          << (arrival.first.second ? " Inbound" : " Outbound") << ": ";
      line_id = arrival.first.first;
      direction = arrival.first.second;
      res << time_mins;
    } else {
      res << ", " << time_mins;
    }
  }
  res << " mins";
  return res.str();
}

}  // namespace sfmtc
