// Copyright Antioch. All rights reserved.

#include "antioch/base/event_loop.h"

#include <iostream>
#include <thread>

#include <bart_converter.h>
#include "antioch/base/config/configerator.h"
#include "antioch/base/constants.h"

namespace antioch::base {

static antioch::bart::BartStation civic_center{antioch::bart::StationIdentifier::CIVC};
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
  init_from_config(config.get());
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

void EventLoop::init_from_config(const antioch::base::Config*) {
  converter = new antioch::bart::BartConverter();
  converter->startTracking(civic_center);
}

int EventLoop::run_tick() {
  std::cout << "Run tick" <<std::endl;
  std::cout << converter->get(civic_center) << std::endl;
  std::cout << "Done run tick" <<std::endl;
  return 0;
}

}  // namespace antioch::base
