// Copyright Antioch. All rights reserved.

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

#include <hal/wifi.h>

#include "antioch/connectivity/wifi/wifi_service.h"

namespace antioch {
namespace connectivity {
namespace wifi {

using namespace hal::wifi;

static constexpr int STATUS_POLL_RATE_MS = 1000;

std::unique_ptr<WifiService> WifiService::create() {
  auto retval = new WifiService();
  return std::unique_ptr<WifiService>(retval);
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
  std::cout << "Shutting down wifi" << std::endl;
  shutdown = true;
  status_thr.join();
  wifi_disconnect();
  wifi_stop_connectivity();
  wifi_hal_destruct();
}

bool WifiService::set_ssid(std::string ssid, std::string psk) noexcept {
  return wifi_set_ssid(ssid, psk) == RES_OK;
}

void WifiService::register_disconnection_cb(StatusCallback cb) noexcept {
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
    if (!wifi_is_connected()) {
      for (auto cb : callbacks) {
        std::thread(cb).detach();
      }
    }
  }
}

}  // namespace wifi
}  // namespace connectivity
}  // namespace antioch
