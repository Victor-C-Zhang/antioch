// Copyright Antioch. All rights reserved.

#pragma once

#include <gfx/gfx_structs.h>

namespace antioch::gfx {

constexpr FontInfo DEFAULT_FONT = {.letters = {FontLetter{.value = 'L',
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
                                               FontLetter{.value = 'E',
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
                                               FontLetter{.value = 'O',
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
