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

const __m256 mm256_2f = _mm256_set1_ps(2.0f);
const __m256 max_r = _mm256_set1_ps(max_quadr_Range);

__m256i calculate_pixel_repeats( __m256i x_screen, __m256i y_screen, __m256 x_center, __m256 y_center, __m256 zoom,
                                                                     __m256i x_half_size,
                                                                     __m256i y_half_size)
{
    

    const __m256 x_start = convert_coord(x_screen, zoom, x_center, x_half_size);
    const __m256 y_start = convert_coord(y_screen, zoom, y_center, y_half_size);

    __m256 x = x_start;
    __m256 y = y_start;
    __m256i repeats = _mm256_set1_epi32(1);
    __m256i is_inside = _mm256_set1_epi32(-1);

    for (int i = 0; i < repeats_to_set && !_mm256_testz_si256(is_inside, is_inside); ++i)
    {
        const __m256 x2 = _mm256_mul_ps(x, x);
        const __m256 y2 = _mm256_mul_ps(y, y);
        const __m256 r = _mm256_add_ps(x2, y2);
        
        is_inside = _mm256_castps_si256(_mm256_cmp_ps(r, max_r, _CMP_LT_OQ));
        
        const __m256 xy = _mm256_mul_ps(x, y);
        y = _mm256_add_ps(_mm256_mul_ps(xy, mm256_2f), y_start);
        x = _mm256_add_ps(_mm256_sub_ps(x2, y2), x_start);
        
        repeats = _mm256_sub_epi32(repeats, is_inside);
    }

    return _mm256_andnot_si256(is_inside, repeats);
}

void calculate_screen_SIMD(int *screen, float x_center, float y_center, float zoom, int32_t x_size, 
                                                                                    int32_t y_size)
{
    __m256i x;
    __m256i y;

    __m256  x_center_mm256 = _mm256_set1_ps(x_center);
    __m256  y_center_mm256 = _mm256_set1_ps(y_center);
    __m256i x_half_size    = _mm256_set1_epi32(x_size/2);
    __m256i y_half_size    = _mm256_set1_epi32(y_size/2);

    for (int y_start = 0; y_start < y_size; y_start++)
    {
        y = _mm256_set1_epi32(y_start);
        for (int x_start = 0; x_start < x_size; x_start+=8)
        {
            const __m256i x = _mm256_add_epi32(_mm256_set1_epi32(x_start), _mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0));
            _mm256_storeu_si256((__m256i*)(screen + x_start + y_start * x_size), calculate_pixel_repeats(x, y, x_center_mm256, y_center_mm256, _mm256_set1_ps(zoom), x_half_size, y_half_size));

        }
    }

    return;
}