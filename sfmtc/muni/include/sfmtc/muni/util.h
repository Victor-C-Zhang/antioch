// Copyright Antioch. All rights reserved.
#pragma once

#include <antioch/transit_base/station.h>
#include <gtfs-realtime.pb.h>

#include "muni_line.h"

namespace sfmtc {
namespace muni {

MuniLine line_of(const transit_realtime::TripUpdate& tu);

}  // namespace muni
}  // namespace sfmtc
