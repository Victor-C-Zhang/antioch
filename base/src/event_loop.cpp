// Copyright Antioch. All rights reserved.

#include "antioch/base/event_loop.h"

#include <iostream>
#include <map>
#include <thread>

#include <bart_converter.h>
#include "antioch/base/config/configerator.h"
#include "antioch/base/constants.h"

namespace antioch::base {

using antioch::transit_base::Converter;
using antioch::transit_base::TransitAgency;

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
  std::map<TransitAgency, Converter*> staging;
  for (auto& station : config->stations) {
    switch (station.agency()) {
      case TransitAgency::BART: {
        if (staging.find(TransitAgency::BART) == staging.end()) {
          staging[TransitAgency::BART] = new antioch::bart::BartConverter();
        }
        staging[TransitAgency::BART]->startTracking(station);
        break;
      }
      case TransitAgency::SF_MUNI: {
        if (staging.find(TransitAgency::SF_MUNI) == staging.end()) {
          // TODO muni converter
        }
        staging[TransitAgency::SF_MUNI]->startTracking(station);
        break;
      }
      case TransitAgency::INVALID: {
        std::cerr << "Invalid station agency" << std::endl;
        break;
      }
    }
  }
  for (auto& [k,v] : staging) {
    converters.push_back(v);
  }
}

int EventLoop::run_tick() {
  std::cout << "Run tick" <<std::endl;
  for (auto converter : converters) {
    std::cout << "Converter:" << std::endl;
    std::cout << converter->get(civic_center) << std::endl;
  }
  std::cout << "Done run tick" <<std::endl;
  return 0;
}

}  // namespace antioch::base
