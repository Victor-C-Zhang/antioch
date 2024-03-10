FILE(GLOB SRCS *.cpp)

set(COMPONENT_SRCS ${SRCS})
set(COMPONENT_REQUIRES wifi-hal pthread espgazer)

register_component()
