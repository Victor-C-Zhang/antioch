// Copyright Antioch. All rights reserved.
#pragma once

#include <string>

namespace sfmtc {
namespace bart {

enum BartLine : int {
  INVALID_LINE = 0,
  BridgeA = 1,  // BridgeA
  BridgeB = 2,  // BridgeB
  YellowN = 3,  // Yellow-N
  YellowS = 4,  // Yellow-S
  OrangeN = 5,  // Orange-N
  OrangeS = 6,  // Orange-S
  GreenN = 7,   // Green-N
  GreenS = 8,   // Green-S
  RedN = 9,     // Red-N
  RedS = 10,    // Red-S
  BlueN = 11,   // Blue-N
  BlueS = 12,   // Blue-S
  BeigeS = 13,  // Beige-S
  BeigeN = 14,  // Beige-N
};

const std::string BartLine_Name(const BartLine& line);
const std::string BartLine_pretty_name(const BartLine id);

}  // namespace bart
}  // namespace sfmtc
