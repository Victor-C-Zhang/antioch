// Copyright Antioch. All rights reserved.

#pragma once

#include <chrono>
#include <memory>

#include "types.h"

namespace antioch {
namespace base {

class EventLoop {
 public:
  EventLoop();
  void run();

 private:
  int run_tick();

  const std::chrono::time_point<std::chrono::system_clock> boot_time;
  std::chrono::time_point<std::chrono::system_clock> tick;
  std::unique_ptr<Config> config;
};

}  // namespace base
}  // namespace antioch
