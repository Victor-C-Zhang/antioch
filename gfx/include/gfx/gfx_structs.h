// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx_typedefs.h>

#include <cstdint>

namespace antioch::gfx {

class CommandBuffer;

struct ColourRGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct Vector3D {
  float x;
  float y;
  float z;
};

struct Vector2D {
  float x;
  float y;
};

struct GlyphLetter {
  uint8_t value;
  uint8_t data[8];
};

struct AllocationCallback {
  void* pUserData;
  PFN_allocateFunction allocationFunction;
  PFN_freeFunction freeFunction;
};

struct InstanceCreateInfo {};
struct DeviceCreateInfo {};
struct CommandBufferCreateInfo {};

struct MemoryAllocateInfo {
  size_t allocationSize;
};

struct CommandBufferBeginInfo {
  ColourRGB clearColour;
};

struct BrushInfo {
  ColourRGB brushColour;
};

struct GlyphInfo {
  GlyphLetter letters[3];
};

struct SubmitInfo {
  uint32_t commandBufferCount;
  const CommandBuffer* pCommandBuffers;
};

}  // namespace antioch::gfx
