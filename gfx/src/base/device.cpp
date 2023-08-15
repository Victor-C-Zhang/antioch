// Copyright Antioch. All rights reserved.

#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "base/object_base.h"

namespace antioch::gfx {

GFX_API Result Device::destory(const AllocationCallback* pAllocator) {
  return IMPL::implDestroyDevice(device, pAllocator);
}

GFX_API Result Device::createCommandBuffer(const CommandBufferCreateInfo& createInfo,
                                           const AllocationCallback* pAllocator,
                                           CommandBuffer* pCommandBuffer) {
  pCommandBuffer->commandBuffer = IMPL::implAllocateCommandBuffer(pAllocator);
  base::CommandBuffer_t* commandBuffer =
      reinterpret_cast<base::CommandBuffer_t*>(pCommandBuffer->commandBuffer);

  if (!commandBuffer) {
    return Result::eOutOfMemory;
  }

  commandBuffer->createInfo = createInfo;

  return Result::eSuccess;
}

GFX_API Result Device::submit(uint32_t submitCount, const SubmitInfo* pSubmits) {
  return IMPL::implSubmit(device, submitCount, pSubmits);
}

}  // namespace antioch::gfx
