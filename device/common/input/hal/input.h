// Copyright Antioch. All rights reserved.

#pragma once

#include <cstdint>
#include <string>

namespace hal {
namespace input {

enum input_err_t : uint16_t {
  RES_NOOP = 1000,
  RES_OK = 0,
};

struct input_callbacks {
  void (*on_left_short_press)(void);
  void (*on_left_long_press)(void);
  void (*on_right_short_press)(void);
  void (*on_right_long_press)(void);
  void (*on_select_short_press)(void);
  void (*on_select_long_press)(void);
};

[[nodiscard]] input_err_t input_init(input_callbacks* callbacks);
input_err_t input_destruct();


}  // namespace input
}  // namespace hal
