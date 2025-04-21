#ifndef DRAWING_H_INCLUDED
#define DRAWING_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

enum NEED_DRAWING{
    NO_DRAW_MODE,
    DRAW_MODE
};

SDL_Renderer *initialize_screen(int32_t x_size, int32_t y_size);
void          redraw_Screen(SDL_Renderer *renderer, int *screen, int x_size, int y_size);

#endif // DRAWING_H_INCLUDED