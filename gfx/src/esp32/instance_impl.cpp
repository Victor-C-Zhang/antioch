// Copyright Antioch. All rights reserved.

#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "common/allocation.h"
#include "esp32/objects.h"

namespace antioch::gfx {

Instance_t* implAllocateInstance(const AllocationCallback* pAllocator) {
  return antioch::gfx::common::allocate<ESP32Instance_t>(pAllocator);
}

Result implCreateInstance([[maybe_unused]] Instance_t* instance) { return Result::eSuccess; }
Result implDestroyInstance(Instance_t* baseInstance, const AllocationCallback* pAllocator) {
  ESP32Instance_t* instance = static_cast<ESP32Instance_t*>(baseInstance);

  antioch::gfx::common::deallocate<ESP32Instance_t>(pAllocator, instance);

  return Result::eSuccess;
}

}  // namespace antioch::gfx
