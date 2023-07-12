// Copyright Antioch. All rights reserved.

#include "antioch/base/event_loop.h"

#include <iostream>
#include <thread>

#include "antioch/base/config/configerator.h"
#include "antioch/base/constants.h"

namespace antioch::base {

static int e = 1;

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
  try {
    Configerator::write_or_exception(*config);
  } catch (const std::exception& e) {
    std::cerr << "Exception writing config: " << e.what() << std::endl;
  }

  while (1) {
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
      tick -= std::chrono::milliseconds(TRANSIT_POLL_MS);  // otherwise we will skip
    }
  }
}

int EventLoop::run_tick() {
  auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::cout << "Tick: " << std::ctime(&time) << std::endl;
  if (e % 10 == 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  }
  ++e;
  return e;
}

}  // namespace antioch::base
