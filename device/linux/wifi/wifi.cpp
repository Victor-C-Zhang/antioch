// Copyright Antioch. All rights reserved.

#include "wifi.h"

namespace hal::wifi {

wifi_err_t wifi_hal_init() {
  return RES_OK;
}

wifi_err_t wifi_hal_destruct() {
  return RES_OK;
}

wifi_err_t wifi_set_ssid(std::string, std::string) {
  return RES_OK;
}

wifi_err_t wifi_start_connectivity() {
  return RES_OK;
}

wifi_err_t wifi_stop_connectivity() {
  return RES_OK;
}

wifi_err_t wifi_connect() {
  return RES_OK;
}

wifi_err_t wifi_disconnect() {
  return RES_OK;
}

bool wifi_is_disconnected() {
  return true;
}

}  // namespace hal::wifi