// Copyright Antioch. All rights reserved.

#pragma once

#include <cstdint>
#include <string>

namespace hal {
namespace wifi {

enum wifi_err_t : int16_t {
  RES_OK = 0,
};

wifi_err_t wifi_hal_init();
wifi_err_t wifi_hal_destruct();
wifi_err_t wifi_set_ssid(std::string ssid, std::string psk);
wifi_err_t wifi_start_connectivity();
wifi_err_t wifi_stop_connectivity();
wifi_err_t wifi_connect();
wifi_err_t wifi_disconnect();

bool wifi_is_disconnected();

}  // namespace wifi
}  // namespace hal