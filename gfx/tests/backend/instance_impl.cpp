// Copyright Antioch. All rights reserved.

#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "base/object_base.h"
#include "common/allocation.h"


namespace antioch::gfx {

Instance_t* implAllocateInstance(const AllocationCallback* pAllocator) {
  return antioch::gfx::common::allocate<Instance_t>(pAllocator);
}

Result implCreateInstance(Instance_t*) { return Result::eSuccess; }

Result implDestroyInstance(Instance_t* instance, const AllocationCallback* pAllocator) {
  antioch::gfx::common::deallocate<Instance_t>(pAllocator, instance);

  return Result::eSuccess;
}

}  // namespace antioch::gfx
