// Copyright Antioch. All rights reserved.

#include <chrono>
#include <thread>

#include <esp_log.h>

#include <hal/wifi.h>
#include <espgazer_runner.h>

static const char* TAG = "main";

void wifi_main() {
  ESP_LOGI(TAG, "Starting wifi");
  hal::wifi::wifi_err_t ret = hal::wifi::wifi_hal_init();
  ESP_LOGI(TAG, "wifi_hal_init ret %du", ret);
  if (ret != hal::wifi::RES_OK)
    return;

  ret = hal::wifi::wifi_set_ssid("MOSS2", "UWMOSS71");
  ESP_LOGI(TAG, "wifi_set_ssid ret %du", ret);
  if (ret != hal::wifi::RES_OK)
    return;

  ret = hal::wifi::wifi_start_connectivity();
  ESP_LOGI(TAG, "wifi_start_connectivity ret %du", ret);
  if (ret != hal::wifi::RES_OK)
    return;

  ret = hal::wifi::wifi_connect();
  ESP_LOGI(TAG, "wifi_connect ret %du", ret);
  if (ret != hal::wifi::RES_OK)
    return;

  while (!hal::wifi::wifi_is_connected()) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ESP_LOGI(TAG, "Waiting for full connection");
  }
  ESP_LOGI(TAG, "Successfully connected to wifi!");

  ESP_LOGI(TAG, "wifi_is_connected %d", hal::wifi::wifi_is_connected());

  ret = hal::wifi::wifi_disconnect();
  ESP_LOGI(TAG, "wifi_disconnect ret %du", ret);
  if (ret != hal::wifi::RES_OK)
    return;
  
  ret = hal::wifi::wifi_stop_connectivity();
  ESP_LOGI(TAG, "wifi_stop_connectivity ret %du", ret);
  if (ret != hal::wifi::RES_OK)
    return;

  ret = hal::wifi::wifi_hal_destruct();
  ESP_LOGI(TAG, "wifi_hal_descruct ret %du", ret);

  return;
}

extern "C" void app_main(void) {
  gazer_main();
}
