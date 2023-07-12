// Copyright Antioch. All rights reserved.

#pragma once

#include <cstddef>

namespace antioch::gfx {

typedef void* (*PFN_allocateFunction)(void* pUserData, size_t size);
typedef void (*PFN_freeFunction)(void* pUserData, void* memory);

};  // namespace antioch::gfx
