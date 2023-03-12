#include <iostream>

#include "chip8.h"

unsigned char fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8() {
    PC = 0x200;     // Set program counter to 0x200
    I = 0;          // Reset index register
    opcode = 0;     // Reset current opcode
    SP = 0;         // Reset stack pointer

}

Chip8::~Chip8() {}

void Chip8::load_rom(){
    // TODO
}

void Chip8::run_cycle() {
    std::cout << "Cycle" << std::endl;
    
    // Fetch opcode
    opcode = (memory[PC] << 8) | memory[PC + 1];    // Read 2-byte opcode that the program counter is currently pointing to
    PC += 2;    // Increment program counter to be ready to fetch next opcode

    // Extract digits from the opcode
    uint16_t instruction_type = (opcode & 0xF000) >> 12;    // Type of instruction
    uint16_t X = (opcode & 0x0F00) >> 8;                    // Used to look up one of the 16 registers (VX)
    uint16_t Y = (opcode & 0x00F0) >> 4;                    // Used to look up one of the 16 registers (VY)
    uint16_t N = opcode & 0x000F;                           // 4-bit number
    uint16_t NN = (opcode & 0x00FF);                        // 8-bit immediate number
    uint16_t NNN = (opcode & 0x0FFF);                       // 12-bit immediate memory address

    switch (instruction_type) {
        case 0x0:
            switch (Y){
                 // Clear display
                case 0xE:
                    for (int i=0; i<DISPLAY_WIDTH; ++i){
                        for (int j=0; j<DISPLAY_HEIGHT; ++j){
                            display[i][j] = 0;
                        }
                    }
                    break;
            }
        
        // Jump to memory address NNN
        case 0x1:
            PC = NNN;
            break;

        // Set register VX to NN
        case 0x6:
            V[X] = NN;
            break;
        
        // Add value NN to register VX
        case 0x7:
            V[X] += NN;
            break;

        // Set index register to NNN
        case 0xA:
            I = NNN;
            break;

        // Draw N pixels tall sprite to the screen, from memory location pointed to by I, at coordinates in VX and VY
        case 0xD:
            unsigned int x_coord = V[X] % 64;
            unsigned int y_coord = V[Y] % 32;
            V[0xF] = 0;

            // Iterate through the N rows of the sprite
            for (int row; row < N; row++){
                int sprite_data = memory[I + row];
                
                // Iterate through the pixels in the sprite row
                for (int pixel; pixel < 8; pixel++){
                    // If current pixel in sprite row is on & pixel in display is also on, turn off pixel on display
                    if ((sprite_data + pixel)==1 && (display[x_coord][y_coord])==1){
                        display[x_coord][y_coord] = 0;
                        V[0xF] = 1;
                    }
                    // Otherwise if current pixel in sprite row is on & pixel in display is off, draw pixel on display
                    else {
                        display[x_coord][y_coord] = 1;
                    }

                    // Stop drawing row if at right edge of screen
                    if (x_coord >= 64){
                        break;
                    }

                    x_coord++;
                }

                // Stop drawing if at bottom of screen
                if (y_coord >=32){
                    break;
                }

                y_coord++;
            }

            break;
    }

}