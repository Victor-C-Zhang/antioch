// Copyright Antioch. All rights reserved.

#include <gtest/gtest.h>
#include <transfer.h>

#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>

#include "bart_converter.h"

namespace antioch::bart {

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
  BartConverter bc{};
  bc.startTracking({antioch::transit_base::TransitAgency::BART, StationIdentifier::CIVC});
  std::cout << "E" << std::endl;
  {
    std::unique_lock l(mtx);
    cv.wait(l, [&] { return poor_man_latch; });
  }
  std::cout << "F" << std::endl;
  auto res = bc.convert(data);
  for (const auto& station : res) {
    std::cout << station.to_string() << std::endl;
  }
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
  BartConverter bc{};
  bc.startTracking({antioch::transit_base::TransitAgency::BART, StationIdentifier::CIVC});
  auto res = bc.convert(data);
  for (const auto& station : res) {
    std::cout << station.to_string() << std::endl;
  }
}

}  // namespace antioch::bart
