// Copyright Antioch. All rights reserved.

#include <SDL2/SDL.h>
#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "common/allocation.h"
#include "sdl2/objects.h"

namespace antioch::gfx {

namespace {

constexpr uint32_t CELL_WIDTH = 30;
constexpr uint32_t CELL_HEIGHT = 30;

constexpr uint32_t SCREEN_WIDTH_CELLS = 64;
constexpr uint32_t SCREEN_HEIGHT_CELLS = 32;

constexpr uint32_t SCREEN_WIDTH = CELL_WIDTH * SCREEN_WIDTH_CELLS;
constexpr uint32_t SCREEN_HEIGHT = CELL_HEIGHT * SCREEN_HEIGHT_CELLS;

void drawCircle(SDL_Renderer* renderer, int x, int y, int radius) {
  for (int w = 0; w < radius * 2; w++) {
    for (int h = 0; h < radius * 2; h++) {
      int dx = radius - w;  // horizontal offset
      int dy = radius - h;  // vertical offset
      if ((dx * dx + dy * dy) <= (radius * radius)) {
        SDL_RenderDrawPoint(renderer, x + dx, y + dy);
      }
    }
  }
}

}  // namespace

Device_t* implAllocateDevice(const AllocationCallback* pAllocator) {
  return antioch::gfx::common::allocate<SDL2Device_t>(pAllocator);
}

Result implCreateDevice(Device_t* baseDevice) {
  SDL2Device_t* device = static_cast<SDL2Device_t*>(baseDevice);
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

Result implDestroyDevice(Device_t* baseDevice, const AllocationCallback* pAllocator) {
  SDL2Device_t* device = static_cast<SDL2Device_t*>(baseDevice);

  SDL_DestroyWindow(device->window);
  device->window = nullptr;

  antioch::gfx::common::deallocate<SDL2Device_t>(pAllocator, device);

  return Result::eSuccess;
}

Result implSubmit(Device_t* baseDevice, [[maybe_unused]] uint32_t submitCount,
                  [[maybe_unused]] const SubmitInfo* pSubmits) {
  // TODO: something that isn't a toy example
  SDL2Device_t* device = static_cast<SDL2Device_t*>(baseDevice);

  SDL_SetRenderDrawColor(device->renderer, 0x00, 0x00, 0x00, 0xFF);
  SDL_RenderClear(device->renderer);

  SDL_SetRenderDrawColor(device->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (uint32_t i = 0; i < SCREEN_WIDTH_CELLS; i++) {
    for (uint32_t j = 0; j < SCREEN_HEIGHT_CELLS; j++) {
      SDL_FRect rect = {(float)i * CELL_WIDTH, (float)j * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT};
      SDL_RenderDrawRectF(device->renderer, &rect);
    }
  }

  for (uint32_t i = 0; i < SCREEN_WIDTH_CELLS; i++) {
    for (uint32_t j = 0; j < SCREEN_HEIGHT_CELLS; j++) {
      uint8_t r = device->screen[j * SCREEN_WIDTH_CELLS * 3 + i * 3];
      uint8_t g = device->screen[j * SCREEN_WIDTH_CELLS * 3 + i * 3 + 1];
      uint8_t b = device->screen[j * SCREEN_WIDTH_CELLS * 3 + i * 3 + 2];
      if (r + g + b > 0) {
        SDL_SetRenderDrawColor(device->renderer, r, g, b, 0xFF);

        drawCircle(device->renderer, i * CELL_WIDTH + CELL_WIDTH / 2,
                   j * CELL_HEIGHT + CELL_HEIGHT / 2, 10);
      }
    }
  }

  SDL_RenderPresent(device->renderer);
  return Result::eSuccess;
}

}  // namespace antioch::gfx
