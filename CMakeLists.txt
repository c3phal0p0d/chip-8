cmake_minimum_required(VERSION 3.20)
project(CHIP-8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

set(SOURCE_FILES src/main.cpp src/chip8.cpp src/chip8.h)
add_executable(chip8 ${SOURCE_FILES})

INCLUDE(FindPkgConfig)
