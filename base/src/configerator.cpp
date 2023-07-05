// Copyright Antioch. All rights reserved.

#include "antioch/base/configerator.h"

namespace antioch::base::Configerator {

using antioch::base::Config;

std::unique_ptr<Config> read_or_exception() {}

void write_or_exception(std::unique_ptr<Config> config) {
  // TODO
}

std::unique_ptr<Config> default_config() {}

}  // namespace antioch::base::Configerator
