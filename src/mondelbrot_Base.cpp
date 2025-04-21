#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "testsystem.h"
#include "mandelbrot.h"
#include "drawing.h"
#include "settingsMandelbrot.h"

#define MEASURE_TIME

float start_coord(int screen_coord, float zoom, float delta, int half_size)
{
    screen_coord -= half_size;
    float ret = (float)screen_coord;
    ret /= zoom;
    return ret;
}
                                                                                                                            
int calculate_pixel_color(int x, int y, float x_center, float y_center, float zoom, int32_t x_size, int32_t y_size) 
{
    float x_coord_start = start_coord(x, zoom, x_center, x_size/2);
    float y_coord_start = start_coord(y, zoom, y_center, y_size/2);
    float x_coord = x_coord_start;
    float y_coord = y_coord_start;
    float x_new   = x_coord;
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

void calculate_screen_BASE(int *screen, float x_center, float y_center, float zoom, int32_t x_size, 
                                                                                    int32_t y_size)
{
    for (int y = 0; y < y_size; y++)
    {
        for (int x = 0; x < x_size; x++)
        {
            screen[x + y * x_size] = calculate_pixel_color(x, y, x_center, y_center, zoom, x_size, y_size);
        }
    }
}
        