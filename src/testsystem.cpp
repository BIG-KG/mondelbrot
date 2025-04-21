#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>


#include "testsystem.h"
#include "mandelbrot.h"
#include "drawing.h"
#include "settingsMandelbrot.h"


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



float test_mondelbrot(NEED_DRAWING mode, int32_t num_frames, void (*calculate_screen_)(int *, float x_center, float y_center, float zoom, int32_t x_size, int32_t y_size))
{
    float x_center = 0;
    float y_center = 0;
    float zoom = 200.0;

    SDL_Renderer *renderer = NULL;
    
    if(mode == DRAW_MODE){
        renderer = initialize_screen(x_size, y_size);
    }

    float *times = (float *)_mm_malloc(sizeof(float) * num_frames, 32);
    if (times == NULL)
    {
        printf("Error in memory alloc1\n");
        return 1;
    }

    int *screen = (int *)_mm_malloc(sizeof(int) * x_size * y_size, 32);
    if (screen == NULL)
    {
        printf("Error in memory alloc2\n");
        return 1;
    }

    clock_t start = clock();

    for(int i = 0; i < num_frames; i++)
    {
        calculate_screen_(screen, x_center, y_center, zoom, x_size, y_size);

        if(mode == DRAW_MODE)
        {
            printf("current frame %d\n", i);
            redraw_Screen(renderer, screen, x_size, y_size);
        }

        printf("frame %d is completed\n", i);
    }  

    return (clock() - start) / (float)CLOCKS_PER_SEC;
}

int main()
{
    float base = test_mondelbrot(NO_DRAW_MODE, 1000, calculate_screen_BASE);
    float intr = test_mondelbrot(NO_DRAW_MODE, 1000, calculate_screen_SIMD);


    printf("better at %f\n", base/intr);
}