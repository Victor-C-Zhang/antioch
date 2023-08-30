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

  GFX_API Result destroy(const AllocationCallback* pAllocator = nullptr);

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
  GFX_API Result createRenderTarget(const RenderTargetCreateInfo& createInfo,
                                    const AllocationCallback* pAllocator,
                                    RenderTarget* pRenderTarget);
  GFX_API Result createBuffer(const BufferCreateInfo& createInfo,
                              const AllocationCallback* pAllocator, Buffer* pBuffer);
  GFX_API Result createImage(const ImageCreateInfo& createInfo,
                             const AllocationCallback* pAllocator, Image* pImage);
  GFX_API Result createBrush(const BrushCreateInfo& createInfo,
                             const AllocationCallback* pAllocator, Brush* pBrush);
  GFX_API Result createGlyph(const GlyphCreateInfo& createInfo,
                             const AllocationCallback* pAllocator, Glyph* pGlyph);
  GFX_API Result allocateMemory(const MemoryAllocateInfo& allocInfo,
                                const AllocationCallback* pAllocator, DeviceMemory* pDeviceMemory);

  GFX_API Result mapMemory(const DeviceMemory& memory, void** ppData);

  GFX_API Result bindImage(const DeviceMemory& memory, Image& image, size_t offset);
  GFX_API Result bindBuffer(const DeviceMemory& memory, Buffer& buffer, size_t offset);

  GFX_API Result submit(uint32_t submitCount, const SubmitInfo* pSubmits);

  GFX_API Result destroy(const AllocationCallback* pAllocator = nullptr);
  GFX_API Result destroyBuffer(Buffer& buffer, const AllocationCallback* pAllocator = nullptr);
  GFX_API Result destroyImage(Image& image, const AllocationCallback* pAllocator = nullptr);
  GFX_API Result destroyBrush(Brush& brush, const AllocationCallback* pAllocator = nullptr);
  GFX_API Result destroyGlyph(Glyph& glyph, const AllocationCallback* pAllocator = nullptr);
  GFX_API Result freeMemory(DeviceMemory& memory, const AllocationCallback* pAllocator = nullptr);

 private:
  Device_t* device;

  friend Result Instance::createDevice(const DeviceCreateInfo& createInfo,
                                       const AllocationCallback* pAllocator, Device* pDevice);
};

class CommandBuffer {
 public:
  GFX_API Result begin(const CommandBufferBeginInfo& beginInfo);

  GFX_API Result bindBrush(const Brush& brush);
  GFX_API Result bindGlyph(uint32_t glpyhCount, const Glyph* pGlyphs);

  GFX_API Result drawText(const char* text, uint32_t textCount, const Vector2D& origin);

  GFX_API Result end();

  GFX_API Result reset();

  GFX_API Result destory(const AllocationCallback* pAllocator = nullptr);

 private:
  CommandBuffer_t* commandBuffer;

  friend Result Device::createCommandBuffer(const CommandBufferCreateInfo& createInfo,
                                            const AllocationCallback* pAllocator,
                                            CommandBuffer* pCommandBuffer);

  friend Result Device::submit(uint32_t submitCount, const SubmitInfo* pSubmits);
};

}  // namespace antioch::gfx
