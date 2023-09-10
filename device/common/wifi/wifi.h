// Copyright Antioch. All rights reserved.

#pragma once

#include <cstdint>
#include <string>

namespace hal {
namespace wifi {

enum wifi_err_t : uint16_t {
  RES_NOOP = 1000,
  RES_OK = 0,
};

[[nodiscard]] wifi_err_t wifi_hal_init();
wifi_err_t wifi_hal_destruct();
[[nodiscard]] wifi_err_t wifi_set_ssid(std::string ssid, std::string psk);
[[nodiscard]] wifi_err_t wifi_start_connectivity();
wifi_err_t wifi_stop_connectivity();
[[nodiscard]] wifi_err_t wifi_connect();
wifi_err_t wifi_disconnect();

bool wifi_is_disconnected();

}  // namespace wifi
}  // namespace hal
