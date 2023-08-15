// Copyright Antioch. All rights reserved.

#include <gfx/gfx.h>

#include "allocation.h"
#include "driver_impl.h"

namespace antioch::gfx {
GFX_API Result createInstance(const InstanceCreateInfo& createInfo,
                              const AllocationCallback* pAllocator, Instance* pInstance) {
  pInstance->instance = allocate<Instance_t>(pAllocator);
  Instance_t* instance = pInstance->instance;

  if (!instance) {
    return Result::eOutOfMemory;
  }

  instance->createInfo = createInfo;

  return Result::eSuccess;
}

GFX_API Result Instance::createDevice(const DeviceCreateInfo& createInfo,
                                      const AllocationCallback* pAllocator, Device* pDevice) {
  pDevice->device = allocate<Device_t>(pAllocator);
  Device_t* device = pDevice->device;

  if (!device) {
    return Result::eOutOfMemory;
  }

  device->createInfo = createInfo;

  Result res = IMPL::implCreateDevice(createInfo, pAllocator, pDevice);

  if (res != Result::eSuccess) {
    deallocate<Device_t>(pAllocator, pDevice->device);
    return res;
  }

  return Result::eSuccess;
}

GFX_API Result Instance::destory(const AllocationCallback* pAllocator) {
  deallocate<Instance_t>(pAllocator, instance);

  return Result::eSuccess;
}
}  // namespace antioch::gfx
