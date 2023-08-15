
FILE(GLOB BASE_SRCS src/base/*.cpp)
FILE(GLOB IMPL_SDL2_SRCS src/sdl2/*.cpp)


add_compile_options(
  -fno-exceptions
)

add_library(antioch-gfx-base ${BASE_SRCS})
target_include_directories(antioch-gfx-base
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/private/base
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/public
)

add_subdirectory(tests)

# Simulator backend

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2main)

add_library(antioch-gfx-sdl2 ${IMPL_SDL2_SRCS})
target_compile_definitions(antioch-gfx-sdl2
  PUBLIC IMPL=sdl2
)
target_include_directories(antioch-gfx-sdl2
  PRIVATE ${SDL2_INCLUDE_DIRS}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/private
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/public
)
