#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "testsystem.h"
#include "mondelbrot.h"

#define MEASURE_TIME

const int x_size = 1600;
const int y_size = 800;
const int repeats_to_set = 50;
const int num_of_frames = 100;
const float max_quadr_Range = 10.0;

const __m128i __m_true = _mm_set1_epi32(1);

__m128 convert_coord(__m128i screen_coord, __m128 zoom, __m128 delta, __m128i half_size)
{   
    screen_coord = _mm_sub_epi32(screen_coord, half_size);
    __m128 ret = _mm_cvtepi32_ps(screen_coord);
    ret = _mm_div_ps(ret, zoom);
    ret = _mm_add_ps(delta, ret);

    return ret;
}

bool _mm_or_inside(__m128i v)
{
    int tempA[4] = {};
    _mm_storeu_si128((__m128i*)tempA, v);

    return (tempA[0] || tempA[1] || tempA[2] || tempA[3]);
}


__m128 calculate_new_x_coord(__m128 x_coord, __m128 y_coord, __m128 x_coord_start)
{
    x_coord = _mm_mul_ps(x_coord, x_coord);
    y_coord = _mm_mul_ps(y_coord, y_coord);
    x_coord = _mm_sub_ps(x_coord, y_coord);
    return _mm_add_ps(x_coord, x_coord_start);
}

__m128 calculate_new_y_coord(__m128 x_coord, __m128 y_coord, __m128 y_coord_start)
{
    y_coord = _mm_mul_ps(x_coord, y_coord);
    y_coord = _mm_mul_ps(y_coord, _mm_set1_ps(2.0));
    return _mm_add_ps(y_coord, y_coord_start);
}


__m128i calculate_pixel_repeats( __m128i x_screen, __m128i y_screen, __m128 x_center, 
                                                                     __m128 y_center,
                                                                     __m128 zoom,
                                                                     __m128i x_half_size,
                                                                     __m128i y_half_size)
{
    

    __m128 x_start = convert_coord(x_screen, zoom, x_center, x_half_size);
    __m128 y_start = convert_coord(y_screen, zoom, y_center, y_half_size);

    //printf("x %f y %f\n", *((float *)&x_start), *(float *)&y_start);

    __m128 x = x_start;
    __m128 y = y_start;
    __m128 x_new;

    __m128i is_inside = _mm_set1_epi32(1);
    __m128i tmp;
    int max = 0;
    __m128i repeats = _mm_set1_epi32(1); 
    __m128 r;
    __m128 x2 = _mm_mul_ps(x, x);
    __m128 y2 = _mm_mul_ps(y, y);

    while (!_mm_testz_si128(is_inside, is_inside)  && max < repeats_to_set)
    {
        y = _mm_mul_ps(_mm_mul_ps(x, y), _mm_set1_ps(2));
        y = _mm_add_ps(y, y_start);
        x = _mm_add_ps(_mm_sub_ps(x2, y2), x_start);

        r = _mm_add_ps(x2, y2);;
        is_inside = _mm_and_si128(_mm_castps_si128(_mm_cmplt_ps(r, _mm_set1_ps(max_quadr_Range))), __m_true);
        repeats   = _mm_add_epi32(repeats, is_inside);

        x2 = _mm_mul_ps(x, x);
        y2 = _mm_mul_ps(y, y);

        max++;
    }

    __m128i mask = _mm_set1_epi32(-1);      
    mask = _mm_srli_epi32(mask, 31);
    is_inside = _mm_xor_si128(is_inside, mask);
    repeats   = _mm_mullo_epi32(is_inside, repeats);
    return repeats;        
}

float intr_mondelbrot(int mode)
{
    float x_center = 0;
    float y_center = 0;
    float zoom = 200.0;
    int repeats[4] = {};
    __m128i x;
    __m128i y;

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

    float *times = (float *)_mm_malloc(sizeof(float) * num_of_frames, 32);
    if (times == NULL)
    {
        printf("Error in memory alloc1\n");
        return 1;
    }
    clock_t start = clock();


    int *screen = (int *)_mm_malloc(sizeof(int) * x_size * y_size, 32);
    if (screen == NULL)
    {
        printf("Error in memory alloc2\n");
        return 1;
    }


    __m128 x_center_mm = _mm_set1_ps(x_center);
    __m128 y_center_mm = _mm_set1_ps(y_center);
    __m128i x_half_size = _mm_set1_epi32(x_size/2);
    __m128i y_half_size = _mm_set1_epi32(y_size/2);

    float AVG_N = (num_of_frames / 2) + 1;
    float AVG_T = 0;

    for(int i = 0; i < num_of_frames; i++)
    {
        for (int y_start = 0; y_start < y_size; y_start++)
        {
            y = _mm_set1_epi32(y_start);
            for (int x_start = 0; x_start < x_size; x_start+=4)
            {
                x = _mm_set_epi32(x_start + 3, x_start + 2, x_start + 1, x_start);
                _mm_storeu_si128((__m128i*)(screen + x_start + y_start * x_size), calculate_pixel_repeats(x, y, x_center_mm, y_center_mm, _mm_set1_ps(zoom), x_half_size, y_half_size));

            }
        }

        if(mode != 1)
        {
            clock_t current = clock();
            times[i] = (float)(current - start) / CLOCKS_PER_SEC;
            AVG_T += times[i]/(float)num_of_frames;
        }

        if(mode == 1)
        {
            printf("current frame %d\n", i);
            redraw_Screen(renderer, screen, x_size, y_size);
        }
    }  

    if(mode != 1)
    {
        save_result(times, num_of_frames, "./results/inctr_results.txt", AVG_N, AVG_T);
    }

    return 0;
}