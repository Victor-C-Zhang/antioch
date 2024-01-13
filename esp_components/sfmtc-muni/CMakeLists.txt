FILE(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../../sfmtc/muni/src/*.cpp")

idf_component_register(SRCS ${SRCS}
                       INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../../sfmtc/muni/include
                       REQUIRES transit-base proto-objs)