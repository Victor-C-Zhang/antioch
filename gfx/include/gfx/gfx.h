// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx_constants.h>
#include <gfx/gfx_enums.h>
#include <gfx/gfx_structs.h>
#include <gfx/gfx_typedefs.h>

#include <cstdint>

#define GFX_API [[nodiscard]]

namespace antioch::gfx {

class Instance_t;
class Device_t;
class CommandBuffer_t;

class Instance;
class Device;
class CommandBuffer;

GFX_API Result createInstance(const InstanceCreateInfo& createInfo,
                              const AllocationCallback* pAllocator, Instance* pInstance);

class Instance {
 public:
  GFX_API Result createDevice(const DeviceCreateInfo& createInfo,
                              const AllocationCallback* pAllocator, Device* pDevice);

  GFX_API Result destory(const AllocationCallback* pAllocator = nullptr);

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

  GFX_API Result allocateMemory(const MemoryAllocateInfo& allocInfo,
                                const AllocationCallback* pAllocator, DeviceMemory* pDeviceMemory);

  GFX_API Result submit(uint32_t submitCount, const SubmitInfo* pSubmits);

  GFX_API Result destory(const AllocationCallback* pAllocator = nullptr);

 private:
  Device_t* device;

  friend Result Instance::createDevice(const DeviceCreateInfo& createInfo,
                                       const AllocationCallback* pAllocator, Device* pDevice);
};

class CommandBuffer {
 public:
  GFX_API Result begin(const CommandBufferBeginInfo& beginInfo);
  GFX_API Result bindBrush(const BrushInfo& brushInfo);
  GFX_API Result bindGlyph(const GlyphInfo& fontInfo);
  GFX_API Result drawText(const char* text, size_t textSize);
  GFX_API Result end();

  GFX_API Result destory(const AllocationCallback* pAllocator = nullptr);

 private:
  CommandBuffer_t* commandBuffer;

  friend Result Device::createCommandBuffer(const CommandBufferCreateInfo& createInfo,
                                            const AllocationCallback* pAllocator,
                                            CommandBuffer* pCommandBuffer);

  friend Result Device::submit(uint32_t submitCount, const SubmitInfo* pSubmits);
};

}  // namespace antioch::gfx
