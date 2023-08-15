// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx.h>

#ifndef IMPL
#define IMPL none
#endif

namespace antioch::gfx {

namespace IMPL {

Result implCreateDevice(const DeviceCreateInfo& createInfo, const AllocationCallback* pAllocator,
                        Device* pDevice);

Result implDestroyDevice(const AllocationCallback* pAllocator);

Result implSubmit(uint32_t submitCount, const SubmitInfo* pSubmits);

}  // namespace IMPL

}  // namespace antioch::gfx
