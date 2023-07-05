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

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

add_library(transit-base INTERFACE)
target_include_directories(transit-base INTERFACE transit_base/include)

# Event loop and core code
add_library(antioch-base
  base/src/configerator.cpp
  base/src/event_loop.cpp
)
target_include_directories(antioch-base PUBLIC base/include/)
target_link_libraries(antioch-base PRIVATE transit-base)
# SF Muni
add_library(sfmuni
  sfmuni/src/converter.cpp
)
target_include_directories(sfmuni PUBLIC sfmuni/include transit_base/include)

# main exe
add_executable(antioch
  base/main.cpp
)
add_dependencies(antioch
  antioch-base
  sfmuni
)
target_link_libraries(antioch PUBLIC
  antioch-base
  sfmuni
)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

add_executable(antioch-base-test
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
  GTest::gmock_main
)
