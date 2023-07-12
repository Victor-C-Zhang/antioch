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

enable_testing()
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

add_subdirectory(gfx)

set(BUILD_TESTING OFF) # to disable abseil test
set(ABSL_ENABLE_INSTALL ON) # now you can enable install rules even in subproject... 
FetchContent_Declare(
  absl
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  GIT_TAG 20230125.3
)
FetchContent_MakeAvailable(absl)

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

FetchContent_Declare(json
URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

set(protobuf_BUILD_TESTS OFF)
FetchContent_Declare(protobuf
  URL https://github.com/protocolbuffers/protobuf/releases/download/v23.4/protobuf-23.4.tar.gz
)
FetchContent_MakeAvailable(protobuf)
# Get source directory of the Protobuf
FetchContent_GetProperties(protobuf SOURCE_DIR Protobuf_SOURCE_DIR)
# Include the script which defines 'protobuf_generate'
include(${Protobuf_SOURCE_DIR}/cmake/protobuf-generate.cmake)
# For generated sources
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated")

add_library(proto-objs OBJECT proto/gtfs-realtime.proto)
target_link_libraries(proto-objs PUBLIC protobuf::libprotobuf)
set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
target_include_directories(proto-objs PUBLIC "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>")
protobuf_generate(
    TARGET proto-objs
    IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}/proto"
    PROTOC_OUT_DIR "${PROTO_BINARY_DIR}"
)

add_library(transit-base INTERFACE)
target_include_directories(transit-base INTERFACE transit_base/include)
target_link_libraries(transit-base INTERFACE nlohmann_json::nlohmann_json)

# Event loop and core code
add_library(antioch-base
  base/src/config/configerator.cpp
  base/src/config/config.cpp
  base/src/event_loop.cpp
)
target_include_directories(antioch-base PUBLIC base/include/)
target_link_libraries(antioch-base PRIVATE transit-base nlohmann_json::nlohmann_json)
# SF Muni
add_library(sfmuni
  sfmuni/src/converter.cpp
)
target_include_directories(sfmuni PUBLIC sfmuni/include transit_base/include)

# BART
add_library(bart
  bart/src/bart_converter.cpp
  bart/src/bart_station.cpp
  bart/src/station_arrivals.cpp
)
target_include_directories(bart PRIVATE bart/include)
target_link_libraries(bart PRIVATE transit-base proto-objs)

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
  gtest
)
include(GoogleTest)
gtest_discover_tests(antioch-base-test)

###############################################################
########################## Utilities ##########################
###############################################################

add_executable(moonlight-vigil
  moonlight_vigil/main.cpp
)
