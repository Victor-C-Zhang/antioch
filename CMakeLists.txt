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

# SF Muni
add_library(sfmuni
  sfmuni/src/converter.cpp
)
target_include_directories(sfmuni PUBLIC sfmuni/include transit_base/include)

# BART
add_library(bart
  bart/src/bart_converter.cpp
  bart/src/bart_line.cpp
  bart/src/bart_station.cpp
  bart/src/station_arrivals.cpp
  bart/src/station_identifier.cpp
)
target_include_directories(bart PUBLIC bart/include)
target_link_libraries(bart PUBLIC transit-base curl-transfer proto-objs)

# Event loop and core code
add_library(antioch-base
  base/src/config/configerator.cpp
  base/src/config/config.cpp
  base/src/event_loop.cpp
)
target_include_directories(antioch-base PUBLIC base/include/)
target_link_libraries(antioch-base PRIVATE transit-base bart nlohmann_json::nlohmann_json)

# main exe
add_executable(antioch
  base/main.cpp
)
add_dependencies(antioch
  antioch-base
  sfmuni
  bart
)
target_link_libraries(antioch PUBLIC
  antioch-base
  sfmuni
  bart
  nlohmann_json::nlohmann_json
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
    bart/test/test_runner.cpp
    bart/test/e2e.cpp
  )
  add_dependencies(bart-test bart)
  target_include_directories(bart-test PUBLIC
    bart/include
  )
  target_link_libraries(bart-test PUBLIC
    bart
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
