
FILE(GLOB BASE_SRCS src/base/*.cpp)


add_compile_options(
  -fno-exceptions
)

add_library(antioch-gfx-base ${BASE_SRCS})
target_include_directories(antioch-gfx-base
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/private/base
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/public
)

add_subdirectory(tests)
