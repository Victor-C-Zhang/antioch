// Copyright Antioch. All rights reserved.

#include "hal/wifi.h"

#include <esp_wifi.h>

/* heavily inspired by
 * https://github.com/espressif/esp-idf/blob/v5.1.1/examples/wifi/getting_started/station/main/station_example_main.c
 */

namespace hal::wifi {

static constexpr char TAG[] = "[esp-wifi-hal]";
static wifi_config_t _sta_config{
    .sta =
        {
            .ssid = "dummyssid",
            .password = "dummypassword",
            .bssid_set = 0,
        },
};
static bool _wifi_connected = false;

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id,
                               void* event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    ESP_LOGI(TAG, "WIFI_EVENT_STA_START");
    return;
  }
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    ESP_LOGI(TAG, "WIFI_EVENT_STA_DISCONNECTED");
    _wifi_connected = false;
    return;
  }
  if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
    ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    _wifi_connected = true;  // wifi without IP is useless, so semantically we are only
                             // "connected" if we get here
    return;
  }
  ESP_LOGI(TAG, "wifi event %d:%d", event_base, event_id);
}

wifi_err_t wifi_hal_init() {
  auto res = esp_netif_init();
  res = esp_event_loop_create_default();
  // todo error checking
  esp_netif_create_default_wifi_sta();

  auto config = WIFI_INIT_CONFIG_DEFAULT();
  res = esp_wifi_init(&config);
  if (res == ESP_ERR_NO_MEM) {
    return RES_NO_MEM;
  }
  if (res != ESP_OK) {
    return RES_INIT_OTHER;
  }

  // todo error checking
  res = esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler,
                                            nullptr, nullptr);
  res = esp_event_handler_instance_register(IP_EVENT, IP_EVENT_GOT_IP, &wifi_event_handler, nullptr,
                                            nullptr);

  res = esp_wifi_set_mode(WIFI_MODE_STA);
  if (res != ESP_OK) {
    return RES_SET_STA;
  }
  ESP_LOGI(TAG, "done init");
  return RES_OK;
}

wifi_err_t wifi_hal_destruct() {
  esp_wifi_deinit();
  _wifi_connected = false;
  _sta_config.sta.ssid = "dummyssid";
  _sta_config.sta.password = "dummypassword";
  return RES_OK;
}

wifi_err_t wifi_set_ssid(std::string ssid, std::string psk) {
  _sta_config.sta.ssid = ssid;
  _sta_config.sta.password = psk;
  auto res = esp_wifi_set_config(WIFI_IF_STA, &_sta_config);
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
  ESP_LOGI(TAG, "done start_connectivity");
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
  ESP_LOGI(TAG, "done connect");
  return RES_OK;
}

wifi_err_t wifi_disconnect() {
  esp_wifi_disconnect();
  ESP_LOGI(TAG, "done disconnect");
  return RES_OK;
}

bool wifi_is_connected() { return _wifi_connected; }

}  // namespace hal::wifi
