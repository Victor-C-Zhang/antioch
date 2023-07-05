// Copyright Antioch. All rights reserved.

#pragma once

#include <memory>

#include "types.h"

namespace antioch {
namespace base {
namespace Configerator {

std::unique_ptr<antioch::base::Config> read_or_exception();

void write_or_exception(std::unique_ptr<antioch::base::Config> config);

std::unique_ptr<antioch::base::Config> default_config();

}  // namespace Configerator
}  // namespace base
}  // namespace antioch
