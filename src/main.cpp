#include <iostream>

#include "chip8.h"

int main(int argc, char *argv[]) {
    std::cout << "CHIP-8" << std::endl;

    if (argc != 2){
        std::cout << "Usage: chip8 <ROM filepath>" << std::endl;
        return 0;
    }

    Chip8 chip8 = Chip8();
    if (chip8.load_rom(argv[1])){
        // Emulation loop
        while (true){
            chip8.emulate_cycle();
        }
    }

    return 0;
}