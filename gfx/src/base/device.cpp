// Copyright Antioch. All rights reserved.

#include <gfx/gfx.h>

#include "allocation.h"
#include "function_impl.h"
#include "object_impl.h"

namespace antioch::gfx {

GFX_API Result Device::destory(const AllocationCallback* pAllocator) {
  Result res = IMPL::implDestroyDevice(pAllocator);

  deallocate<Device_t>(pAllocator, device);

  return res;
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

GFX_API Result Device::submit(uint32_t submitCount, const SubmitInfo* pSubmits) {
  return IMPL::implSubmit(submitCount, pSubmits);
}

}  // namespace antioch::gfx
