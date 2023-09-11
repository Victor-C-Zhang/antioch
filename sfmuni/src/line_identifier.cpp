// #include <fstream>
// #include <iostream>
// #include <vector>
// using namespace std;

// int main() {
//   std::ifstream t("/home/victor/antioch_data/sf_lines.json");
//   t.seekg(0, std::ios::end);
//   size_t size = t.tellg();
//   std::string buffer(size, ' ');
//   t.seekg(0);
//   t.read(&buffer[0], size);

//   cout << "here" << std::endl;
//   int n = buffer.size();
//   int i = 0;
//   vector<string> e;
//   vector<string> names;
//   while (1) {
//     i = buffer.find("\"Id\": \"", i + 1);
//     if (i >= n || i == string::npos) break;
//     cout << "and here" << endl;
//     string s;
//     int j = i + 7;
//     while (buffer[j] != '"') {
//       s += buffer[j];
//       ++j;
//     }
//     e.push_back(s);

//     i = buffer.find("\"Name\": \"", i + 1);
//     if (i >= n || i == string::npos) break;
//     cout << "and also here" << endl;
//     string t;
//     int k = i + 9;
//     while (buffer[k] != '"') {
//       t += buffer[k];
//       ++k;
//     }
//     names.push_back(t);
//   }
//   for (int i = 0; i < e.size(); ++i) {
//     auto s = e[i];
//     string id = s;
//     if (id[0] >= '0' && id[0] <= '9') id = "_" + id;
//     std::cout << id << " = " << i + 1 << ", // " << s << std::endl;
//   }

//   cout << "###########################################" << endl;
//   cout << "###########################################" << endl;
//   cout << "###########################################" << endl;

//   for (int i = 0; i < e.size(); ++i) {
//     auto s = e[i];
//     string id = s;
//     if (id[0] >= '0' && id[0] <= '9') id = "_" + id;
//     std::cout << "case " << id << ":\n  return \"" << names[i] << "\";" << std::endl;
//   }

//   cout << "###########################################" << endl;
//   cout << "###########################################" << endl;
//   cout << "###########################################" << endl;

//   for (int i = 0; i < e.size(); ++i) {
//     auto s = e[i];
//     string id = s;
//     if (id[0] >= '0' && id[0] <= '9') id = "_" + id;
//     std::cout << "case " << id << ":\n  return \"" << s << "\";" << std::endl;
//   }
// }

// Copyright Antioch. All rights reserved.

#include "line_identifier.h"

namespace antioch::sfmuni {

const std::string LineIdentifier_pretty_name(const LineIdentifier id) {
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

const std::string LineIdentifier_Name(const LineIdentifier id) {
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

}  // namespace antioch::sfmuni
