// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx.h>

#include <vector>

#include "base/object_base.h"

namespace antioch::gfx {

struct TestData {
  std::vector<std::vector<uint8_t>> framesSubmitted;
  uint32_t numFramesSubmitted;
};

extern TestData gTestData;

struct TestDevice_t : public Device_t {};

typedef TestDevice_t* TestDevice;

}  // namespace antioch::gfx
