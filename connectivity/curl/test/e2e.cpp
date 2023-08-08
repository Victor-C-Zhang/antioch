// Copyright Antioch. All rights reserved.

#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>

#include <gtest/gtest.h>

#include "transfer.h"

void cb(std::string buf) {
  std::cout << "called end" << std::endl;
  printf("%s\n", buf.c_str());
  std::cout << "done" << std::endl;
}

void cb2(std::string buf) {
  std::cout << "called end with ";
  std::cout << buf.size() << std::endl;
  std::cout << "done2" << std::endl;
}

TEST(E2E, DISABLED_curlGoogle) {
  antioch::connectivity::curl_transfer::start_transfer("www.google.com", false, cb);
  std::this_thread::sleep_for(std::chrono::seconds(2));
}

TEST(E2E, DISABLED_curlTwo) {
  antioch::connectivity::curl_transfer::start_transfer("www.google.com", false, cb);
  antioch::connectivity::curl_transfer::start_transfer("api.bart.gov/gtfsrt/tripupdate.aspx", true, cb2);
  std::this_thread::sleep_for(std::chrono::seconds(4));
}

TEST(E2E, DISABLED_curlApart) {
  antioch::connectivity::curl_transfer::start_transfer("www.google.com", false, cb);
  std::cout << "Start sleep" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "End sleep" << std::endl;
  antioch::connectivity::curl_transfer::start_transfer("api.bart.gov/gtfsrt/tripupdate.aspx", true, cb2);
  std::this_thread::sleep_for(std::chrono::seconds(4));
}
