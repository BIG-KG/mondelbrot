#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


float intr_mondelbrot(int mode);
float base_mondelbrot(int mode);
void  redraw_Screen(SDL_Renderer *renderer, int *screen, int x_size, int y_size);

void calculate_screen_SIMD(int *screen, float x_center, float y_center, float zoom, int32_t x_size, int32_t y_size); 
void calculate_screen_BASE(int *screen, float  x_center,float y_center, float zoom, int32_t x_size, int32_t y_size);


#endif // MANDELBROT_H