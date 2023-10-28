FILE(GLOB SRCS *.cpp)


set(COMPONENT_SRCS ${SRCS})
set(COMPONENT_ADD_INCLUDEDIRS "../gfx/include")
set(COMPONENT_REQUIRES spi_flash esp32-hub75 gfx)

register_component()
