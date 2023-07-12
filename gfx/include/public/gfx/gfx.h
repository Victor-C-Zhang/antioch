// Copyright Antioch. All rights reserved.

#pragma once

#define GFX_API [[nodiscard]]

namespace antioch::gfx {
enum class Result {
  Success,
  InvalidParameter,
  OutOfMemory

};

GFX_API Result createDevice();
GFX_API Result createFramebuffer();

}  // namespace antioch::gfx
