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

Result implSubmit(Device_t*, uint32_t submitCount, const SubmitInfo* pSubmits) {
  gTestData.numFramesSubmitted++;

  std::vector<uint8_t> frame;

  for (uint32_t n = 0; n < submitCount; n++) {
    RenderTarget renderTarget = pSubmits[n].renderTarget;
    for (uint32_t i = 0; i < renderTarget->createInfo.extents.y; i++) {
      for (uint32_t j = 0; j < renderTarget->createInfo.extents.x; j++) {
        for (uint32_t k = 0; k < renderTarget->createInfo.numChannels; k++) {
          uint8_t val = renderTarget->screen[i * renderTarget->createInfo.extents.x *
                                                 renderTarget->createInfo.numChannels +
                                             j * renderTarget->createInfo.numChannels + k];
          frame.push_back(val);
        }
      }
    }
  }
  gTestData.framesSubmitted.push_back(frame);

  return Result::eSuccess;
}

}  // namespace antioch::gfx
