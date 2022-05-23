cmake_minimum_required(VERSION 3.12)
project(DecoratorsPlugin)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_FIND_DEBUG_MODE 1)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

include_directories(${CLANG_INCLUDE_DIRS})

find_package(fmt)

list(APPEND CMAKE_MODULE_PATH ${LLVM_CMAKE_DIR})
include(AddLLVM)

add_definitions(${LLVM_DEFINITIONS})
if(NOT ${LLVM_ENABLE_RTTI})
    # For non-MSVC compilers
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

set(_SOURCES
    # src/DecorateAttribute.cpp
    src/DecoratorsAction.cpp
    src/FunctionPreconditions.cpp
    src/CodeGeneration.cpp
    src/ASTVisiting.cpp
)

add_llvm_library(DecoratorsPlugin MODULE  ${_SOURCES}
                                  PLUGIN_TOOL clang
                                  LINK_LIBS fmt::fmt)
