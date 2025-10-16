cmake_minimum_required(VERSION 3.21.0)
set (CMAKE_CXX_STANDART 20)
set (CMAKE_CXX_STANDARD_REQUIRED True)

project(IRGen VERSION 1.0.0)

include(FetchContent)
FetchContent_Declare(
  CLI11
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11
  GIT_TAG        4160d259d961cd393fd8d67590a8c7d210207348 # v2.5.0
)

FetchContent_MakeAvailable(CLI11)

add_executable(ir_gen)
add_subdirectory(src)
