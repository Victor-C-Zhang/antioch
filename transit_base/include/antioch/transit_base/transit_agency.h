// Copyright Antioch. All rights reserved.

#pragma once

#include <nlohmann/json.hpp>

namespace antioch {
namespace transit_base {

enum class TransitAgency {
  INVALID,
  BART,
  SF_MUNI,
};
NLOHMANN_JSON_SERIALIZE_ENUM(TransitAgency, {
                                                {TransitAgency::INVALID, nullptr},
                                                {TransitAgency::BART, "bart"},
                                                {TransitAgency::SF_MUNI, "sf_muni"},
                                            });

}  // namespace transit_base
}  // namespace antioch
