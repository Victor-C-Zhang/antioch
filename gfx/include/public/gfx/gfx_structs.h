// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx_typedefs.h>

#include <cstdint>

namespace antioch::gfx {

class CommandBuffer;

struct AllocationCallback {
  void* pUserData;
  PFN_allocateFunction allocationFunction;
  PFN_freeFunction freeFunction;
};

struct InstanceCreateInfo {};
struct DeviceCreateInfo {};
struct CommandBufferCreateInfo {};

struct SubmitInfo {
  uint32_t commandBufferCount;
  const CommandBuffer* pCommandBuffers;
};

struct Instance_t {
  InstanceCreateInfo createInfo;
};

struct Device_t {
  DeviceCreateInfo createInfo;
};

struct CommandBuffer_t {
  CommandBufferCreateInfo createInfo;
};

struct Framebuffer_t {};

};  // namespace antioch::gfx
