cmake_minimum_required(VERSION 3.16)
include (FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
project(antioch)

# Make the default build type Release. If user or another
# project sets a different value than use that
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to default -- Release")
  set(CMAKE_BUILD_TYPE "Release" CACHE
      STRING "Choose the type of build." FORCE)
endif()
message(STATUS "antioch build type ${CMAKE_BUILD_TYPE}")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  message(STATUS "Adding GNU compiler flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  message(STATUS "Adding MSVC compiler flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
else()
  message(STATUS "${CMAKE_CXX_COMPILER_ID} not recognized, no flags added")
endif()

add_compile_options(-fno-rtti)

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

# API Key for SFMTC api.511.org
file(STRINGS "${PROJECT_SOURCE_DIR}/build/api_key_511" API_KEY_511)

# Switch dependencies and HAL impls based on product
include(product/common.cmake)
if(PRODUCT STREQUAL "LINUX")
  include(product/linux.cmake)
elseif(PRODUCT STREQUAL "RPI3")
  include(product/rpi3.cmake)
else()
  message(FATAL_ERROR "${PRODUCT} PRODUCT variable not supported. Aborting.")
endif()

add_subdirectory(gfx)

###############################################################
########################## Libraries ##########################
###############################################################

# Non-blocking curl
add_library(curl-transfer
  connectivity/curl/src/latch.cpp
  connectivity/curl/src/multi_performer.cpp
  connectivity/curl/src/transfer.cpp
)
target_include_directories(curl-transfer PUBLIC connectivity/curl/include)
target_link_libraries(curl-transfer PUBLIC curl)

# Wifi
add_library(libwifi
  connectivity/wifi/src/wifi_service.cpp
)
target_include_directories(libwifi PUBLIC connectivity/wifi/include)
target_link_libraries(libwifi PUBLIC wifi-hal)

# Base transit boilerplate
add_library(transit-base INTERFACE)
target_include_directories(transit-base INTERFACE transit_base/include)
target_link_libraries(transit-base INTERFACE nlohmann_json::nlohmann_json)

# BART
FILE(GLOB_RECURSE bart-srcs bart/src/*.cpp)
add_library(bart ${bart-srcs})
target_include_directories(bart PUBLIC bart/include)
target_link_libraries(bart PUBLIC transit-base curl-transfer proto-objs)

# SFMTC
# BART sub-library
FILE(GLOB_RECURSE sfmtc-bart-srcs sfmtc/bart/src/*.cpp)
add_library(sfmtc-bart ${sfmtc-bart-srcs})
target_include_directories(sfmtc-bart PUBLIC sfmtc/bart/include)
target_link_libraries(sfmtc-bart transit-base proto-objs)
# Muni sub-library
FILE(GLOB_RECURSE sfmtc-muni-srcs sfmtc/muni/*.cpp)
add_library(sfmtc-muni ${sfmtc-muni-srcs})
target_include_directories(sfmtc-muni PUBLIC sfmtc/muni/include)
target_link_libraries(sfmtc-muni transit-base proto-objs)

FILE(GLOB_RECURSE sfmtc-srcs sfmtc/common/src/*.cpp)
add_library(sfmtc ${sfmtc-srcs})
target_include_directories(sfmtc PUBLIC sfmtc/common/include)
target_compile_definitions(sfmtc PRIVATE -DAPI_KEY_511=${API_KEY_511})
add_dependencies(sfmtc transit-base curl-transfer proto-objs)
target_link_libraries(sfmtc PUBLIC transit-base sfmtc-bart sfmtc-muni curl-transfer proto-objs)

# Event loop and core code
FILE(GLOB_RECURSE antioch-base-srcs base/src/*.cpp)
add_library(antioch-base ${antioch-base-srcs})
target_include_directories(antioch-base PUBLIC base/include/)
target_link_libraries(antioch-base PUBLIC transit-base libwifi sfmtc bart nlohmann_json::nlohmann_json)
add_dependencies(antioch-base
  transit-base
  bart
  sfmtc
  libwifi
)

# main exe
add_executable(antioch
  base/main.cpp
)
add_dependencies(antioch
  antioch-base
)
target_link_libraries(antioch PRIVATE
  antioch-base
)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
###############################################################
############################ Tests ############################
###############################################################
  add_executable(curl-transfer-test
    connectivity/curl/test/e2e.cpp
    # connectivity/curl/test/test_runner.cpp
    base/test/test_runner.cpp

  )
  add_dependencies(curl-transfer-test curl-transfer)
  target_include_directories(curl-transfer-test PUBLIC
    connectivity/curl/include
  )
  target_link_libraries(curl-transfer-test PUBLIC
    curl-transfer
    GTest::gtest_main
    GTest::gmock_main
  )

  add_executable(bart-test
    bart/test/e2e.cpp
  )
  add_dependencies(bart-test bart)
  target_link_libraries(bart-test PUBLIC
    bart
    GTest::gtest_main
    GTest::gmock_main
  )

  FILE(GLOB_RECURSE sfmtc-test-srcs sfmtc/common/test/*.cpp)
  add_executable(sfmtc-test ${sfmtc-test-srcs})
  add_dependencies(sfmtc-test sfmtc)
  target_include_directories(sfmtc-test PUBLIC
    sfmtc/common/include
  )
  target_link_libraries(sfmtc-test PUBLIC
    sfmtc
    curl-transfer
    GTest::gtest_main
  )

  add_executable(antioch-base-test
    base/test/trivial_test.cpp
    base/test/test_runner.cpp
  )
  add_dependencies(antioch-base-test antioch-base)
  target_include_directories(antioch-base-test PUBLIC
    base/include
    base/test/include
  )
  target_link_libraries(antioch-base-test PUBLIC
    antioch-base
    GTest::gtest_main
  )
  include(GoogleTest)
  gtest_discover_tests(antioch-base-test)

###############################################################
########################## Utilities ##########################
###############################################################

  add_executable(moonlight-vigil
    moonlight_vigil/main.cpp
  )

  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.22 AND GFX_DEBUG)
    find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

    add_executable(sungazer
      sungazer/main.cpp
    )
    add_dependencies(sungazer antioch-gfx-sdl2)
    target_link_libraries(sungazer PRIVATE antioch-gfx-sdl2)

    if(TARGET SDL2::SDL2main)
        target_link_libraries(sungazer PRIVATE SDL2::SDL2main)
    endif()

    target_link_libraries(sungazer PRIVATE SDL2::SDL2)
  endif()
endif()
