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

}  // namespace sfmtc::bart
