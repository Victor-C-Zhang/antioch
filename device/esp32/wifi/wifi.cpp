// Copyright Antioch. All rights reserved.

#include "hal/wifi.h"

#include <esp_wifi.h>

namespace hal::wifi {

static wifi_config_t sta_config {
    .sta = {
      .ssid = "dummyssid",
      .password = "dummypassword",
      .bssid_set = 0,
    },
};

wifi_err_t wifi_hal_init() {
  auto res = esp_event_loop_init(wifi_event_handler, nullptr);
  
  auto config = WIFI_INIT_CONFIG_DEFAULT();
  res = esp_wifi_init(&config);
  if (res == ESP_ERR_NO_MEM) {
    return RES_NO_MEM;
  }
  if (res != ESP_OK) {
    return RES_INIT_OTHER;
  }
  // TODO esp wifi set storage ?
  res = esp_wifi_set_mode(WIFI_MODE_STA);
  if (res != ESP_OK) {
    return RES_SET_STA;
  }
  return RES_OK;
}

wifi_err_t wifi_hal_destruct() {
  esp_wifi_deinit();
  return RES_OK;
}

wifi_err_t wifi_set_ssid(std::string ssid, std::string psk) {
  sta_config.sta.ssid = ssid;
  sta_config.sta.password = psk;
  auto res = esp_wifi_set_config(WIFI_IF_STA, &sta_config);
  if (res == ESP_ERR_WIFI_NOT_INIT) {
    return RES_NOT_INIT;
  }
  if (res == ESP_ERR_WIFI_NVS) {
    return RES_NVS;
  }
  if (res == ESP_ERR_WIFI_PASSWORD) {
    return RES_PSK;
  }
  if (res != ESP_OK) {
    return RES_SET_CONFIG_OTHER;
  }
  return RES_OK;
}

wifi_err_t wifi_start_connectivity() {
  auto res = esp_wifi_start();
  if (res == ESP_ERR_WIFI_NOT_INIT) {
    return RES_NOT_INIT;
  }
  if (res == ESP_ERR_NO_MEM) {
    return RES_NO_MEM;
  }
  if (res != ESP_OK) {
    return RES_START;
  }
  return RES_OK;
}

wifi_err_t wifi_stop_connectivity() {
  esp_wifi_stop();
  return RES_OK;
}

wifi_err_t wifi_connect() {
  auto res = esp_wifi_connect();
  if (res == ESP_ERR_WIFI_NOT_INIT) {
    return RES_NOT_INIT;
  }
  if (res == ESP_ERR_WIFI_SSID) {
    return RES_WIFI_SSID;
  }
  if (res != RES_OK) {
    return RES_CONN;
  }
  return RES_OK;
}

wifi_err_t wifi_disconnect() {
  esp_wifi_disconnect();
  return RES_OK;
}

bool wifi_is_connected() {
  return true;
}

}  // namespace hal::wifi
