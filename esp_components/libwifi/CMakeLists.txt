FILE(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../../connectivity/wifi/src/*.cpp")


idf_component_register(SRCS ${SRCS}
                       INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../../connectivity/wifi/include
                       REQUIRES wifi-hal)
