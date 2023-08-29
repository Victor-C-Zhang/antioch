// Copyright Antioch. All rights reserved.
#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "base/object_base.h"
#include "common/allocation.h"

namespace antioch::gfx {
CommandBuffer_t* implAllocateCommandBuffer(const AllocationCallback* pAllocator) {
  return antioch::gfx::common::allocate<CommandBuffer_t>(pAllocator);
}

Result implDestroyCommandBuffer(CommandBuffer_t* commandBuffer,
                                const AllocationCallback* pAllocator) {
  antioch::gfx::common::deallocate<CommandBuffer_t>(pAllocator, commandBuffer);
  return Result::eSuccess;
}

}  // namespace antioch::gfx
