// Copyright Antioch. All rights reserved.

#include "sfmtc/muni/muni_line.h"

#include <unordered_map>

namespace sfmtc::muni {

const std::string MuniLine_pretty_name(const MuniLine id) {
  switch (id) {
    case INVALID:
      return "Invalid station";
    case _1:
      return "CALIFORNIA";
    case _12:
      return "FOLSOM-PACIFIC";
    case _14:
      return "MISSION";
    case _14R:
      return "MISSION RAPID";
    case _15:
      return "BAYVIEW HUNTERS POINT EXPRESS";
    case _18:
      return "46TH AVENUE";
    case _19:
      return "POLK";
    case _1X:
      return "CALIFORNIA EXPRESS";
    case _2:
      return "SUTTER";
    case _21:
      return "HAYES";
    case _22:
      return "FILLMORE";
    case _23:
      return "MONTEREY";
    case _24:
      return "DIVISADERO";
    case _25:
      return "TREASURE ISLAND";
    case _27:
      return "BRYANT";
    case _28:
      return "19TH AVENUE";
    case _29:
      return "SUNSET";
    case _30:
      return "STOCKTON";
    case _31:
      return "BALBOA";
    case _33:
      return "ASHBURY-18TH ST";
    case _35:
      return "EUREKA";
    case _36:
      return "TERESITA";
    case _37:
      return "CORBETT";
    case _38:
      return "GEARY";
    case _38R:
      return "GEARY RAPID";
    case _39:
      return "COIT";
    case _43:
      return "MASONIC";
    case _44:
      return "O'SHAUGHNESSY";
    case _45:
      return "UNION-STOCKTON";
    case _48:
      return "QUINTARA-24TH STREET";
    case _49:
      return "VAN NESS-MISSION";
    case _5:
      return "FULTON";
    case _52:
      return "EXCELSIOR";
    case _54:
      return "FELTON";
    case _55:
      return "DOGPATCH";
    case _56:
      return "RUTLAND";
    case _57:
      return "PARKMERCED";
    case _58:
      return "LAKE MERCED";
    case _5R:
      return "FULTON RAPID";
    case _6:
      return "HAIGHT-PARNASSUS";
    case _66:
      return "QUINTARA";
    case _67:
      return "BERNAL HEIGHTS";
    case _7:
      return "HAIGHT-NORIEGA";
    case _714:
      return "BART EARLY BIRD";
    case _8:
      return "BAYSHORE";
    case _8AX:
      return "BAYSHORE A EXPRESS";
    case _8BX:
      return "BAYSHORE B EXPRESS";
    case _9:
      return "SAN BRUNO";
    case _90:
      return "SAN BRUNO OWL";
    case _91:
      return "3RD-19TH AVE OWL";
    case _9R:
      return "SAN BRUNO RAPID";
    case CA:
      return "CALIFORNIA STREET CABLE CAR";
    case F:
      return "MARKET & WHARVES";
    case J:
      return "CHURCH";
    case K:
      return "INGLESIDE";
    case KBUS:
      return "K INGLESIDE BUS";
    case LBUS:
      return "TARAVAL BUS";
    case LOWL:
      return "OWL TARAVAL";
    case M:
      return "M OCEAN VIEW";
    case N:
      return "JUDAH";
    case NBUS:
      return "JUDAH BUS";
    case NOWL:
      return "OWL JUDAH";
    case PH:
      return "POWELL-HYDE CABLE CAR";
    case PM:
      return "POWELL-MASON CABLE CAR";
    case S:
      return "SHUTTLE";
    case T:
      return "THIRD";
    case TBUS:
      return "THIRD BUS";
    case _28R:
      return "19TH AVENUE RAPID";
  }
  return "Invalid station";
}

const std::string MuniLine_Name(const MuniLine id) {
  switch (id) {
    case INVALID:
      return "INVALID";
    case _1:
      return "1";
    case _12:
      return "12";
    case _14:
      return "14";
    case _14R:
      return "14R";
    case _15:
      return "15";
    case _18:
      return "18";
    case _19:
      return "19";
    case _1X:
      return "1X";
    case _2:
      return "2";
    case _21:
      return "21";
    case _22:
      return "22";
    case _23:
      return "23";
    case _24:
      return "24";
    case _25:
      return "25";
    case _27:
      return "27";
    case _28:
      return "28";
    case _29:
      return "29";
    case _30:
      return "30";
    case _31:
      return "31";
    case _33:
      return "33";
    case _35:
      return "35";
    case _36:
      return "36";
    case _37:
      return "37";
    case _38:
      return "38";
    case _38R:
      return "38R";
    case _39:
      return "39";
    case _43:
      return "43";
    case _44:
      return "44";
    case _45:
      return "45";
    case _48:
      return "48";
    case _49:
      return "49";
    case _5:
      return "5";
    case _52:
      return "52";
    case _54:
      return "54";
    case _55:
      return "55";
    case _56:
      return "56";
    case _57:
      return "57";
    case _58:
      return "58";
    case _5R:
      return "5R";
    case _6:
      return "6";
    case _66:
      return "66";
    case _67:
      return "67";
    case _7:
      return "7";
    case _714:
      return "714";
    case _8:
      return "8";
    case _8AX:
      return "8AX";
    case _8BX:
      return "8BX";
    case _9:
      return "9";
    case _90:
      return "90";
    case _91:
      return "91";
    case _9R:
      return "9R";
    case CA:
      return "CA";
    case F:
      return "F";
    case J:
      return "J";
    case K:
      return "K";
    case KBUS:
      return "KBUS";
    case LBUS:
      return "LBUS";
    case LOWL:
      return "LOWL";
    case M:
      return "M";
    case N:
      return "N";
    case NBUS:
      return "NBUS";
    case NOWL:
      return "NOWL";
    case PH:
      return "PH";
    case PM:
      return "PM";
    case S:
      return "S";
    case T:
      return "T";
    case TBUS:
      return "TBUS";
    case _28R:
      return "28R";
  }
  return "INVALID";
}

static const std::unordered_map<std::string, MuniLine> parse_map{
    {"1", _1},      {"12", _12},    {"14", _14},    {"14R", _14R},  {"15", _15},   {"18", _18},
    {"19", _19},    {"1X", _1X},    {"2", _2},      {"21", _21},    {"22", _22},   {"23", _23},
    {"24", _24},    {"25", _25},    {"27", _27},    {"28", _28},    {"29", _29},   {"30", _30},
    {"31", _31},    {"33", _33},    {"35", _35},    {"36", _36},    {"37", _37},   {"38", _38},
    {"38R", _38R},  {"39", _39},    {"43", _43},    {"44", _44},    {"45", _45},   {"48", _48},
    {"49", _49},    {"5", _5},      {"52", _52},    {"54", _54},    {"55", _55},   {"56", _56},
    {"57", _57},    {"58", _58},    {"5R", _5R},    {"6", _6},      {"66", _66},   {"67", _67},
    {"7", _7},      {"714", _714},  {"8", _8},      {"8AX", _8AX},  {"8BX", _8BX}, {"9", _9},
    {"90", _90},    {"91", _91},    {"9R", _9R},    {"CA", CA},     {"F", F},      {"J", J},
    {"K", K},       {"KBUS", KBUS}, {"LBUS", LBUS}, {"LOWL", LOWL}, {"M", M},      {"N", N},
    {"NBUS", NBUS}, {"NOWL", NOWL}, {"PH", PH},     {"PM", PM},     {"S", S},      {"T", T},
    {"TBUS", TBUS}, {"28R", _28R},
};

MuniLine MuniLine_Parse(const std::string& name) {
  if (parse_map.find(name) == parse_map.end()) {
    return INVALID;
  }
  return parse_map.at(name);
}

}  // namespace sfmtc::muni
