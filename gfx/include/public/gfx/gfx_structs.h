// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx_typedefs.h>

namespace antioch::gfx {

struct AllocationCallback {
  void* pUserData;
  PFN_allocateFunction allocationFunction;
  PFN_freeFunction freeFunction;
};

struct InstanceCreateInfo {};
struct DeviceCreateInfo {};
struct CommandBufferCreateInfo {};
struct FramebufferCreateInfo {};
struct BufferCreateInfo {};
struct ImageViewCreateInfo {};

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
