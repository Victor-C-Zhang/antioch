// Copyright Antioch. All rights reserved.

#include "antioch/base/event_loop.h"

#include <sfmtc/muni/muni_station.h>
#include <sfmtc/sfmtc_converter.h>

#include <iostream>
#include <map>
#include <thread>

#include "antioch/base/config/configerator.h"
#include "antioch/base/constants.h"
#include "antioch/base/service.h"

namespace antioch::base {

using antioch::transit_base::Converter;
using antioch::transit_base::TransitAgency;

EventLoop::EventLoop(Service* service, std::unique_ptr<Config> cfg)
    : boot_time(std::chrono::system_clock::now()), config(std::move(cfg)), service(service) {
  tick = boot_time;
}

void EventLoop::run() {
  init_from_config(config.get());

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
  for (auto& station : config->stations) {
    switch (station.agency()) {
      case TransitAgency::BART: {
        if (converters.find(TransitAgency::BART) == converters.end()) {
          converters[TransitAgency::BART] = sfmtc::SfmtcConverter::instance();
        }
        converters[TransitAgency::BART]->startTracking(station);
        break;
      }
      case TransitAgency::SF_MUNI: {
        if (converters.find(TransitAgency::SF_MUNI) == converters.end()) {
          converters[TransitAgency::SF_MUNI] = sfmtc::SfmtcConverter::instance();
        }
        converters[TransitAgency::SF_MUNI]->startTracking(station);
        break;
      }
      case TransitAgency::INVALID: {
        std::cerr << "Invalid station agency" << std::endl;
        break;
      }
    }
  }
}

int EventLoop::run_tick() {
  std::cout << "Run tick" << std::endl;
  auto station = service->curr_station();
  std::cout << "Converter:" << std::endl;
  std::cout << converters[station.agency()]->get(station) << std::endl;
  std::cout << "Done run tick" << std::endl;
  return 0;
}

}  // namespace antioch::base
