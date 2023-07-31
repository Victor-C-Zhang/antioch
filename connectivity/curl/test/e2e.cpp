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
}

TEST(E2E, curlGoogle) {
  antioch::connectivity::curl_transfer::start_transfer("www.google.com", cb);
  std::this_thread::sleep_for(std::chrono::seconds(2));
}