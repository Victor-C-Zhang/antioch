
FILE(GLOB BASE_SRCS src/base/*.cpp)
FILE(GLOB IMPL_SDL2_SRCS src/sdl2/*.cpp)


add_compile_options(
  -fno-exceptions
)

add_subdirectory(tests)

# Simulator backend
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.22)

  find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
  
  add_library(antioch-gfx-sdl2 ${BASE_SRCS} ${IMPL_SDL2_SRCS})
  target_compile_definitions(antioch-gfx-sdl2
    PUBLIC IMPL=sdl2
  )
  target_include_directories(antioch-gfx-sdl2
    PRIVATE ${SDL2_INCLUDE_DIRS}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/private
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/public
  )
endif()