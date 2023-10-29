// Copyright Antioch. All rights reserved.

#pragma once

#include <chrono>
#include <vector>

#include <antioch/transit_base/converter.h>

#include "config/config.h"
#include "types.h"

namespace antioch {
namespace base {

// forward declaration
class Service;

class EventLoop {
 public:
  EventLoop(Config* cfg);
  void run();
  // special handling to display new station (upon startup) or switch to a new station
  void display_new();

 private:
  void init_from_config(const antioch::base::Config*);
  int run_tick();

  const std::chrono::time_point<std::chrono::system_clock> boot_time;
  std::chrono::time_point<std::chrono::system_clock> tick;
  Config* config;
  std::map<antioch::transit_base::TransitAgency, antioch::transit_base::Converter*> converters;
};

}  // namespace base
}  // namespace antioch
