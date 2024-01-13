FILE(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../../sfmtc/bart/src/*.cpp")

idf_component_register(SRCS ${SRCS}
                       INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../../sfmtc/bart/include
                       REQUIRES transit-base proto-objs)