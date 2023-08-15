// Copyright Antioch. All rights reserved.
#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "common/allocation.h"
#include "sdl2/objects.h"

namespace antioch::gfx::IMPL {
CommandBuffer_t* implAllocateCommandBuffer(const AllocationCallback* pAllocator) {
  return antioch::gfx::common::allocate<CommandBuffer_t>(pAllocator);
}

Result implDestroyCommandBuffer(CommandBuffer_t* commandBuffer,
                                const AllocationCallback* pAllocator) {
  antioch::gfx::common::deallocate<CommandBuffer_t>(pAllocator, commandBuffer);
  return Result::eSuccess;
}

}  // namespace antioch::gfx::IMPL
