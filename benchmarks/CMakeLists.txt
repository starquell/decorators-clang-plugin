cmake_minimum_required(VERSION 3.0.0)
project(thesis-examples VERSION 0.1.0)

find_package(benchmark REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

string(APPEND CMAKE_CXX_FLAGS "-fplugin=/root/thesis/clang-plugin/build/DecoratorsPlugin.so")

add_executable(bench-count Count.cpp)
target_link_libraries(bench-count benchmark::benchmark)
