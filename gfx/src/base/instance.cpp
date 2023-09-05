// Copyright Antioch. All rights reserved.

#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "base/object_base.h"

namespace antioch::gfx {
GFX_API Result createInstance(const InstanceCreateInfo& createInfo,
                              const AllocationCallback* pAllocator, Instance* pInstance) {
  pInstance->instance = implAllocateInstance(pAllocator);
  Instance_t* instance = reinterpret_cast<Instance_t*>(pInstance->instance);

  if (!instance) {
    return Result::eOutOfMemory;
  }

  instance->createInfo = createInfo;

  Result res = implCreateInstance(pInstance->instance);

  if (res != Result::eSuccess) {
    implDestroyInstance(pInstance->instance, pAllocator);

    return res;
  }

  return Result::eSuccess;
}

GFX_API Result Instance::createDevice(const DeviceCreateInfo& createInfo,
                                      const AllocationCallback* pAllocator, Device* pDevice) {
  pDevice->device = implAllocateDevice(pAllocator);
  Device_t* device = reinterpret_cast<Device_t*>(pDevice->device);

  if (!device) {
    return Result::eOutOfMemory;
  }

  device->createInfo = createInfo;

  Result res = implCreateDevice(pDevice->device);

  if (res != Result::eSuccess) {
    implDestroyDevice(pDevice->device, pAllocator);

    return res;
  }

  return Result::eSuccess;
}

GFX_API Result Instance::destroy(const AllocationCallback* pAllocator) {
  return implDestroyInstance(instance, pAllocator);
}
}  // namespace antioch::gfx
