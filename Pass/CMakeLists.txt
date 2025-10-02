cmake_minimum_required(VERSION 3.0)
project(Pass VERSION 1.0.0)

add_subdirectory(src)

find_package(LLVM REQUIRED)
target_include_directories(Pass PUBLIC ${LLVM_INCLUDE_DIRS})
