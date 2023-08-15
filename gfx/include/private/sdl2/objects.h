// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx.h>

#include "base/object_base.h"

#ifndef IMPL
#define IMPL sdl2
#endif

struct SDL_Window;
struct SDL_Renderer;

namespace antioch::gfx {

namespace IMPL {

struct Instance_t : public antioch::gfx::base::Instance_t {};

struct Device_t : public antioch::gfx::base::Device_t {
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
};

struct CommandBuffer_t : public antioch::gfx::base::CommandBuffer_t {};

}  // namespace IMPL

}  // namespace antioch::gfx
