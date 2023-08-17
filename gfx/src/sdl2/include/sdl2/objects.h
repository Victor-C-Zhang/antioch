// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx.h>

#include "base/object_base.h"

struct SDL_Window;
struct SDL_Renderer;

namespace antioch::gfx {

struct SDL2Instance_t : public Instance_t {};

struct SDL2Device_t : public Device_t {
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
};

struct SDL2CommandBuffer_t : public CommandBuffer_t {};

}  // namespace antioch::gfx
