// Copyright Antioch. All rights reserved.

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

#include <wifi.h>

#include "antioch/connectivity/wifi/wifi_service.h"

namespace antioch {
namespace connectivity {
namespace wifi {

using hal::wifi::wifi_hal_init;
using hal::wifi::wifi_hal_destruct;
using hal::wifi::wifi_set_ssid;
using hal::wifi::wifi_start_connectivity;
using hal::wifi::wifi_stop_connectivity;
using hal::wifi::wifi_connect;
using hal::wifi::wifi_disconnect;

using hal::wifi::wifi_is_disconnected;

using hal::wifi::RES_OK;

static constexpr int STATUS_POLL_RATE_MS = 100;

std::unique_ptr<WifiService> WifiService::create() {
  try {
    return std::make_unique<WifiService>();
  } catch (std::exception& e) {
    std::cerr << "WifiService create failed: " << e.what() << std::endl;
    return {nullptr};
  }
}

WifiService::WifiService() {
  auto res = wifi_hal_init();
  if (res) {
    std::string msg = "Could not init wifi: " + std::to_string(res);
    throw std::runtime_error(msg);
  }
  status_thr = std::thread(&WifiService::status_poll_loop, this);
}

WifiService::~WifiService() {
  shutdown = true;
  status_thr.join();
  wifi_disconnect();
  wifi_stop_connectivity();
  wifi_hal_destruct();
}

bool WifiService::set_ssid(std::string ssid, std::string psk) {
  return wifi_set_ssid(ssid, psk);
}

void WifiService::register_disconnection_cb(StatusCallback cb) {
  callbacks.push_back(cb);
}

void WifiService::start_connectivity() {
  auto res = wifi_start_connectivity();
  if (res != RES_OK) {
    std::cerr << "wifi_start_connectivity returned non-ok " << res << std::endl;
  }
  // TODO throw or return based on res
}

void WifiService::connect() {
  auto res = wifi_connect();
  if (res != RES_OK) {
    std::cerr << "wifi_connect returned non-ok " << res << std::endl;
  }
  // TODO throw based on res
}

void WifiService::status_poll_loop() {
  while (!shutdown) {
    std::this_thread::sleep_for(std::chrono::milliseconds(STATUS_POLL_RATE_MS));
    if (wifi_is_disconnected()) {
      for (auto cb : callbacks) {
        std::thread(cb).detach();
      }
    }
  }
}

}  // namespace wifi
}  // namespace connectivity
}  // namespace antioch
