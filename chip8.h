#ifndef CHIP8_H
#define CHIP8_H

#include <stack>
#include <stdint.h>

class Chip8 {
    private:
        uint8_t memory[4096];   // RAM
        int display[64 * 32];    // 64 x 32 pixel display
        std::stack<uint16_t> f_stack[16];    // Stack
        uint16_t PC;    // Program counter which points to current instruction in memory
        uint16_t I;     // Index register which points to locations in memory
        uint8_t V[16];  // General purpose variable registers
        uint8_t delay_timer;    // Delay timer
        uint8_t sound_timer;    // Sound timer

    public:
        Chip8();
        ~Chip8();

        void load_rom();    // Load ROM file
        void run_cycle();   // Run one cycle
};

#endif