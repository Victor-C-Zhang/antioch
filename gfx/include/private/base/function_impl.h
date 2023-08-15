// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx.h>

namespace antioch::gfx {

namespace IMPL {

class Instance_t;
class Device_t;
class CommandBuffer_t;

Instance_t* implAllocateInstance(const AllocationCallback* pAllocator);
Device_t* implAllocateDevice(const AllocationCallback* pAllocator);
CommandBuffer_t* implAllocateCommandBuffer(const AllocationCallback* pAllocator);

Result implCreateInstance(Instance_t* instance);
Result implDestroyInstance(Instance_t* instance, const AllocationCallback* pAllocator);

Result implCreateDevice(Device_t* device);
Result implDestroyDevice(Device_t* device, const AllocationCallback* pAllocator);
Result implSubmit(Device_t* device, uint32_t submitCount, const SubmitInfo* pSubmits);

Result implDestroyCommandBuffer(CommandBuffer_t* commandBuffer,
                                const AllocationCallback* pAllocator);

}  // namespace IMPL

}  // namespace antioch::gfx
