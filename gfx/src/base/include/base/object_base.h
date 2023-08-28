// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx.h>

#include <limits>

namespace antioch::gfx {

struct Instance_t {
  InstanceCreateInfo createInfo;
};

struct Device_t {
  DeviceCreateInfo createInfo;

  uint8_t screen[SCREEN_WIDTH * SCREEN_HEIGHT * NUM_CHANNELS];
};

struct CommandBuffer_t {
  struct Prim {
    Glyph glyph;
    Vector2D offset;
  };

  struct DrawData {
    Vector2D origin;
    uint32_t numPrims;
    Brush brush;
    Prim prims[MAX_PRIMS_PER_DRAW];
  };

  CommandBufferCreateInfo createInfo;

  ColourRGB clearColour;

  struct State {
    Brush brush;
    Glyph font[std::numeric_limits<uint8_t>::max()];

    uint32_t numDraws;
    DrawData drawData[MAX_DRAWS_PER_BUFFER];
  } state;
};

struct DeviceMemory_t {
  MemoryAllocateInfo allocInfo;
  void* data;
};

struct Buffer_t {
  BufferCreateInfo createInfo;
  DeviceMemory memory;
  size_t memoryOffset;
};

struct Image_t {
  ImageCreateInfo createInfo;
  DeviceMemory memory;
  size_t memoryOffset;
};

struct Brush_t {
  BrushCreateInfo createInfo;
};

struct Glyph_t {
  GlyphCreateInfo createInfo;
};

}  // namespace antioch::gfx
