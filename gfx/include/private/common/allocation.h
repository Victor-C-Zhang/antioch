// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx.h>

#include <cassert>
#include <cstdlib>

namespace antioch::gfx::common {
inline void* defaultAllocate([[maybe_unused]] void* pUserData, size_t size) {
  return std::malloc(size);
}

inline void defaultFree([[maybe_unused]] void* pUserData, void* pMemory) { std::free(pMemory); }

static const AllocationCallback defaultAllocator = AllocationCallback{
    .pUserData = nullptr,
    .allocationFunction = defaultAllocate,
    .freeFunction = defaultFree,
};

template <class T>
T* allocate(const AllocationCallback* pAllocator) {
  if (pAllocator == nullptr) {
    pAllocator = &defaultAllocator;
  }
  return reinterpret_cast<T*>(pAllocator->allocationFunction(pAllocator->pUserData, sizeof(T)));
}

template <class T>
void deallocate(const AllocationCallback* pAllocator, T* pMemory) {
  if (pAllocator == nullptr) {
    pAllocator = &defaultAllocator;
  }
  pAllocator->freeFunction(pAllocator->pUserData, pMemory);
  return;
}
}  // namespace antioch::gfx::common
