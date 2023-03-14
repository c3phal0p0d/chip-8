#include <iostream>
#include <SDL.h>

#include "chip8.h"

int main(int argc, char *argv[]) {
    Chip8 chip8 = Chip8();

    std::cout << "CHIP-8" << std::endl;

    // Incorrect number of command line arguments
    if (argc != 2){
        std::cout << "Usage: chip8 <ROM filepath>" << std::endl;
        exit(1);
    }

    // Load ROM from provided filepath
    if (!chip8.load_rom(argv[1])){
        exit(1);
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING)){
        std::cerr << "Error iniitializing SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Set up SDL
    SDL_Window *window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000, SDL_WINDOW_SHOWN);
    if (window == nullptr){
        std::cerr << "Error creating SDL window " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr){
        std::cerr << "Error creating SDL renderer: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    if (texture == nullptr){
        std::cerr << "Error creating SDL texture: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_Event event;

    // Emulation loop
    while (!(event.type == SDL_QUIT)){
        SDL_PollEvent(&event);
        chip8.emulate_cycle();

        // Render display
        if (chip8.get_draw_flag()==1){
            // Read from 2d array into pixels buffer to be used for rendering
            int pixels[DISPLAY_WIDTH * DISPLAY_HEIGHT];
            for (int i=0; i<DISPLAY_WIDTH; ++i){
                for (int j=0; j<DISPLAY_HEIGHT; ++j){
                    pixels[i*DISPLAY_HEIGHT + j] = chip8.get_display_pixel(i, j);
                }
            }

            // Update SDL texture
            SDL_UpdateTexture(texture, NULL, pixels, 1);

            // Clear display & render updated version
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);

            chip8.set_draw_flag(0);
        }

        SDL_Delay(10);
    }

    return 0;
}