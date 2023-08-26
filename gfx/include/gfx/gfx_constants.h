// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx_structs.h>

namespace antioch::gfx {

constexpr uint32_t MAX_PRIMS_PER_DRAW = 50;
constexpr uint32_t MAX_DRAWS_PER_BUFFER = 1;

constexpr uint32_t SCREEN_WIDTH = 64;
constexpr uint32_t SCREEN_HEIGHT = 32;
constexpr uint32_t NUM_CHANNELS = 3;

namespace default_glyphs {

// clang-format off

constexpr uint8_t kL[8] = {
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b11111111,
};

constexpr uint8_t kE[8] = {
    0b11111111,
    0b10000000,
    0b10000000,
    0b11111111,
    0b10000000,
    0b10000000,
    0b10000000,
    0b11111111,
};

constexpr uint8_t kO[8] = {
    0b11111111,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b11111111,
};

// clang-format on

}  // namespace default_glyphs

}  // namespace antioch::gfx
