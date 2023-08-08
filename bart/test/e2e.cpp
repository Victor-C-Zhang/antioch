// Copyright Antioch. All rights reserved.

#include "bart_converter.h"

namespace antioch::bart {
TEST(E2E, convert) {
  BartConverter bc{};
  bc.startTracking({BART, StationIdentifier::CIVC});
  auto res = bc.convert();
  for (const auto& station : res) {
    std::cout << station.to_string() << std::endl;
  }
}

}  // namespace antioch::bart
