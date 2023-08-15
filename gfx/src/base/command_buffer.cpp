// Copyright Antioch. All rights reserved.

#include <gfx/gfx.h>

#include "allocation.h"
#include "object_impl.h"

namespace antioch::gfx {

GFX_API Result CommandBuffer::destory(const AllocationCallback* pAllocator) {
  deallocate<CommandBuffer_t>(pAllocator, commandBuffer);

  return Result::eSuccess;
}

}  // namespace antioch::gfx
