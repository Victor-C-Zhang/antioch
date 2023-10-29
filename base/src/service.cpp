// Copyright Antioch. All rights reserved.

#include "antioch/base/service.h"

#include <hal/input.h>

#include <cstdlib>
#include <iostream>

#include "antioch/base/config/configerator.h"

namespace antioch::base {

using antioch::connectivity::wifi::WifiConnectionError;
using antioch::connectivity::wifi::WifiService;

hal::input::input_callbacks input_cbs = {
    .on_left_short_press =
        []() {
          std::cout << "left short press" << std::endl;
          Service::instance()->cycle_prev_home_stop();
        },
    .on_left_long_press = []() { std::cout << "left long press" << std::endl; },
    .on_right_short_press =
        []() {
          std::cout << "right short press" << std::endl;
          Service::instance()->cycle_next_home_stop();
        },
    .on_right_long_press = []() { std::cout << "right long press" << std::endl; },
    .on_select_short_press = []() { std::cout << "select short press" << std::endl; },
    .on_select_long_press = []() { std::cout << "select long press" << std::endl; },
};
Service* Service::singleton_instance = nullptr;

Service* Service::instance() {
  if (singleton_instance == nullptr) {
    singleton_instance = new Service();
  }
  return singleton_instance;
}

void Service::start() {
  if (!gfx_init()) {
    std::cerr << "No graphics driver available! Aborting." << std::endl;
    return;
  }
  if (!late_init()) {
    // TODO display with gfx
    std::cerr << "Couldn't start up! Shutting down." << std::endl;
  }
  looper = std::thread(&EventLoop::run, event_loop.get());
}

void Service::spin() { looper.join(); }

antioch::transit_base::Station Service::curr_station() {
  return config->stations[curr_station_ptr_];
}

bool Service::gfx_init() { return true; }

bool Service::late_init() {
  try {
    wifi = WifiService::create();
  } catch (std::exception& e) {
    std::cerr << "WifiService create failed: " << e.what() << std::endl;
    return false;
  }

  if (!wifi->set_ssid("SSID", "PWD_WPA_PSK")) {
    std::cerr << "Could not set SSID" << std::endl;
    return false;
  }
  auto disconnection_cb = [&]() {
    std::cout << "wifi disconnected. reconnecting..." << std::endl;
    try {
      wifi->connect();
      std::cout << "Wifi reconnected" << std::endl;
    } catch (WifiConnectionError& e) {
      std::cerr << e.what() << std::endl;
      stop();
    }
  };

  try {
    wifi->register_disconnection_cb(disconnection_cb);
    wifi->start_connectivity();
    std::cout << "Started connectivity" << std::endl;
    wifi->connect();
    std::cout << "Connected to wifi" << std::endl;
  } catch (WifiConnectionError& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }

  auto input_init_res = hal::input::input_init(&input_cbs);
  if (input_init_res != hal::input::RES_OK) {
    std::cerr << "input_init returned nonzero code " << input_init_res << std::endl;
    return false;
  }

  std::unique_ptr<Config> config;
  try {
    config = std::move(Configerator::read_or_exception());
  } catch (std::exception& e) {
    std::cerr << "Using default config, exception reading saved config: " << e.what() << std::endl;
    config = std::move(Configerator::default_config());
  }
  if (config->user_mode == UserMode::HOME_STOP) {
    curr_station_ptr_ = 0;
  }
  try {
    Configerator::write_or_exception(*config);
  } catch (const std::exception& e) {
    std::cerr << "Exception writing config: " << e.what() << std::endl;
  }

  event_loop = std::make_unique<EventLoop>(this, config.get());

  return true;
}

void Service::cycle_next_home_stop() {
  ++curr_station_ptr_;
  curr_station_ptr_ %= config->stations.size();
  event_loop->display_new();
}

void Service::cycle_prev_home_stop() {
  curr_station_ptr_ += config->stations.size() - 1;
  curr_station_ptr_ %= config->stations.size();
  event_loop->display_new();
}

void Service::stop() {
  std::cerr << "Abort called." << std::endl;
  std::exit(1);
}

}  // namespace antioch::base
