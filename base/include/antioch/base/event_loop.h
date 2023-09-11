// Copyright Antioch. All rights reserved.

#pragma once

#include <chrono>
#include <memory>
#include <vector>

#include <antioch/transit_base/converter.h>

#include "config/config.h"
#include "types.h"

namespace antioch {
namespace base {

class EventLoop {
 public:
  EventLoop();
  void run();

 private:
  void init_from_config(const antioch::base::Config*);
  int run_tick();

  const std::chrono::time_point<std::chrono::system_clock> boot_time;
  std::chrono::time_point<std::chrono::system_clock> tick;
  std::unique_ptr<Config> config;
  
  std::vector<antioch::transit_base::Converter*> converters;
};

}  // namespace base
}  // namespace antioch
