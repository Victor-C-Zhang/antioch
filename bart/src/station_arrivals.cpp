// Copyright Antioch. All rights reserved.
#pragma once

#include "station_arrivals.h"

namespace antioch::bart {

StationArrivals::StationArrivals(antioch::transit_base::Station station,
                                 std::vector<TrainArrival>&& arrivals)
    : station_(station), arrivals_(std::move(arrivals)) {}

std::string StationArrivals::to_string() const { return "TODO"; }

}  // namespace antioch::bart
