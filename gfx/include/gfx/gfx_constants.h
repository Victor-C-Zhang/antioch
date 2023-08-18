// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx_structs.h>

namespace antioch::gfx {

constexpr uint32_t MAX_PRIMS_PER_DRAW = 50;

constexpr GlyphInfo DEFAULT_FONT = {.letters = {GlyphLetter{.value = 'L',
                                                            .data =
                                                                {
                                                                    0b10000000,
                                                                    0b10000000,
                                                                    0b10000000,
                                                                    0b10000000,
                                                                    0b10000000,
                                                                    0b10000000,
                                                                    0b10000000,
                                                                    0b11111111,
                                                                }},
                                                GlyphLetter{.value = 'E',
                                                            .data =
                                                                {
                                                                    0b11111111,
                                                                    0b10000000,
                                                                    0b10000000,
                                                                    0b11111111,
                                                                    0b10000000,
                                                                    0b10000000,
                                                                    0b10000000,
                                                                    0b11111111,
                                                                }},
                                                GlyphLetter{.value = 'O',
                                                            .data = {
                                                                0b11111111,
                                                                0b10000001,
                                                                0b10000001,
                                                                0b10000001,
                                                                0b10000001,
                                                                0b10000001,
                                                                0b10000001,
                                                                0b11111111,
                                                            }}}};

}  // namespace antioch::gfx
