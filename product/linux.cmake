include (FetchContent)

message("Building for LINUX, installing abseil, protobuf, and protoc")

set(BUILD_TESTING OFF) # to disable abseil test
set(ABSL_ENABLE_INSTALL ON) # now you can enable install rules even in subproject... 
set(ABSL_PROPAGATE_CXX_STD ON)  # To avoid CMake warning

FetchContent_Declare(
  absl
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  GIT_TAG 20230125.3
)
FetchContent_MakeAvailable(absl)

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
file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/proto/generated")

add_library(proto-objs OBJECT proto/gtfs-realtime.proto)
target_link_libraries(proto-objs PUBLIC protobuf::libprotobuf)
set(PROTO_BINARY_DIR "${PROJECT_SOURCE_DIR}/proto/generated")
target_include_directories(proto-objs PUBLIC "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>")
protobuf_generate(
    TARGET proto-objs
    IMPORT_DIRS "${PROJECT_SOURCE_DIR}/proto"
    PROTOC_OUT_DIR "${PROTO_BINARY_DIR}"
)

######## HALs ########
include(${PROJECT_SOURCE_DIR}/device/linux/wifi/wifi.cmake)
include(${PROJECT_SOURCE_DIR}/device/linux/input/input.cmake)
