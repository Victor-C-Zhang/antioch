// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx.h>

namespace antioch::gfx::base {

struct Instance_t {
  InstanceCreateInfo createInfo;
};

struct Device_t {
  DeviceCreateInfo createInfo;
};

struct CommandBuffer_t {
  CommandBufferCreateInfo createInfo;
};

}  // namespace antioch::gfx::base
