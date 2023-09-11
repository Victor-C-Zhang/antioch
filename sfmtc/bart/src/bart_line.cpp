// Copyright Antioch. All rights reserved.

#include "sfmtc/bart/bart_line.h"

namespace sfmtc::bart {

const std::string BartLine_Name(const BartLine line) {
  switch (line) {
    case INVALID_LINE:
      return "INVALID_LINE";
    case BridgeA:
      return "BridgeA";
    case BridgeB:
      return "BridgeB";
    case YellowN:
      return "Yellow-N";
    case YellowS:
      return "Yellow-S";
    case OrangeN:
      return "Orange-N";
    case OrangeS:
      return "Orange-S";
    case GreenN:
      return "Green-N";
    case GreenS:
      return "Green-S";
    case RedN:
      return "Red-N";
    case RedS:
      return "Red-S";
    case BlueN:
      return "Blue-N";
    case BlueS:
      return "Blue-S";
    case BeigeS:
      return "Beige-S";
    case BeigeN:
      return "Beige-N";
  }
  return "BartLine_INVALID";
}

const std::string BartLine_pretty_name(const BartLine id) {
  switch (id) {
    case INVALID_LINE:
      return "Invalid line";
    case BridgeA:
      return "Bus Bridge";
    case BridgeB:
      return "Bus Bridge";
    case YellowN:
      return "Millbrae/SFIA to Antioch";
    case YellowS:
      return "Antioch to SFIA/Millbrae";
    case OrangeN:
      return "Berryessa/North San Jose to Richmond";
    case OrangeS:
      return "Richmond to Berryessa/North San Jose";
    case GreenN:
      return "Daly City to Berryessa/North San Jose";
    case GreenS:
      return "Berryessa/North San Jose to Daly City";
    case RedN:
      return "Millbrae/Daly City to Richmond";
    case RedS:
      return "Richmond to Daly City/Millbrae";
    case BlueN:
      return "Daly City to Dublin/Pleasanton";
    case BlueS:
      return "Dublin/Pleasanton to Daly City";
    case BeigeS:
      return "Coliseum to Oakland Airport";
    case BeigeN:
      return "Oakland Airport to Coliseum";
  }
  return "Invalid line";
}

}  // namespace sfmtc::bart
