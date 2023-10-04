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

// forward declaration
class Service;

class EventLoop {
 public:
  EventLoop(Service* service, std::unique_ptr<Config> cfg);
  void run();

 private:
  void init_from_config(const antioch::base::Config*);
  int run_tick();

  const std::chrono::time_point<std::chrono::system_clock> boot_time;
  std::chrono::time_point<std::chrono::system_clock> tick;
  std::unique_ptr<Config> config;
  std::map<antioch::transit_base::TransitAgency, antioch::transit_base::Converter*> converters;
  Service* service;
};

}  // namespace base
}  // namespace antioch
