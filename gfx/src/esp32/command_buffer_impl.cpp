// Copyright Antioch. All rights reserved.
#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "common/allocation.h"
#include "esp32/objects.h"

namespace antioch::gfx {
CommandBuffer_t* implAllocateCommandBuffer(const AllocationCallback* pAllocator) {
  return antioch::gfx::common::allocate<ESP32CommandBuffer_t>(pAllocator);
}

Result implDestroyCommandBuffer(CommandBuffer_t* baseCommandBuffer,
                                const AllocationCallback* pAllocator) {
  ESP32CommandBuffer_t* commandBuffer = static_cast<ESP32CommandBuffer_t*>(baseCommandBuffer);

  antioch::gfx::common::deallocate<ESP32CommandBuffer_t>(pAllocator, commandBuffer);
  return Result::eSuccess;
}

}  // namespace antioch::gfx
