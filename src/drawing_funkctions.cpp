#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>


#include "testsystem.h"
#include "mandelbrot.h"
#include "drawing.h"
#include "settingsMandelbrot.h"


SDL_Renderer *initialize_screen(int32_t x_size, int32_t y_size)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error SDL: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Mondelbrot array",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        x_size, y_size,                   
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,                           
        SDL_RENDERER_ACCELERATED      
    );

    return renderer;
}


void redraw_Screen(SDL_Renderer *renderer, int *screen, int x_size, int y_size)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (size_t y = 0; y < y_size; y++)
    {
        for (size_t x = 0; x < x_size; x++)
        {
            if (!screen[x + y * x_size])
                SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    
    SDL_RenderPresent(renderer);
}

