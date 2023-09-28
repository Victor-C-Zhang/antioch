// Copyright Antioch. All rights reserved.

#include "sfmtc/bart/station_identifier.h"

namespace sfmtc::bart {

const std::string StationIdentifier_pretty_name(const StationIdentifier id) {
  switch (id) {
    case INVALID_STATION:
      return "Invalid station";
    case LAKE:
      return "Lake Merritt";
    case FTVL:
      return "Fruitvale";
    case COLS:
      return "Coliseum";
    case SANL:
      return "San Leandro";
    case BAYF:
      return "Bay Fair";
    case HAYW:
      return "Hayward";
    case SHAY:
      return "South Hayward";
    case UCTY:
      return "Union City";
    case FRMT:
      return "Fremont";
    case ROCK:
      return "Rockridge";
    case ORIN:
      return "Orinda";
    case LAFY:
      return "Lafayette";
    case WCRK:
      return "Walnut Creek";
    case PHIL:
      return "Pleasant Hill";
    case CONC:
      return "Concord";
    case NCON:
      return "North Concord";
    case PITT:
      return "Pittsburg / Bay Point";
    case PCTR:
      return "Pittsburg Center";
    case ANTC:
      return "Antioch";
    case OAKL:
      return "Oakland International Airport";
    case _12TH:
      return "12th Street / Oakland City Center";
    case _19TH:
      return "19th Street Oakland";
    case MCAR:
      return "MacArthur";
    case CAST:
      return "Castro Valley";
    case WDUB:
      return "West Dublin / Pleasanton";
    case DUBL:
      return "Dublin / Pleasanton";
    case WOAK:
      return "West Oakland";
    case EMBR:
      return "Embarcadero";
    case MONT:
      return "Montgomery Street";
    case POWL:
      return "Powell Street";
    case CIVC:
      return "Civic Center / UN Plaza";
    case _16TH:
      return "16th Street / Mission";
    case _24TH:
      return "24th Street / Mission";
    case GLEN:
      return "Glen Park";
    case BALB:
      return "Balboa Park";
    case DALY:
      return "Daly City";
    case ASHB:
      return "Ashby";
    case DBRK:
      return "Downtown Berkeley";
    case NBRK:
      return "North Berkeley";
    case PLZA:
      return "El Cerrito Plaza";
    case DELN:
      return "El Cerrito Del Norte";
    case RICH:
      return "Richmond";
    case WARM:
      return "Warm Springs";
    case MLPT:
      return "Milpitas";
    case BERY:
      return "Beryessa / North San Jose";
    case COLM:
      return "Colma";
    case SSAN:
      return "South San Francisco";
    case SBRN:
      return "San Bruno";
    case MLBR:
      return "Millbrae (Cantrain Transfer Platform)";
    case SFIA:
      return "San Francisco International Airport";
  }
  return "Invalid station";
}

const std::string StationIdentifier_Name(const StationIdentifier id) {
  switch (id) {
    case INVALID_STATION:
      return "INVALID_STATION";
    case LAKE:
      return "LAKE";
    case FTVL:
      return "FTVL";
    case COLS:
      return "COLS";
    case SANL:
      return "SANL";
    case BAYF:
      return "BAYF";
    case HAYW:
      return "HAYW";
    case SHAY:
      return "SHAY";
    case UCTY:
      return "UCTY";
    case FRMT:
      return "FRMT";
    case ROCK:
      return "ROCK";
    case ORIN:
      return "ORIN";
    case LAFY:
      return "LAFY";
    case WCRK:
      return "WCRK";
    case PHIL:
      return "PHIL";
    case CONC:
      return "CONC";
    case NCON:
      return "NCON";
    case PITT:
      return "PITT";
    case PCTR:
      return "PCTR";
    case ANTC:
      return "ANTC";
    case OAKL:
      return "OAKL";
    case _12TH:
      return "12TH";
    case _19TH:
      return "19TH";
    case MCAR:
      return "MCAR";
    case CAST:
      return "CAST";
    case WDUB:
      return "WDUB";
    case DUBL:
      return "DUBL";
    case WOAK:
      return "WOAK";
    case EMBR:
      return "EMBR";
    case MONT:
      return "MONT";
    case POWL:
      return "POWL";
    case CIVC:
      return "CIVC";
    case _16TH:
      return "16TH";
    case _24TH:
      return "24TH";
    case GLEN:
      return "GLEN";
    case BALB:
      return "BALB";
    case DALY:
      return "DALY";
    case ASHB:
      return "ASHB";
    case DBRK:
      return "DBRK";
    case NBRK:
      return "NBRK";
    case PLZA:
      return "PLZA";
    case DELN:
      return "DELN";
    case RICH:
      return "RICH";
    case WARM:
      return "WARM";
    case MLPT:
      return "MLPT";
    case BERY:
      return "BERY";
    case COLM:
      return "COLM";
    case SSAN:
      return "SSAN";
    case SBRN:
      return "SBRN";
    case MLBR:
      return "MLBR";
    case SFIA:
      return "SFIA";
  }
  return "INVALID_STATION";
}

}  // namespace sfmtc::bart
