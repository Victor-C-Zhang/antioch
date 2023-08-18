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
  struct Prim {
    uint8_t data[8];
    BrushInfo brush;
  };

  struct DrawData {
    Vector2D origin;
    Prim prims[MAX_PRIMS_PER_DRAW];
  };

  CommandBufferCreateInfo createInfo;

  ColourRGB clearColour;

  struct State {
    BrushInfo brush;
    GlyphInfo font;
  } state;
};

struct DeviceMemory_t {
  MemoryAllocateInfo allocInfo;
  void* data;
};

}  // namespace antioch::gfx
