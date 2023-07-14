// Copyright Antioch. All rights reserved.

#pragma once

#include <nlohmann/json.hpp>

#include "transit_agency.h"

namespace antioch {
namespace transit_base {

class Station {
 public:
  Station(TransitAgency agency, int id) : agency_(agency), id_(id) {}
  Station() : agency_(TransitAgency::INVALID), id_(0) {}

  virtual ~Station() = default;
  bool operator==(const Station& other) const {
    return agency_ == other.agency_ && id_ == other.id_;
  }
  TransitAgency agency() const { return agency_; }
  int id() const { return id_; }

 protected:
  TransitAgency agency_;
  int id_;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Station, agency_, id_);
};

}  // namespace transit_base
}  // namespace antioch
