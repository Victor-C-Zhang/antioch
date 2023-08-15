// Copyright Antioch. All rights reserved.

#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "base/object_base.h"

namespace antioch::gfx {
GFX_API Result createInstance(const InstanceCreateInfo& createInfo,
                              const AllocationCallback* pAllocator, Instance* pInstance) {
  pInstance->instance = IMPL::implAllocateInstance(pAllocator);
  base::Instance_t* instance = reinterpret_cast<base::Instance_t*>(pInstance->instance);

  if (!instance) {
    return Result::eOutOfMemory;
  }

  instance->createInfo = createInfo;

  Result res = IMPL::implCreateInstance(pInstance->instance);

  if (res != Result::eSuccess) {
    IMPL::implDestroyInstance(pInstance->instance, pAllocator);

    return res;
  }

  return Result::eSuccess;
}

GFX_API Result Instance::createDevice(const DeviceCreateInfo& createInfo,
                                      const AllocationCallback* pAllocator, Device* pDevice) {
  pDevice->device = IMPL::implAllocateDevice(pAllocator);
  base::Device_t* device = reinterpret_cast<base::Device_t*>(pDevice->device);

  if (!device) {
    return Result::eOutOfMemory;
  }

  device->createInfo = createInfo;

  Result res = IMPL::implCreateDevice(pDevice->device);

  if (res != Result::eSuccess) {
    IMPL::implDestroyDevice(pDevice->device, pAllocator);

    return res;
  }

  return Result::eSuccess;
}

GFX_API Result Instance::destory(const AllocationCallback* pAllocator) {
  return IMPL::implDestroyInstance(instance, pAllocator);
}
}  // namespace antioch::gfx
