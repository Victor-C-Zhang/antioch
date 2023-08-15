// Copyright Antioch. All rights reserved.

#include <SDL2/SDL.h>
#include <gfx/gfx.h>

#include "base/function_impl.h"

namespace antioch::gfx::IMPL {

namespace {
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = NULL;

constexpr uint32_t SCREEN_WIDTH = 1210;
constexpr uint32_t SCREEN_HEIGHT = 910;

}  // namespace

Result implCreateDevice(const DeviceCreateInfo& createInfo, const AllocationCallback* pAllocator,
                        Device* pDevice) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return Result::eSystemError;
  } else {
    // Create window
    gWindow = SDL_CreateWindow("Antioch Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      return Result::eSystemError;
    } else {
      // Create renderer for window
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
      if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return Result::eSystemError;
      } else {
        // Initialize renderer color
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
      }
    }
  }

  return Result::eSuccess;
}

Result implDestroyDevice(const AllocationCallback* pAllocator) {
  SDL_DestroyWindow(gWindow);
  gWindow = nullptr;

  SDL_Quit();
}

Result implSubmit(uint32_t submitCount, const SubmitInfo* pSubmits) {}

}  // namespace antioch::gfx::IMPL
