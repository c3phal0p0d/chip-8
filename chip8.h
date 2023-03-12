#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define MEMORY_SIZE 4096
#define STACK_SIZE 16
#define NUM_REGISTERS 16

class Chip8 {
    public:
        Chip8();
        ~Chip8();

        void load_rom();    // Load ROM file
        void run_cycle();   // Run one cycle

    private:
        int display[DISPLAY_WIDTH][DISPLAY_HEIGHT];    // 64 x 32 pixel display

        uint8_t memory[MEMORY_SIZE];    // RAM
        uint16_t stack[STACK_SIZE];     // Stack
        uint16_t SP;                    // Stack pointer

        uint16_t PC;                // Program counter which points to current instruction in memory
        uint16_t I;                 // Index register which points to locations in memory
        uint16_t opcode;            // Current opcode
        uint8_t V[NUM_REGISTERS];   // General purpose variable registers

        uint8_t delay_timer;    // Delay timer
        uint8_t sound_timer;    // Sound timer
};

#endif