// Copyright Antioch. All rights reserved.

#include <gtest/gtest.h>
#include <transfer.h>

#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>

#include "sfmtc/sfmtc_converter.h"

namespace sfmtc::bart {

bool poor_man_latch = false;
std::condition_variable cv;
std::mutex mtx;
std::string data;

TEST(E2E, convert) {

  auto cb = [](std::string str) {
    std::lock_guard lg(mtx);
    std::cout << "Got bart API " << str.size() << " chars" << std::endl;
    data.swap(str);
    poor_man_latch = true;
    cv.notify_all();
  };
  antioch::connectivity::curl_transfer::start_transfer("api.bart.gov/gtfsrt/tripupdate.aspx", true,
                                                       cb);
  SfmtcConverter bc{};
  bc.startTracking({antioch::transit_base::TransitAgency::BART, StationIdentifier::CIVC});
  std::cout << "E" << std::endl;
  {
    std::unique_lock l(mtx);
    cv.wait(l, [&] { return poor_man_latch; });
  }
  std::cout << "F" << std::endl;
  auto res = bc.convert(data);
  for (const auto& station : res) {
    std::cout << station.bart_to_string() << std::endl;
  }
}

TEST(E2E, getCompressed) {
  auto cb = [](std::string str) {
    std::lock_guard lg(mtx);
    std::cout << "Got " << str.size() << " chars" << std::endl;
    std::cout << str << std::endl;
    poor_man_latch = true;
    cv.notify_all();
  };
  antioch::connectivity::curl_transfer::start_transfer("http://httpbin.org/gzip", false,
                                                       cb);
  {
    std::unique_lock l(mtx);
    cv.wait(l, [&] { return poor_man_latch; });
  }
  std::cout << "F" << std::endl;
}

TEST(E2E, getCompressed511) {
  auto cb = [](std::string str) {
    std::lock_guard lg(mtx);
    std::cout << "Got " << str.size() << " chars" << std::endl;
    std::ofstream out{"/home/victor/antioch_data/e2e.pb"};
    out << str;
    out.close();
    poor_man_latch = true;
    cv.notify_all();
  };
  antioch::connectivity::curl_transfer::start_transfer("http://api.511.org/transit/TripUpdates?api_key=1e160508-ca8c-4fe8-9554-0dd4553d9e0a&agency=RG", false,
                                                       cb);
  {
    std::unique_lock l(mtx);
    cv.wait(l, [&] { return poor_man_latch; });
  }
  std::cout << "F" << std::endl;
}

TEST(E2E, convertFromFile) {
  char buf[10000];
  std::ifstream is;
  is.open("/home/victor/temp.pb", std::ios::binary);
  is.seekg(0, std::ios::end);
  int len = is.tellg();
  is.seekg(0, std::ios::beg);
  is.read(buf, len);
  std::string data(buf, len);
  SfmtcConverter bc{};
  bc.startTracking({antioch::transit_base::TransitAgency::BART, StationIdentifier::CIVC});
  auto res = bc.convert(data);
  for (const auto& station : res) {
    std::cout << station.bart_to_string() << std::endl;
  }
}

}  // namespace sfmtc::bart