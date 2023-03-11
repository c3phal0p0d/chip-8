#include <iostream>

#include "chip8.h"

int main() {
    std::cout << "CHIP-8" << std::endl;

    Chip8 chip8;
    chip8.run_cycle();
    chip8.run_cycle();

    return 0;
}