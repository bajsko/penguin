//
//  main.c
//  chip8
//
//  Created by bajsko on 2015-04-03.
//  Copyright (c) 2015 bajsko. All rights reserved.
//

#include <stdio.h>
#include <SDL2/SDL.h>
#include "cpu.h"

void updateEvents();
void drawGame();
int quit = 0;
SDL_Window* window;
SDL_Renderer* renderer;
Chip8 *cpu;

int main(int argc, const char * argv[])
{
    cpu = loadRom("pong");
    
    if(cpu == 0)
        return 1;
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("SDL INIT Error: %s", SDL_GetError());
        return 1;
    }
    
    window = SDL_CreateWindow("Chip 8 emulator by bajsko", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, SDL_WINDOW_OPENGL);
    
    if(window == 0)
    {
        printf("SDL Window Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    
    if(renderer == 0)
    {
        SDL_DestroyWindow(window);
        printf("SDL Render Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    while(quit == 0)
    {
        emulate(cpu);
        updateTimers(cpu);
        updateEvents();
        
        if(cpu->drawFlag)
        {
            drawGame();
            cpu->drawFlag = FALSE;
        }
        
        SDL_Delay(50);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    dumpregisters(cpu);
    releaseCPU(cpu);
    
    return 0;
}

void updateEvents()
{
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            case SDL_QUIT:
                quit = 1;
                
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym){
                    case SDLK_0:
                        cpu->keyboard[0] = 1;
                        break;
                    case SDLK_1:
                        cpu->keyboard[1] = 1;
                        break;
                    case SDLK_2:
                        cpu->keyboard[2] = 1;
                        break;
                    case SDLK_3:
                        cpu->keyboard[3] = 1;
                        break;
                    case SDLK_4:
                        cpu->keyboard[4] = 1;
                        break;
                    case SDLK_5:
                        cpu->keyboard[5] = 1;
                        break;
                    case SDLK_6:
                        cpu->keyboard[6] = 1;
                        break;
                    case SDLK_7:
                        cpu->keyboard[7] = 1;
                        break;
                    case SDLK_8:
                        cpu->keyboard[8] = 1;
                        break;
                    case SDLK_9:
                        cpu->keyboard[9] = 1;
                        break;
                    case SDLK_a:
                        cpu->keyboard[10] = 1;
                        break;
                    case SDLK_b:
                        cpu->keyboard[11] = 1;
                        break;
                    case SDLK_c:
                        cpu->keyboard[12] = 1;
                        break;
                    case SDLK_d:
                        cpu->keyboard[13] = 1;
                        break;
                    case SDLK_e:
                        cpu->keyboard[14] = 1;
                        break;
                    case SDLK_f:
                        cpu->keyboard[15] = 1;
                        break;
                    default:
                        break;
                }
                break;
                //On key release, set relevant array spots to 0
            case SDL_KEYUP:
                switch(e.key.keysym.sym){
                    case SDLK_0:
                        cpu->keyboard[0] = 0;
                        break;
                    case SDLK_1:
                        cpu->keyboard[1] = 0;
                        break;
                    case SDLK_2:
                        cpu->keyboard[2] = 0;
                        break;
                    case SDLK_3:
                        cpu->keyboard[3] = 0;
                        break;
                    case SDLK_4:
                        cpu->keyboard[4] = 0;
                        break;
                    case SDLK_5:
                        cpu->keyboard[5] = 0;
                        break;
                    case SDLK_6:
                        cpu->keyboard[6] = 0;
                        break;
                    case SDLK_7:
                        cpu->keyboard[7] = 0;
                        break;
                    case SDLK_8:
                        cpu->keyboard[8] = 0;
                        break;
                    case SDLK_9:
                        cpu->keyboard[9] = 0;
                        break;
                    case SDLK_a:
                        cpu->keyboard[10] = 0;
                        break;
                    case SDLK_b:
                        cpu->keyboard[11] = 0;
                        break;
                    case SDLK_c:
                        cpu->keyboard[12] = 0;
                        break;
                    case SDLK_d:
                        cpu->keyboard[13] = 0;
                        break;
                    case SDLK_e:
                        cpu->keyboard[14] = 0;
                        break;
                    case SDLK_f:
                        cpu->keyboard[15] = 0;
                        break;
                    default:
                        break;
                }
                break;
        }
    }
}

void drawGame()
{
    uint8_t pixelData = 0;
    SDL_Rect pixel = {0, 0, 10, 10};
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    
    for(int y = 0; y < 32; y++)
    {
        for(int x = 0; x < 64; x++)
        {
            pixelData = cpu->gfx[x + y * 64];
            
            if(pixelData != 0)
            {
                pixel.x = x * 10;
                pixel.y = y * 10;
                
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
    
    SDL_RenderPresent(renderer);
}

