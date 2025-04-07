#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>


#include "testsystem.h"
#include "mondelbrot.h"


float save_result(float *data, int size, char *file_name, float AVG_N, float AVG_T)
{
    FILE *outputfile = fopen(file_name, "w");
    if(outputfile == NULL)
    {
        return -1;
    }
    
    for(int i = 0; i < size; i ++)
    {
        fprintf(outputfile, "%d %f\n", i + 1,data[i]);
    }
    
    fclose(outputfile);

    return print_minimal_quadr(data, size, AVG_N, AVG_T);;
}


float print_minimal_quadr(float *times, int size, float AVG_N, float AVG_T)
{
    float UpSum = 0;
    float DownSum = 0;

    for(int i = 1; i < size + 1; i++)
    {
        UpSum += (i - AVG_N) * (times[i - 1] - AVG_T);
        DownSum += pow((i - AVG_N), 2);
    }

    float T = UpSum / DownSum;

    printf("avg time to kadr %f\n", T);
    return T;
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

int main()
{
    float base = base_mondelbrot(0);
    float intr = intr_mondelbrot(0);

    printf("better at %f\n", base/intr);
}