// Copyright Antioch. All rights reserved.

#pragma once

#include <memory>
#include <stdexcept>
#include <string>

#include "../types.h"
#include "config.h"

namespace antioch {
namespace base {
namespace Configerator {

std::unique_ptr<antioch::base::Config> read_or_exception();

void write_or_exception(std::unique_ptr<antioch::base::Config> config);

std::unique_ptr<antioch::base::Config> default_config();

class ConfigeratorReadException : public std::runtime_error {
 public:
  ConfigeratorReadException(const std::string& what) : std::runtime_error(what) {}
};

}  // namespace Configerator
}  // namespace base
}  // namespace antioch
