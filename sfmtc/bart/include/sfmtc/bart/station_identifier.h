// Copyright Antioch. All rights reserved.
#pragma once

#include <string>

namespace sfmtc {
namespace bart {

enum StationIdentifier : int {
  INVALID_STATION = 0,
  LAKE = 1,    // Lake Merritt
  FTVL = 2,    // Fruitvale
  COLS = 3,    // Coliseum
  SANL = 4,    // San Leandro
  BAYF = 5,    // Bay Fair
  HAYW = 6,    // Hayward
  SHAY = 7,    // South Hayward
  UCTY = 8,    // Union City
  FRMT = 9,    // Fremont
  ROCK = 10,   // Rockridge
  ORIN = 11,   // Orinda
  LAFY = 12,   // Lafayette
  WCRK = 13,   // Walnut Creek
  PHIL = 14,   // Pleasant Hill
  CONC = 15,   // Concord
  NCON = 16,   // North Concord
  PITT = 17,   // Pittsburg / Bay Point
  PCTR = 18,   // Pittsburg Center
  ANTC = 19,   //
  OAKL = 20,   // Oakland International Airport
  _12TH = 21,  // 12th Street / Oakland City Center
  _19TH = 22,  // 19th Street Oakland
  MCAR = 23,   // MacArthur
  CAST = 24,   // Castro Valley
  WDUB = 25,   // West Dublin / Pleasanton
  DUBL = 26,   // Dublin / Pleasanton
  WOAK = 27,   // West Oakland
  EMBR = 28,   // Embarcadero
  MONT = 29,   // Montgomery Street
  POWL = 30,   // Powell Street
  CIVC = 31,   // Civic Center / UN Plaza
  _16TH = 32,  // 16th Street / Mission
  _24TH = 33,  // 24th Street / Mission
  GLEN = 34,   // Glen Park
  BALB = 35,   // Balboa Park
  DALY = 36,   // Daly City
  ASHB = 37,   // Ashby
  DBRK = 38,   // Downtown Berkeley
  NBRK = 39,   // North Berkeley
  PLZA = 40,   // El Cerrito Plaza
  DELN = 41,   // El Cerrito Del Norte
  RICH = 42,   // Richmond
  WARM = 43,   // Warm Springs
  MLPT = 44,   // Milpitas
  BERY = 45,   // Beryessa / North San Jose
  COLM = 46,   // Colma
  SSAN = 47,   // South San Francisco
  SBRN = 48,   // San Bruno
  MLBR = 49,   // Millbrae (Cantrain Transfer Platform)
  SFIA = 50,   // San Francisco International Airport
};

const std::string StationIdentifier_Name(const StationIdentifier id);

const std::string StationIdentifier_pretty_name(const StationIdentifier id);

}  // namespace bart
}  // namespace sfmtc
