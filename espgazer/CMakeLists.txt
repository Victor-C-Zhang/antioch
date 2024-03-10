FILE(GLOB SRCS *.cpp)

idf_component_register(SRCS ${SRCS}
                       INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../gfx/include ${CMAKE_CURRENT_SOURCE_DIR}
                       REQUIRES spi_flash esp32-hub75 gfx)
