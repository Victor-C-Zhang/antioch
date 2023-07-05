// Copyright Antioch. All rights reserved.

#include "antioch/base/event_loop.h"

#include <iostream>
#include <thread>

#include "antioch/base/configerator.h"
#include "antioch/base/constants.h"

namespace antioch::base {

EventLoop::EventLoop() : boot_time(std::chrono::system_clock::now()) {
  tick = boot_time;
  try {
    config = std::move(Configerator::read_or_exception());
  } catch (std::exception& e) {
    // TODO: print to console
    std::cerr << "Using default config, exception reading saved config: " << e.what() << std::endl;
    config = std::move(Configerator::default_config());
  }
}

void EventLoop::run() {
  run_tick();

  // make adjustments for drift here
  auto now = std::chrono::system_clock::now();
  tick += std::chrono::milliseconds(TRANSIT_POLL_MS);
  if (now < tick) {
    std::this_thread::sleep_for(tick - now);
  } else {
    while (now >= tick) {
      tick += std::chrono::milliseconds(TRANSIT_POLL_MS);
    }
  }
}

int EventLoop::run_tick() {}

}  // namespace antioch::base
