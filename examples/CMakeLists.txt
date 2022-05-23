cmake_minimum_required(VERSION 3.0.0)
project(thesis-examples VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

string(APPEND CMAKE_CXX_FLAGS "-fplugin=/root/thesis/clang-plugin/build/DecoratorsPlugin.so")

add_executable(example-count Count.cpp)
add_executable(example-repeat Repeat.cpp)
add_executable(example-errors Errors.cpp)
add_executable(example-cached Cached.cpp)
add_executable(example-count-repeat CountAndRepeat.cpp)


