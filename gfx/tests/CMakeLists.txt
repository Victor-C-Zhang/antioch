FILE(GLOB IMPL_TEST_BACKEND_SRCS backend/*.cpp)
FILE(GLOB IMPL_TEST_SRCS gfx_tests.cpp)


add_library(antioch-gfx-test-backend ${IMPL_TEST_BACKEND_SRCS})

target_include_directories(antioch-gfx-test-backend
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/base/include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/backend/include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

add_library(antioch-gfx-test ${IMPL_TEST_SRCS})
add_dependencies(antioch-gfx-test antioch-gfx-base-lib antioch-gfx-test-backend)
target_link_libraries(antioch-gfx-test 
  PRIVATE antioch-gfx-test-backend
  PRIVATE antioch-gfx-base-lib
  PUBLIC GTest::gtest_main
)