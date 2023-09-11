// Copyright Antioch. All rights reserved.
#pragma once

#include <antioch/transit_base/station.h>
#include <gtfs-realtime.pb.h>

#include "bart_line.h"

namespace sfmtc {
namespace bart {

BartLine line_of(const transit_realtime::TripUpdate& tu);

}  // namespace bart
}  // namespace sfmtc
