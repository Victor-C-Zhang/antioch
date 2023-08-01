// Copyright Antioch. All rights reserved.

#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>

#include <gtest/gtest.h>

#include "transfer.h"

void cb(std::unique_ptr<char> buf) {
  std::cout << "called end" << std::endl;
  char* e = buf.get();
  printf("%s\n", e);
  std::cout << "done" << std::endl;
}

void cb2(std::unique_ptr<char> buf) {
  std::cout << "called end with ";
  char* e = buf.get();
  int ptr;
  while (e[ptr]) ++ptr;
  std::cout << ptr << std::endl;
  std::cout << "done2" << std::endl;
}

TEST(E2E, curlGoogle) {
  antioch::connectivity::curl_transfer::start_transfer("www.google.com", false, cb);
  std::this_thread::sleep_for(std::chrono::seconds(2));
}

TEST(E2E, curlTwo) {
  antioch::connectivity::curl_transfer::start_transfer("www.google.com", false, cb);
  antioch::connectivity::curl_transfer::start_transfer("api.bart.gov/gtfsrt/tripupdate.aspx", true, cb2);
  std::this_thread::sleep_for(std::chrono::seconds(4));
}
