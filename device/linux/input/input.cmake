# Copyright Antioch. All rights reserved.

add_library(input-hal
  ${CMAKE_CURRENT_LIST_DIR}/input.cpp
)
target_include_directories(input-hal PUBLIC ${PROJECT_SOURCE_DIR}/device/common/input)
