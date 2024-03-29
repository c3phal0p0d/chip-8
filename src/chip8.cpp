#include <iostream>
#include <fstream>
#include <vector>

#include "chip8.h"

#define FONTSET_SIZE 80

unsigned char fontset[FONTSET_SIZE] = {
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

Chip8::Chip8() {}
Chip8::~Chip8() {}

// Initialize registers and memory
void Chip8::init() {
    PC = START_ADDRESS;     // Set program counter to 0x200
    I = 0;                  // Reset index register
    opcode = 0;             // Reset current opcode
    SP = 0;                 // Reset stack pointer
    set_draw_flag(0);       // Reset draw_flag
    
    // Clear registers V0 to VF
    for (int i=0; i<NUM_REGISTERS; ++i){
        V[i] = 0;
    }

    // Clear stack
    for (int i=0; i<STACK_SIZE; ++i){
        stack[i] = 0;
    }

    // Clear memory
    for (int i=0; i<MEMORY_SIZE; ++i){
        memory[i] = 0;
    }

    // Clear display
    for (int i=0; i<DISPLAY_WIDTH; ++i){
        for (int j=0; j<DISPLAY_HEIGHT; ++j){
            display[i][j] = 0;
        }
    }

    // Load fontset into memory
    for (int i=0; i<FONTSET_SIZE; ++i){
        memory[i] = fontset[i];
    }

    // Reset timers
    delay_timer = 0;
    sound_timer = 0;

}

// Load ROM instructions into memory
bool Chip8::load_rom(const std::string &filename){
    init();

    std::ifstream rom(filename, std::ios::binary);
    if (rom.is_open()){
        // Read ROM instructions from file to buffer
        char tmp;
        std::vector<char> buffer;
        while (rom.good()){
            rom.read(&tmp, 4);      // Read 4-byte hexadecimal instruction
            buffer.push_back(tmp);
        }

        // Check that size of ROM file is not greater than allowed memory space
        if (buffer.size()>MEMORY_SIZE-START_ADDRESS){
            std::cerr << "ROM file is too big to be read into memory (max 4KB)" << std::endl;
            return false;
        }

        // Read ROM instructions from buffer to memory
        for (int i=0; i<buffer.size(); ++i){
            memory[START_ADDRESS + i] = buffer.at(i);
        }

        rom.close();
    } 
    else {
        std::cerr << "Unable to load ROM" << std::endl;
        return false;
    }

    return true;
}

// Emulate one cycle
void Chip8::emulate_cycle() {
    // Fetch opcode
    opcode = (memory[PC] << 8) | memory[PC + 1];    // Read 2-byte opcode that the program counter is currently pointing to
    PC += 2;                                        // Increment program counter to be ready to fetch next opcode

    // Extract digits from the opcode
    uint16_t instruction_type = (opcode & 0xF000) >> 12;    // Type of instruction
    uint16_t X = (opcode & 0x0F00) >> 8;                    // Used to look up one of the 16 registers (VX)
    uint16_t Y = (opcode & 0x00F0) >> 4;                    // Used to look up one of the 16 registers (VY)
    uint16_t N = opcode & 0x000F;                           // 4-bit number
    uint16_t NN = (opcode & 0x00FF);                        // 8-bit immediate number
    uint16_t NNN = (opcode & 0x0FFF);                       // 12-bit immediate memory address

    std::cout << instruction_type << std::endl;

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
                    set_draw_flag(1);
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
            std::cout << "Draw" << std::endl;
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

            set_draw_flag(1);

            break;
    }
}

void Chip8::set_draw_flag(bool value){
    draw_flag = value;
}

bool Chip8::get_draw_flag(){
    return draw_flag;
}

int Chip8::get_display_pixel(int x, int y){
    return display[x][y];
}