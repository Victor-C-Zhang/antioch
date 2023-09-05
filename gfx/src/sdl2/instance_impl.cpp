// Copyright Antioch. All rights reserved.

#include <SDL2/SDL.h>
#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "common/allocation.h"
#include "sdl2/objects.h"

namespace antioch::gfx {

Instance_t* implAllocateInstance(const AllocationCallback* pAllocator) {
  return antioch::gfx::common::allocate<SDL2Instance_t>(pAllocator);
}

Result implCreateInstance([[maybe_unused]] Instance_t* instance) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return Result::eSystemError;
  }
  return Result::eSuccess;
}
Result implDestroyInstance(Instance_t* baseInstance, const AllocationCallback* pAllocator) {
  SDL_Quit();
  SDL2Instance_t* instance = static_cast<SDL2Instance_t*>(baseInstance);

  antioch::gfx::common::deallocate<SDL2Instance_t>(pAllocator, instance);

  return Result::eSuccess;
}

}  // namespace antioch::gfx
