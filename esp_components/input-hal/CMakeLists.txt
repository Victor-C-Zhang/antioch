FILE(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../../device/esp32/input/*.cpp")

idf_component_register(SRCS ${SRCS}
                       INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../../device/common/input)