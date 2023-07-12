// Copyright Antioch. All rights reserved.

#include <gfx/gfx.h>

#include "allocation.h"

namespace antioch::gfx {

GFX_API Result Device::destory(const AllocationCallback* pAllocator) {
  deallocate<Device_t>(pAllocator, device);

  return Result::eSuccess;
}

GFX_API Result Device::createCommandBuffer(const CommandBufferCreateInfo& createInfo,
                                           const AllocationCallback* pAllocator,
                                           CommandBuffer* pCommandBuffer) {
  pCommandBuffer->commandBuffer = allocate<CommandBuffer_t>(pAllocator);
  CommandBuffer_t* commandBuffer = pCommandBuffer->commandBuffer;

  if (!commandBuffer) {
    return Result::eOutOfMemory;
  }

  commandBuffer->createInfo = createInfo;

  return Result::eSuccess;
}
}  // namespace antioch::gfx
