// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx_enums.h>
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
  uint32_t x;
  uint32_t y;
  uint32_t z;
};

struct Vector2D {
  uint32_t x;
  uint32_t y;

  inline Vector2D operator+(const Vector2D& other) const {
    return {.x = x + other.x, .y = y + other.y};
  }

  inline Vector2D& operator+=(const Vector2D& other) {
    x += other.x;
    y += other.y;
    return *this;
  }
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

struct BufferCreateInfo {
  size_t size;
};

struct ImageCreateInfo {
  Vector2D extents;
  ImageFormat format;
};

struct BrushCreateInfo {
  ColourRGB brushColour;
};

struct GlyphCreateInfo {
  uint8_t value;
  Buffer buffer;
  size_t bufferOffset;
  uint32_t strideBytes;
  Vector2D extent;
  bool isColMajor;
};

struct CommandBufferBeginInfo {
  ColourRGB clearColour;
};

struct SubmitInfo {
  uint32_t commandBufferCount;
  const CommandBuffer* pCommandBuffers;
};

}  // namespace antioch::gfx
