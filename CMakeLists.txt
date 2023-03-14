cmake_minimum_required(VERSION 3.20)
project(CHIP-8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

set(SOURCE_FILES src/main.cpp src/chip8.cpp src/chip8.h)
add_executable(chip8 ${SOURCE_FILES})

target_include_directories(
    chip8 PRIVATE /usr/local/include/SDL2
)

target_link_libraries(
    chip8 PRIVATE /usr/local/lib/libSDL2-2.0.0.dylib
)