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

GFX_API Result Device::createRenderTarget(const RenderTargetCreateInfo& createInfo,
                                          const AllocationCallback* pAllocator,
                                          RenderTarget* pRenderTarget) {
  if (createInfo.extents.x == 0 || createInfo.extents.y == 0) {
    return Result::eInvalidParameter;
  }

  RenderTarget renderTarget = antioch::gfx::common::allocate<RenderTarget_t>(pAllocator);

  if (!renderTarget) {
    return Result::eOutOfMemory;
  }

  renderTarget->createInfo = createInfo;
  renderTarget->screen = reinterpret_cast<uint8_t*>(antioch::gfx::common::allocate(
      pAllocator, createInfo.extents.x * createInfo.extents.y * createInfo.numChannels));

  if (!renderTarget->screen) {
    antioch::gfx::common::deallocate<RenderTarget_t>(pAllocator, renderTarget);
  }

  *pRenderTarget = renderTarget;
  return Result::eSuccess;
}

GFX_API Result Device::createBuffer(const BufferCreateInfo& createInfo,
                                    const AllocationCallback* pAllocator, Buffer* pBuffer) {
  Buffer buffer = antioch::gfx::common::allocate<Buffer_t>(pAllocator);

  if (!buffer) {
    return Result::eOutOfMemory;
  }

  buffer->createInfo = createInfo;

  *pBuffer = buffer;

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
  if (createInfo.extent.x > 8 || createInfo.extent.y > 8) {
    return Result::eInvalidParameter;
  }
  if (createInfo.extent.x == 0 || createInfo.extent.y == 0) {
    return Result::eInvalidParameter;
  }
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

GFX_API Result Device::bindBuffer(const DeviceMemory& memory, Buffer& buffer, size_t offset) {
  buffer->memory = memory;
  buffer->memoryOffset = offset;

  return Result::eSuccess;
}

GFX_API Result Device::submit(uint32_t submitCount, const SubmitInfo* pSubmits) {
  for (uint32_t n = 0; n < submitCount; ++n) {
    RenderTarget renderTarget = pSubmits[n].renderTarget;
    uint32_t width = renderTarget->createInfo.extents.x;
    uint32_t height = renderTarget->createInfo.extents.y;
    uint32_t numChannels = renderTarget->createInfo.numChannels;

    for (uint32_t i = 0; i < width * height * numChannels; i++) {
      renderTarget->screen[i] = 0;
    }

    for (uint32_t i = 0; i < pSubmits[n].commandBufferCount; ++i) {
      CommandBuffer_t* commandBuffer = pSubmits[n].pCommandBuffers[i].commandBuffer;
      for (uint32_t j = 0; j < width * height; ++j) {
        renderTarget->screen[j * 3] = commandBuffer->clearColour.r;
        renderTarget->screen[j * 3 + 1] = commandBuffer->clearColour.g;
        renderTarget->screen[j * 3 + 2] = commandBuffer->clearColour.b;
      }

      const CommandBuffer_t::State& state = commandBuffer->state;
      for (uint32_t drawIdx = 0; drawIdx < state.numDraws; ++drawIdx) {
        Brush brush = state.drawData[drawIdx].brush;
        Vector2D offset = state.drawData[drawIdx].origin;
        for (uint32_t primIdx = 0; primIdx < state.drawData[drawIdx].numPrims; ++primIdx) {
          offset += state.drawData[drawIdx].prims[primIdx].offset;
          Glyph glyph = state.drawData[drawIdx].prims[primIdx].glyph;

          uint8_t* pData = static_cast<uint8_t*>(glyph->createInfo.buffer->memory->data);
          for (uint32_t x = 0; x < glyph->createInfo.extent.x; ++x) {
            for (uint32_t y = 0; y < glyph->createInfo.extent.y; ++y) {
              if (x + offset.x >= width || y + offset.y >= height) {
                continue;
              }
              uint32_t screenIdx = ((y + offset.y) * width + x + offset.x) * numChannels;

              uint32_t memoryIdx =
                  glyph->createInfo.buffer->memoryOffset + glyph->createInfo.bufferOffset;
              uint32_t dataBit = 0;

              if (glyph->createInfo.isColMajor) {
                memoryIdx += x * glyph->createInfo.strideBytes;
                dataBit += y;
              } else {
                memoryIdx += y * glyph->createInfo.strideBytes;
                dataBit += glyph->createInfo.extent.x - 1 - x;
              }

              if ((1 << dataBit) & pData[memoryIdx]) {
                renderTarget->screen[screenIdx] = brush->createInfo.brushColour.r;
                renderTarget->screen[screenIdx + 1] = brush->createInfo.brushColour.g;
                renderTarget->screen[screenIdx + 2] = brush->createInfo.brushColour.b;
              }
            }
          }
        }
      }
    }
  }

  return implSubmit(device, submitCount, pSubmits);
}

GFX_API Result Device::destroyBuffer(Buffer& buffer, const AllocationCallback* pAllocator) {
  antioch::gfx::common::deallocate<Buffer_t>(pAllocator, buffer);
  return Result::eSuccess;
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
