// Copyright Antioch. All rights reserved.

#pragma once

namespace antioch::gfx {
enum class Result {
  eSuccess,
  eInvalidParameter,
  eSystemError,
  eOutOfMemory,
};

enum class ImageFormat {
  eR8G8B8Uint,
  eA1Uint,
};

enum class EdgeBehaviour {
  eClamp,
  eWrap,
};

}  // namespace antioch::gfx
