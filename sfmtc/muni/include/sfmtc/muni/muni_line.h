// Copyright Antioch. All rights reserved.
#pragma once

#include <string>

namespace sfmtc {
namespace muni {

enum MuniLine : int {
  INVALID = 0,
  _1 = 1,     // 1
  _12 = 2,    // 12
  _14 = 3,    // 14
  _14R = 4,   // 14R
  _15 = 5,    // 15
  _18 = 6,    // 18
  _19 = 7,    // 19
  _1X = 8,    // 1X
  _2 = 9,     // 2
  _21 = 10,   // 21
  _22 = 11,   // 22
  _23 = 12,   // 23
  _24 = 13,   // 24
  _25 = 14,   // 25
  _27 = 15,   // 27
  _28 = 16,   // 28
  _29 = 17,   // 29
  _30 = 18,   // 30
  _31 = 19,   // 31
  _33 = 20,   // 33
  _35 = 21,   // 35
  _36 = 22,   // 36
  _37 = 23,   // 37
  _38 = 24,   // 38
  _38R = 25,  // 38R
  _39 = 26,   // 39
  _43 = 27,   // 43
  _44 = 28,   // 44
  _45 = 29,   // 45
  _48 = 30,   // 48
  _49 = 31,   // 49
  _5 = 32,    // 5
  _52 = 33,   // 52
  _54 = 34,   // 54
  _55 = 35,   // 55
  _56 = 36,   // 56
  _57 = 37,   // 57
  _58 = 38,   // 58
  _5R = 39,   // 5R
  _6 = 40,    // 6
  _66 = 41,   // 66
  _67 = 42,   // 67
  _7 = 43,    // 7
  _714 = 44,  // 714
  _8 = 45,    // 8
  _8AX = 46,  // 8AX
  _8BX = 47,  // 8BX
  _9 = 48,    // 9
  _90 = 49,   // 90
  _91 = 50,   // 91
  _9R = 51,   // 9R
  CA = 52,    // CA
  F = 53,     // F
  J = 54,     // J
  K = 55,     // K
  KBUS = 56,  // KBUS
  LBUS = 57,  // LBUS
  LOWL = 58,  // LOWL
  M = 59,     // M
  N = 60,     // N
  NBUS = 61,  // NBUS
  NOWL = 62,  // NOWL
  PH = 63,    // PH
  PM = 64,    // PM
  S = 65,     // S
  T = 66,     // T
  TBUS = 67,  // TBUS
  _28R = 68,  // 28R
};

const std::string MuniLine_Name(const MuniLine id);

const std::string MuniLine_pretty_name(const MuniLine id);

MuniLine MuniLine_Parse(const std::string& name);

}  // namespace muni
}  // namespace sfmtc