#ifndef MONDELBROT_H
#define MONDELBROT_H

float intr_mondelbrot(int mode);
float base_mondelbrot(int mode);
void redraw_Screen(SDL_Renderer *renderer, int *screen, int x_size, int y_size);



#endif // MONDELBROT_H