// Copyright Antioch. All rights reserved.

#pragma once

#include <cstdint>
#include <string>

namespace hal {
namespace wifi {

enum wifi_err_t : uint16_t {
  RES_NOOP = 1000,
  RES_OK = 0,

  // esp wifi init
  RES_NO_MEM = 1,
  RES_INIT_OTHER = 2,
  RES_SET_STA = 3,

  RES_NOT_INIT = 4,

  // esp wifi set_config
  RES_NVS = 5,
  RES_PSK = 6,
  RES_SET_CONFIG_OTHER = 7,

  // esp wifi start
  RES_START = 8,

  // esp wifi connect
  RES_WIFI_SSID = 9, // "SSID of AP which station connects is invalid"
  RES_CONN = 10,

  // handler registration
  RES_INVALID_HANDLER_ARG = 11,
  RES_HANDLER_OTHER = 12,
};

[[nodiscard]] wifi_err_t wifi_hal_init();
wifi_err_t wifi_hal_destruct();
[[nodiscard]] wifi_err_t wifi_set_ssid(std::string ssid, std::string psk);
[[nodiscard]] wifi_err_t wifi_start_connectivity();
wifi_err_t wifi_stop_connectivity();
[[nodiscard]] wifi_err_t wifi_connect();
wifi_err_t wifi_disconnect();

bool wifi_is_connected();

}  // namespace wifi
}  // namespace hal
