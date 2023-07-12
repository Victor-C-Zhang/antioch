
set(SRCS
  src/main.cpp
)

add_compile_options(
  -fno-exceptions
)

add_library(antioch-gfx ${SRCS})
target_include_directories(antioch-gfx
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/private
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/public
)

add_subdirectory(tests)
