FILE(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../../proto/esp32/generated/*.c")

idf_component_register(SRCS ${SRCS}
                       INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../../proto/esp32/generated
                       REQUIRES protobuf-c)