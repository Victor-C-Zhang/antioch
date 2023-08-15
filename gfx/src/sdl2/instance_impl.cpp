// Copyright Antioch. All rights reserved.

#include <SDL2/SDL.h>
#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "common/allocation.h"
#include "sdl2/objects.h"

namespace antioch::gfx::IMPL {

Instance_t* implAllocateInstance(const AllocationCallback* pAllocator) {
  return antioch::gfx::common::allocate<Instance_t>(pAllocator);
}

Result implCreateInstance(Instance_t* instance) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return Result::eSystemError;
  }
  return Result::eSuccess;
}
Result implDestroyInstance(Instance_t* instance, const AllocationCallback* pAllocator) {
  SDL_Quit();

  antioch::gfx::common::deallocate<Instance_t>(pAllocator, instance);

  return Result::eSuccess;
}

}  // namespace antioch::gfx::IMPL
