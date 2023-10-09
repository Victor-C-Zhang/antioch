// Copyright Antioch. All rights reserved.
#pragma once

#include <antioch/transit_base/converter.h>

namespace sfmtc {

// {line ID, direction}, unix time
typedef std::pair<std::pair<int32_t, bool>, int64_t> TrainArrival;

class StationArrivals {
 public:
  StationArrivals(antioch::transit_base::Station station, std::vector<TrainArrival>&& arrivals);
  std::string bart_to_string() const;
  std::string muni_to_string() const;

  const antioch::transit_base::Station& station() const { return station_; }
  const std::vector<TrainArrival>& arrivals() const { return arrivals_; }

 private:
  antioch::transit_base::Station station_;
  std::vector<TrainArrival> arrivals_;
};

}  // namespace sfmtc
