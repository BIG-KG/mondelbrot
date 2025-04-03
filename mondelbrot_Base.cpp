#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <SDL2/SDL.h>


const int x_size = 800;
const int y_size = 600;
const int repeats_to_set = 50;
const int num_of_frames = 1000;
const double max_quadr_Range = 100.0;



int calculate_pixel_color(int x, int y, int x_center, int y_center, double zoom) 
{
    double x_coord_start = ((double)x - (double)x_center) / zoom;
    double y_coord_start = ((double)y - (double)y_center) / zoom;
    double x_coord = x_coord_start;
    double y_coord = y_coord_start;
    double r = x_coord * x_coord + y_coord * y_coord;

    int repeats = 0;

    while (r < max_quadr_Range && repeats < repeats_to_set)
    {
        x_coord = x_coord * x_coord - y_coord * y_coord + x_coord_start;
        y_coord = 2 * x_coord * y_coord + y_coord_start;
        r = x_coord * x_coord + y_coord * y_coord;
        repeats++;
    }

    if (r < max_quadr_Range)
    {
        return 1;
    }

    return 0;        
}

int main()
{
    int x_center = 0.0;
    int y_center = 0.0;
    double zoom = 20.0;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error SDL: %s\n", SDL_GetError());
        return 1;
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

    int screen[x_size][y_size] = {0};

    for(int i = 0; i < num_of_frames; i++)
    {
        for (int x = 0; x < x_size; x++)
        {
            for (int y = 0; y < y_size; y++)
            {
                screen[x][y] = calculate_pixel_color(x, y, x_center, y_center, zoom);
            }
        }
        redraw_Screen(renderer, (int *)screen, x_size, y_size);
    }
}


void redraw_Screen(SDL_Renderer *renderer, int *screen, int x_size, int y_size)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (size_t x = 0; x < x_size; x++)
    {
        for (size_t y = 0; y < y_size; y++)
        {
            if (!screen[x + y * x_size])
                SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    
    SDL_RenderPresent(renderer);
}
        