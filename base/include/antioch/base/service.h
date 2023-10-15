// Copyright Antioch. All rights reserved.

#pragma once

#include <memory>
#include <thread>

#include <antioch/connectivity/wifi/wifi_service.h>
#include "event_loop.h"

namespace antioch {
namespace base {

class Service {
 public:
  void start();
  void spin();

  // for event loop
  antioch::transit_base::Station curr_station();

 private:
  // starts up the graphics interface
  bool gfx_init();

  // inits wifi, input devices, and reads config
  // TODO: init BT and app listener
  bool late_init();

  // cycle through which of the home stops is displayed to UI
  void cycle_home_stops();

  void stop();

  std::unique_ptr<Config> config;
  size_t curr_station_ptr_;
  std::unique_ptr<antioch::connectivity::wifi::WifiService> wifi;
  std::unique_ptr<EventLoop> event_loop;
  std::thread looper;
};

}  // namespace base
}  // namespace antioch
