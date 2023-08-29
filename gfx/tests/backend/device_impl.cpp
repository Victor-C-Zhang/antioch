// Copyright Antioch. All rights reserved.

#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "common/allocation.h"
#include "test_objects.h"

namespace antioch::gfx {

TestData gTestData;

Device_t* implAllocateDevice(const AllocationCallback* pAllocator) {
  return antioch::gfx::common::allocate<TestDevice_t>(pAllocator);
}

Result implCreateDevice(Device_t*) {
  gTestData.numFramesSubmitted = 0;
  return Result::eSuccess;
}

Result implDestroyDevice(Device_t* device, const AllocationCallback* pAllocator) {
  antioch::gfx::common::deallocate<TestDevice_t>(pAllocator, static_cast<TestDevice_t*>(device));

  return Result::eSuccess;
}

Result implSubmit(Device_t* device, uint32_t, const SubmitInfo*) {
  for (uint32_t i = 0; i < SCREEN_HEIGHT; i++) {
    for (uint32_t j = 0; j < SCREEN_WIDTH; j++) {
      for (uint32_t k = 0; k < NUM_CHANNELS; k++) {
        uint8_t val = device->screen[i * SCREEN_WIDTH * NUM_CHANNELS + j * NUM_CHANNELS + k];
        gTestData.framesSubmitted.push_back(val);
        gTestData.numFramesSubmitted++;
      }
    }
  }
  return Result::eSuccess;
}

}  // namespace antioch::gfx
