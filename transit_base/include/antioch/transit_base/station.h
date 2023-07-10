// Copyright Antioch. All rights reserved.

#pragma once

#include <nlohmann/json.hpp>

#include "transit_agency.h"

namespace antioch {
namespace transit_base {

class Station {
 public:
  virtual ~Station() = default;

  TransitAgency agency;
  int station_id;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Station, agency, station_id);

}  // namespace transit_base
}  // namespace antioch
