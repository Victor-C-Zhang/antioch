// Copyright Antioch. All rights reserved.

#include <SDL2/SDL.h>
#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "common/allocation.h"
#include "sdl2/objects.h"

namespace antioch::gfx::IMPL {

namespace {

constexpr uint32_t SCREEN_WIDTH = 1210;
constexpr uint32_t SCREEN_HEIGHT = 910;

}  // namespace

Device_t* implAllocateDevice(const AllocationCallback* pAllocator) {
  return antioch::gfx::common::allocate<Device_t>(pAllocator);
}

Result implCreateDevice(Device_t* device) {
  // Create window
  device->window =
      SDL_CreateWindow("Sungazer - An Antioch Simulator", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (device->window == nullptr) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return Result::eSystemError;
  } else {
    // Create renderer for window
    device->renderer = SDL_CreateRenderer(device->window, -1, SDL_RENDERER_ACCELERATED);
    if (device->renderer == NULL) {
      printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
      return Result::eSystemError;
    } else {
      // Initialize renderer color
      SDL_SetRenderDrawColor(device->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    }
  }

  return Result::eSuccess;
}

Result implDestroyDevice(Device_t* device, const AllocationCallback* pAllocator) {
  SDL_DestroyWindow(device->window);
  device->window = nullptr;

  antioch::gfx::common::deallocate<Device_t>(pAllocator, device);

  return Result::eSuccess;
}

Result implSubmit(Device_t* device, uint32_t submitCount, const SubmitInfo* pSubmits) {
  // TODO: something that isn't a toy example

  SDL_SetRenderDrawColor(device->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(device->renderer);

  for (uint32_t i = 0; i < submitCount; i++) {
    SDL_SetRenderDrawColor(device->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_FRect rect = {(float)SCREEN_WIDTH / submitCount * i, (float)SCREEN_HEIGHT / submitCount * i,
                      50, 50};
    SDL_RenderFillRectF(device->renderer, &rect);
  }

  SDL_RenderPresent(device->renderer);
  return Result::eSuccess;
}

}  // namespace antioch::gfx::IMPL
