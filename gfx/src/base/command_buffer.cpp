// Copyright Antioch. All rights reserved.

#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "base/object_base.h"

namespace antioch::gfx {

GFX_API Result CommandBuffer::destory(const AllocationCallback* pAllocator) {
  return IMPL::implDestroyCommandBuffer(commandBuffer, pAllocator);
}

}  // namespace antioch::gfx
