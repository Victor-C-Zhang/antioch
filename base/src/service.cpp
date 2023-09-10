// Copyright Antioch. All rights reserved.

#include "antioch/base/service.h"

#include <cstdlib>
#include <iostream>

#include "antioch/base/config/configerator.h"

namespace antioch::base {

using antioch::connectivity::wifi::WifiService;
using antioch::connectivity::wifi::WifiConnectionError;

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

void Service::spin() {
  looper.join();
}

bool Service::gfx_init() {
  return true;
}

bool Service::late_init() {
  try {
    wifi = WifiService::create();
  } catch (std::exception& e) {
    std::cerr << "WifiService create failed: " << e.what() << std::endl;
    return false;
  }

  if (!wifi->set_ssid("MOSS", "UWMOSS71")) {
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

  std::unique_ptr<Config> config;
  try {
    config = std::move(Configerator::read_or_exception());
  } catch (std::exception& e) {
    std::cerr << "Using default config, exception reading saved config: " << e.what() << std::endl;
    config = std::move(Configerator::default_config());
  }
  try {
    Configerator::write_or_exception(*config);
  } catch (const std::exception& e) {
    std::cerr << "Exception writing config: " << e.what() << std::endl;
  }

  event_loop = std::make_unique<EventLoop>(std::move(config));
  
  return true;
}

void Service::stop() {
  std::cerr << "Abort called." << std::endl;
  std::exit(1);
}

}  // namespace antioch::base
