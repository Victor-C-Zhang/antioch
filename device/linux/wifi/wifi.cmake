# Copyright Antioch. All rights reserved.

add_library(wifi-hal
  ${CMAKE_CURRENT_LIST_DIR}/wifi.cpp
)
target_include_directories(wifi-hal PUBLIC ${PROJECT_SOURCE_DIR}/device/common/wifi)
