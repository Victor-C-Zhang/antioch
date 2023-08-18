// Copyright Antioch. All rights reserved.

#pragma once

#include <cstddef>

namespace antioch::gfx {

struct DeviceMemory_t;

typedef void* (*PFN_allocateFunction)(void* pUserData, size_t size);
typedef void (*PFN_freeFunction)(void* pUserData, void* memory);

typedef DeviceMemory_t* DeviceMemory;

}  // namespace antioch::gfx
