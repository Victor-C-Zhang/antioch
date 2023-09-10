// Copyright Antioch. All rights reserved.

#pragma once

#include <functional>
#include <memory>
#include <thread>
#include <vector>

#include "wifi_connection_error.h"

namespace antioch {
namespace connectivity {
namespace wifi {

typedef std::function<void(void)> StatusCallback;

class WifiService {
 public:
  static std::unique_ptr<WifiService> create();
  ~WifiService();
  bool set_ssid(std::string ssid, std::string psk) noexcept;
  void register_disconnection_cb(StatusCallback cb) noexcept;

  /**
   * Call this function before connect()
   * @throws WifiConnectionError if unable to enable connectivity.
   */
  void start_connectivity();

  /**
   * Call this function after start_connectivity()
   * @throws WifiConnectionError if unable to connect to the AP.
   */
  void connect();

 private:
  WifiService();
  void status_poll_loop();
  bool shutdown = false;
  std::thread status_thr;

  std::vector<StatusCallback> callbacks;
};

}  // namespace wifi
}  // namespace connectivity
}  // namespace antioch
