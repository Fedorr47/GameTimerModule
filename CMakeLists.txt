cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

# Gate MUST be set before project()/enable_language(CXX)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
  "d0edc3af-4c50-42ea-a356-e2862fe7a444")

project(GameTimerModule LANGUAGES CXX)
message(STATUS "CMAKE_CXX_COMPILER_IMPORT_STD='${CMAKE_CXX_COMPILER_IMPORT_STD}'")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Tell CMake we want `import std;` (initializes target property CXX_MODULE_STD)
set(CMAKE_CXX_MODULE_STD 1)

# Optional: verify toolchain actually supports it
if (NOT "23" IN_LIST CMAKE_CXX_COMPILER_IMPORT_STD)
  message(WARNING "Toolchain doesn't report C++23 `import std;` support. Falling back to #include may be needed.")
endif()

option(WITH_TESTS "Build tests" ON)

if (WITH_TESTS)
  enable_testing()
  add_subdirectory(extern/googletest)
  add_subdirectory(tests)
endif()

add_library(GameTimerLib STATIC)

target_sources(GameTimerLib
  PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
      src/GameTimer.cppm
  PRIVATE
      src/GameTimer_impl.cpp
)

set_property(TARGET GameTimerLib PROPERTY CXX_SCAN_FOR_MODULES ON)
target_compile_features(GameTimerLib PUBLIC cxx_std_23)

add_executable(app src/main.cpp)
set_property(TARGET app PROPERTY CXX_SCAN_FOR_MODULES ON)
target_compile_features(app PRIVATE cxx_std_23)
target_link_libraries(app PRIVATE GameTimerLib)

target_link_libraries(app PRIVATE GameTimerLib)