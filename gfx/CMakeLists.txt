
FILE(GLOB BASE_SRCS src/base/*.cpp)

if(ESP_PLATFORM)

FILE(GLOB ESP32_SRCS src/esp32/*.cpp)

idf_component_register(SRCS ${ESP32_SRCS} ${BASE_SRCS}
                       INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/base/include ${CMAKE_CURRENT_SOURCE_DIR}/src/esp32/include ${CMAKE_CURRENT_SOURCE_DIR}/include
                       REQUIRES esp32-hub75 heap)

# Simulator backend
else()

add_compile_options(
  -fno-exceptions
)

FILE(GLOB IMPL_SDL2_SRCS src/sdl2/*.cpp)

add_library(antioch-gfx-base-lib ${BASE_SRCS})
target_include_directories(antioch-gfx-base-lib
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/base/include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Simulator backend
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.22 AND CMAKE_BUILD_TYPE STREQUAL "Debug" AND GFX_DEBUG)

  find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
  
  add_library(antioch-gfx-sdl2-lib ${IMPL_SDL2_SRCS})

  target_include_directories(antioch-gfx-sdl2-lib
    PRIVATE ${SDL2_INCLUDE_DIRS}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/base/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/sdl2/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
  target_link_libraries(antioch-gfx-sdl2-lib PRIVATE SDL2::SDL2)

  add_library(antioch-gfx-sdl2 INTERFACE)
  add_dependencies(antioch-gfx-sdl2 antioch-gfx-base-lib antioch-gfx-sdl2-lib)
  target_link_libraries(antioch-gfx-sdl2 INTERFACE antioch-gfx-base-lib antioch-gfx-sdl2-lib)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_subdirectory(tests)
endif()
endif()
