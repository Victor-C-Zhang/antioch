include (FetchContent)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Installing common dependencies: json, gtest")
else()
    message("Installing common dependencies: json")
endif()

FetchContent_Declare(json
  URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  enable_testing()
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  endif()
  FetchContent_MakeAvailable(googletest)
endif()
