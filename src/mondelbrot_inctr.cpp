#include <immintrin.h>
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

const __m256i __m_true = _mm256_set1_epi32(1);

__m256 convert_coord(__m256i screen_coord, __m256 zoom, __m256 delta, __m256i half_size)
{   
    screen_coord = _mm256_sub_epi32(screen_coord, half_size);
    __m256 ret = _mm256_cvtepi32_ps(screen_coord);
    ret = _mm256_div_ps(ret, zoom);
    ret = _mm256_add_ps(delta, ret);

    return ret;
}

bool _mm256_or_inside(__m256i v)
{
    int tempA[8] = {};
    _mm256_storeu_si256((__m256i*)tempA, v);

    return (tempA[0] || tempA[1] || tempA[2] || tempA[3]);
}


__m256 calculate_new_x_coord(__m256 x_coord, __m256 y_coord, __m256 x_coord_start)
{
    x_coord = _mm256_mul_ps(x_coord, x_coord);
    y_coord = _mm256_mul_ps(y_coord, y_coord);
    x_coord = _mm256_sub_ps(x_coord, y_coord);
    return _mm256_add_ps(x_coord, x_coord_start);
}

__m256 calculate_new_y_coord(__m256 x_coord, __m256 y_coord, __m256 y_coord_start)
{
    y_coord = _mm256_mul_ps(x_coord, y_coord);
    y_coord = _mm256_mul_ps(y_coord, _mm256_set1_ps(2.0));
    return _mm256_add_ps(y_coord, y_coord_start);
}


__m256i calculate_pixel_repeats( __m256i x_screen, __m256i y_screen, __m256 x_center, 
                                                                     __m256 y_center,
                                                                     __m256 zoom,
                                                                     __m256i x_half_size,
                                                                     __m256i y_half_size)
{
    

    __m256 x_start = convert_coord(x_screen, zoom, x_center, x_half_size);
    __m256 y_start = convert_coord(y_screen, zoom, y_center, y_half_size);

    //printf("x %f y %f\n", *((float *)&x_start), *(float *)&y_start);

    __m256 x = x_start;
    __m256 y = y_start;
    __m256 x_new;

    __m256i is_inside = _mm256_set1_epi32(1);
    __m256i tmp;
    int max = 0;
    __m256i repeats = _mm256_set1_epi32(1); 
    __m256 r;
    __m256 x2 = _mm256_mul_ps(x, x);
    __m256 y2 = _mm256_mul_ps(y, y);

    while (!_mm256_testz_si256(is_inside, is_inside)  && max < repeats_to_set)
    {
        y = _mm256_mul_ps(_mm256_mul_ps(x, y), _mm256_set1_ps(2));
        y = _mm256_add_ps(y, y_start);
        x = _mm256_add_ps(_mm256_sub_ps(x2, y2), x_start);

        r = _mm256_add_ps(x2, y2);;
        is_inside = _mm256_and_si256(_mm256_castps_si256(_mm256_cmp_ps(r, _mm256_set1_ps(max_quadr_Range), _CMP_LT_OQ)), __m_true);
        repeats   = _mm256_add_epi32(repeats, is_inside);

        x2 = _mm256_mul_ps(x, x);
        y2 = _mm256_mul_ps(y, y);

        max++;
    }

    __m256i mask = _mm256_set1_epi32(-1);      
    mask = _mm256_srli_epi32(mask, 31);
    is_inside = _mm256_xor_si256(is_inside, mask);
    repeats   = _mm256_mullo_epi32(is_inside, repeats);
    return repeats;        
}

void calculate_screen_SIMD(int *screen, float x_center, float y_center, float zoom, int32_t x_size, 
                                                                                    int32_t y_size)
{
    __m256i x;
    __m256i y;

    __m256  x_center_mm256 = _mm256_set1_ps(x_center);
    __m256  y_center_mm256 = _mm256_set1_ps(y_center);
    __m256i x_half_size = _mm256_set1_epi32(x_size/2);
    __m256i y_half_size = _mm256_set1_epi32(y_size/2);

    for (int y_start = 0; y_start < y_size; y_start++)
    {
        y = _mm256_set1_epi32(y_start);
        for (int x_start = 0; x_start < x_size; x_start+=8)
        {
            x = _mm256_set_epi32(x_start + 7, x_start + 6, x_start + 5, x_start + 4, x_start + 3, x_start + 2, x_start + 1, x_start);
            _mm256_storeu_si256((__m256i*)(screen + x_start + y_start * x_size), calculate_pixel_repeats(x, y, x_center_mm256, y_center_mm256, _mm256_set1_ps(zoom), x_half_size, y_half_size));

        }
    }

    return;
}