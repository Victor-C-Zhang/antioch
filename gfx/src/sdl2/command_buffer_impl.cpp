// Copyright Antioch. All rights reserved.
#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "common/allocation.h"
#include "sdl2/objects.h"

namespace antioch::gfx {
CommandBuffer_t* implAllocateCommandBuffer(const AllocationCallback* pAllocator) {
  return antioch::gfx::common::allocate<SDL2CommandBuffer_t>(pAllocator);
}

Result implDestroyCommandBuffer(CommandBuffer_t* baseCommandBuffer,
                                const AllocationCallback* pAllocator) {
  SDL2CommandBuffer_t* commandBuffer = static_cast<SDL2CommandBuffer_t*>(baseCommandBuffer);

  antioch::gfx::common::deallocate<SDL2CommandBuffer_t>(pAllocator, commandBuffer);
  return Result::eSuccess;
}

}  // namespace antioch::gfx
