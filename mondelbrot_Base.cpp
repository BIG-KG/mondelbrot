#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "testsystem.h"

#define MEASURE_TIME

const int x_size = 1600;
const int y_size = 800;
const int repeats_to_set = 50;
const int num_of_frames = 100;
const float max_quadr_Range = 10.0;

void redraw_Screen(SDL_Renderer *renderer, int *screen, int x_size, int y_size);

float start_coord(int screen_coord, float zoom, float delta, int half_size)
{
    screen_coord -= half_size;
    float ret = (float)screen_coord;
    ret /= zoom;
    return ret;
}

int calculate_pixel_color(int x, int y, float x_center, float y_center, float zoom) 
{
    float x_coord_start = start_coord(x, zoom, x_center, x_size/2);
    float y_coord_start = start_coord(y, zoom, y_center, y_size/2);
    //printf("x%lf y%lf\n", x_coord_start, y_coord_start);
    float x_coord = x_coord_start;
    float y_coord = y_coord_start;
    float x_new = x_coord;
    float r = x_coord * x_coord + y_coord * y_coord;

    int repeats = 1;

    while (r < max_quadr_Range && repeats < repeats_to_set)
    {
        x_new = x_coord * x_coord - y_coord * y_coord + x_coord_start;
        y_coord = 2 * x_coord * y_coord + y_coord_start;
        x_coord = x_new;
        r = x_coord * x_coord + y_coord * y_coord;
        repeats++;
    }
    return repeats * !(r < max_quadr_Range);  
}

int main()
{
    float x_center = 0;
    float y_center = 0;
    float zoom = 200.0;

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

    #ifdef MEASURE_TIME
    float *times = (float *)calloc(sizeof(float), num_of_frames);
    clock_t start = clock();
    #endif

    int screen[x_size * y_size] = {0};

    for(int i = 0; i < num_of_frames; i++)
    {
        for (int y = 0; y < y_size; y++)
        {
            for (int x = 0; x < x_size; x++)
            {
                screen[x + y * x_size] = calculate_pixel_color(x, y, x_center, y_center, zoom);
            }
        }

        #ifdef MEASURE_TIME
        clock_t current = clock();
        times[i] = (float)(current - start) / CLOCKS_PER_SEC;
        #endif

        #ifndef MEASURE_TIME
        printf("current frame %d\n", i);
        redraw_Screen(renderer, (int *)&screen, x_size, y_size);
        #endif
    }

    #ifdef MEASURE_TIME
    save_result(times, num_of_frames, "./results/base_results.txt");
    #endif
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
        