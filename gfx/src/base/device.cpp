// Copyright Antioch. All rights reserved.

#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "base/object_base.h"

namespace antioch::gfx {

GFX_API Result Device::destory(const AllocationCallback* pAllocator) {
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

GFX_API Result Device::submit(uint32_t submitCount, const SubmitInfo* pSubmits) {
  for (uint32_t i = 0; i < 64 * 64 * 3; i++) {
    device->screen[i] = 0;
  }

  for (uint32_t i = 0; i < submitCount; ++i) {
    for (uint32_t j = 0; j < pSubmits[i].commandBufferCount; ++j) {
      CommandBuffer_t* commandBuffer = pSubmits[i].pCommandBuffers[j].commandBuffer;
      for (uint32_t i = 0; i < 64 * 64; i++) {
        device->screen[i * 3] = commandBuffer->clearColour.r;
        device->screen[i * 3 + 1] = commandBuffer->clearColour.g;
        device->screen[i * 3 + 2] = commandBuffer->clearColour.b;
      }
    }
  }

  return implSubmit(device, submitCount, pSubmits);
}

}  // namespace antioch::gfx
