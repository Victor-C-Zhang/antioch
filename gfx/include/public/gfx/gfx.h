// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx_enums.h>
#include <gfx/gfx_structs.h>
#include <gfx/gfx_typedefs.h>

#define GFX_API [[nodiscard]]

namespace antioch::gfx {

class Instance;
class Device;
class CommandBuffer;

GFX_API Result createInstance(const InstanceCreateInfo& createInfo,
                              const AllocationCallback* pAllocator, Instance* pInstance);

class Instance {
 public:
  GFX_API Result createDevice(const DeviceCreateInfo& createInfo,
                              const AllocationCallback* pAllocator, Device* pDevice);

  GFX_API Result destory(const AllocationCallback* pAllocator);

 private:
  Instance_t* instance;

  friend Result createInstance(const InstanceCreateInfo& createInfo,
                               const AllocationCallback* pAllocator, Instance* pInstance);
};

class Device {
 public:
  GFX_API Result createCommandBuffer(const CommandBufferCreateInfo& createInfo,
                                     const AllocationCallback* pAllocator,
                                     CommandBuffer* pCommandBuffer);

  GFX_API Result destory(const AllocationCallback* pAllocator);

 private:
  Device_t* device;

  friend Result Instance::createDevice(const DeviceCreateInfo& createInfo,
                                       const AllocationCallback* pAllocator, Device* pDevice);
};

class CommandBuffer {
  GFX_API Result destory(const AllocationCallback* pAllocator);

 private:
  CommandBuffer_t* commandBuffer;

  friend Result Device::createCommandBuffer(const CommandBufferCreateInfo& createInfo,
                                            const AllocationCallback* pAllocator,
                                            CommandBuffer* pCommandBuffer);
};

}  // namespace antioch::gfx
