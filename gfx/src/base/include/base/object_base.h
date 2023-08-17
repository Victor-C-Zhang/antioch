// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx.h>

namespace antioch::gfx {

struct Instance_t {
  InstanceCreateInfo createInfo;
};

struct Device_t {
  DeviceCreateInfo createInfo;

  uint8_t screen[64 * 64 * 3];
};

struct CommandBuffer_t {
  CommandBufferCreateInfo createInfo;

  ColourRGB clearColour;

  struct State {
    BrushInfo brush;
    FontInfo font;
  } state;
};

}  // namespace antioch::gfx
