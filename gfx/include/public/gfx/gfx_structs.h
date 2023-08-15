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

};  // namespace antioch::gfx
