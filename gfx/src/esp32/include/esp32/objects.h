// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx.h>

#include <memory>

#include "base/object_base.h"

class MatrixPanel_I2S_DMA;

namespace antioch::gfx {

struct ESP32Instance_t : public Instance_t {};

struct ESP32Device_t : public Device_t {
  MatrixPanel_I2S_DMA* display;
};

struct ESP32CommandBuffer_t : public CommandBuffer_t {};

}  // namespace antioch::gfx
