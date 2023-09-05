include (FetchContent)

message("Building for RPI3, installing abseil and protobuf")

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
