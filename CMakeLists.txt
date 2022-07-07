cmake_minimum_required(VERSION 3.12)
set(_PROJECT_NAME starscope)
project(${_PROJECT_NAME} C CXX ASM)

# Compiler options
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_DEBUG_POSTFIX d)

add_compile_options(-Wall
        # -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        # -Wno-unused-function # we have some for the docs that aren't called
        # -Wno-maybe-uninitialized
)

include(./src/starscope/CMakeLists.txt)
