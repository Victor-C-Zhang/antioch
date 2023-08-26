// Copyright Antioch. All rights reserved.

#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "base/object_base.h"
#include "common/allocation.h"

namespace antioch::gfx {

GFX_API Result Device::destroy(const AllocationCallback* pAllocator) {
  return implDestroyDevice(device, pAllocator);
}

GFX_API Result Device::createCommandBuffer(const CommandBufferCreateInfo& createInfo,
                                           const AllocationCallback* pAllocator,
                                           CommandBuffer* pCommandBuffer) {
  pCommandBuffer->commandBuffer = implAllocateCommandBuffer(pAllocator);
  CommandBuffer_t* commandBuffer =
      reinterpret_cast<CommandBuffer_t*>(pCommandBuffer->commandBuffer);

  if (!commandBuffer) {
    return Result::eOutOfMemory;
  }

  commandBuffer->createInfo = createInfo;

  return Result::eSuccess;
}

GFX_API Result Device::allocateMemory(const MemoryAllocateInfo& allocInfo,
                                      const AllocationCallback* pAllocator,
                                      DeviceMemory* pDeviceMemory) {
  DeviceMemory deviceMemory = antioch::gfx::common::allocate<DeviceMemory_t>(pAllocator);

  if (!deviceMemory) {
    return Result::eOutOfMemory;
  }

  deviceMemory->allocInfo = allocInfo;
  deviceMemory->data = antioch::gfx::common::allocate(pAllocator, allocInfo.allocationSize);

  *pDeviceMemory = deviceMemory;

  return Result::eSuccess;
}

GFX_API Result Device::mapMemory(const DeviceMemory& memory, void** ppData) {
  *ppData = memory->data;

  return Result::eSuccess;
}

GFX_API Result Device::createImage(const ImageCreateInfo& createInfo,
                                   const AllocationCallback* pAllocator, Image* pImage) {
  Image image = antioch::gfx::common::allocate<Image_t>(pAllocator);

  if (!image) {
    return Result::eOutOfMemory;
  }

  image->createInfo = createInfo;

  *pImage = image;

  return Result::eSuccess;
}

GFX_API Result Device::createBrush(const BrushCreateInfo& createInfo,
                                   const AllocationCallback* pAllocator, Brush* pBrush) {
  Brush brush = antioch::gfx::common::allocate<Brush_t>(pAllocator);

  if (!brush) {
    return Result::eOutOfMemory;
  }

  brush->createInfo = createInfo;

  *pBrush = brush;

  return Result::eSuccess;
}

GFX_API Result Device::createGlyph(const GlyphCreateInfo& createInfo,
                                   const AllocationCallback* pAllocator, Glyph* pGlyph) {
  Glyph glyph = antioch::gfx::common::allocate<Glyph_t>(pAllocator);

  if (!glyph) {
    return Result::eOutOfMemory;
  }

  glyph->createInfo = createInfo;

  *pGlyph = glyph;

  return Result::eSuccess;
}

GFX_API Result Device::bindImage(const DeviceMemory& memory, Image& image, size_t offset) {
  image->memory = memory;
  image->memoryOffset = offset;

  return Result::eSuccess;
}

GFX_API Result Device::submit(uint32_t submitCount, const SubmitInfo* pSubmits) {
  for (uint32_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * NUM_CHANNELS; i++) {
    device->screen[i] = 0;
  }

  for (uint32_t i = 0; i < submitCount; ++i) {
    for (uint32_t j = 0; j < pSubmits[i].commandBufferCount; ++j) {
      CommandBuffer_t* commandBuffer = pSubmits[i].pCommandBuffers[j].commandBuffer;
      for (uint32_t k = 0; k < SCREEN_WIDTH * SCREEN_HEIGHT; ++k) {
        device->screen[k * 3] = commandBuffer->clearColour.r;
        device->screen[k * 3 + 1] = commandBuffer->clearColour.g;
        device->screen[k * 3 + 2] = commandBuffer->clearColour.b;
      }

      const CommandBuffer_t::State& state = commandBuffer->state;
      for (uint32_t drawIdx = 0; drawIdx < state.numDraws; ++drawIdx) {
        Brush brush = state.drawData[drawIdx].brush;
        Vector2D offset = state.drawData[drawIdx].origin;
        for (uint32_t primIdx = 0; primIdx < state.drawData[drawIdx].numPrims; ++primIdx) {
          offset += state.drawData[drawIdx].prims[primIdx].offset;
          Glyph glyph = state.drawData[drawIdx].prims[primIdx].glyph;

          uint8_t* pData = static_cast<uint8_t*>(glyph->createInfo.image->memory->data);
          for (uint32_t x = 0; x < 8; ++x) {
            for (uint32_t y = 0; y < 8; ++y) {
              if (x + offset.x >= SCREEN_WIDTH || y + offset.y >= SCREEN_HEIGHT) {
                continue;
              }
              uint32_t screenIdx = ((y + offset.y) * SCREEN_WIDTH + x + offset.x) * NUM_CHANNELS;

              uint32_t memoryIdx = glyph->createInfo.image->memoryOffset + y;

              if ((1 << (7 - x)) & pData[memoryIdx]) {
                device->screen[screenIdx] = brush->createInfo.brushColour.r;
                device->screen[screenIdx + 1] = brush->createInfo.brushColour.g;
                device->screen[screenIdx + 2] = brush->createInfo.brushColour.b;
              }
            }
          }
        }
      }
    }
  }

  return implSubmit(device, submitCount, pSubmits);
}

GFX_API Result Device::destroyImage(Image& image, const AllocationCallback* pAllocator) {
  antioch::gfx::common::deallocate<Image_t>(pAllocator, image);
  return Result::eSuccess;
}

GFX_API Result Device::destroyBrush(Brush& brush, const AllocationCallback* pAllocator) {
  antioch::gfx::common::deallocate<Brush_t>(pAllocator, brush);
  return Result::eSuccess;
}
GFX_API Result Device::destroyGlyph(Glyph& glyph, const AllocationCallback* pAllocator) {
  antioch::gfx::common::deallocate<Glyph_t>(pAllocator, glyph);
  return Result::eSuccess;
}

GFX_API Result Device::freeMemory(DeviceMemory& memory, const AllocationCallback* pAllocator) {
  antioch::gfx::common::deallocate<DeviceMemory_t>(pAllocator, memory);
  return Result::eSuccess;
}

}  // namespace antioch::gfx
