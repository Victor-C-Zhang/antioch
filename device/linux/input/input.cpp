// Copyright Antioch. All rights reserved.

#include "hal/input.h"

#include <iostream>
#include <mutex>
#include <thread>

namespace hal::input {

static input_callbacks* cb = nullptr;
std::thread* input_thr;

void input_loop() {
  std::string s;
  while (1) {
    std::cin >> s;
    std::cout << "Got input \"" << s << "\"" << std::endl;
    if (s == "ls") {
      cb->on_left_short_press();
    }
    if (s == "ll") {
      cb->on_left_long_press();
    }
    if (s == "rs") {
      cb->on_right_short_press();
    }
    if (s == "rl") {
      cb->on_right_long_press();
    }
    if (s == "ss") {
      cb->on_select_short_press();
    }
    if (s == "sl") {
      cb->on_select_long_press();
    }
  }
}

input_err_t input_init(input_callbacks* callbacks) {
  cb = new input_callbacks(*callbacks);
  input_thr = new std::thread(input_loop);
  input_thr->detach();
  return RES_OK;
}

input_err_t input_destruct() {
  delete cb;
  delete input_thr;  // There's not really a good way to wait for input from console.
                     // This will just leave the thread a zombie for the OS to clean up.
                     // Alternatively, we can poll fd 0 with poll(2) but CBA for desktop linux
  return RES_OK;
}

}  // namespace hal::input
