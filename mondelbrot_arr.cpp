#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <SDL2/SDL.h>


const int repeats_to_set = 50;
const int num_of_frames = 1000;
const double max_quadr_Range[4] = {100.0, 100.0, 100.0, 100.0};

const int x_size = 800;
const int y_size = 600;

void convert_arr(double dst[4], const int src[4])
{
    for (int i = 0; i < 4; i++) {dst[i] = (double)src[i];}
}

void add_arr(double result[4], const double sum2[4], const double sum1[4])
{
    for (int i = 0; i < 4; i++) {result[i] = sum1[i] + sum2[i];}
}

void add_arr_i(int result[4], const int sum2[4], const int sum1[4])
{
    for (int i = 0; i < 4; i++) {result[i] = sum1[i] + sum2[i];}
}

void mul_arr(double result[4], const double arr1[4], const double arr2[4])
{
    for (int i = 0; i < 4; i++) {result[i] = arr1[i] * arr2[i];}
}

void sub_arr(double result[4], const double arr1[4], const double arr2[4])
{
    for (int i = 0; i < 4; i++) {result[i] = arr1[i] - arr2[i];}
}

void div_arr(double result[4], const double arr1[4], const double arr2[4])
{
    for (int i = 0; i < 4; i++) {result[i] = arr1[i] / arr2[i];}
}


void cpy_arr(double dst[4], const double src[4])
{
    for (int i = 0; i < 4; i++) {dst[i] = src[i];}
}

void set_arr(double dst[4], const double value)
{
    for (int i = 0; i < 4; i++) {dst[i] = value;}
}

void arr_cmp(int result[4], const double *bigger, const double *smaller)
{
    for (int i = 0; i < 4; i++) 
    {
        if (bigger[i] > smaller[i])
        {
            result[i] = 1;
        }
        else
            result[i] = 0;
    }
}

void set_start_coord(double *start_coord, int *coord, int coord_center, double zoom)
{
    double tmp[4] = {};
    convert_arr(tmp, coord);
    cpy_arr(start_coord, tmp);
    set_arr(start_coord, (double)coord_center);
    sub_arr(start_coord, start_coord, tmp);
    double zoom_arr[4]={zoom, zoom, zoom, zoom};
    div_arr(start_coord, start_coord, zoom_arr);
}

void calculate_r(double *x_coord, double *y_coord, double *r_arr)
{
   double tmp[4] = {};
   cpy_arr(tmp, x_coord);
   mul_arr(tmp, tmp, tmp);
   cpy_arr(r_arr, tmp);

   cpy_arr(tmp, y_coord);
   mul_arr(tmp, tmp, tmp);
   add_arr(r_arr, r_arr, tmp);
}

void calculate_new_x_coord(double *result, double *x_coord, double *y_coord, double *x_coord_start)
{
    double tmp[4]={};
    mul_arr(tmp, x_coord, x_coord);
    cpy_arr(result, tmp);
    mul_arr(tmp, y_coord, y_coord);
    sub_arr(result, result, tmp);
    add_arr(result, result, x_coord_start);
}

void calculate_new_y_coord(double *result, double *x_coord, double *y_coord, double *y_coord_start)
{
    cpy_arr(result, x_coord);
    mul_arr(result, result, y_coord);
    double tmp[4] = {};
    set_arr(tmp, 2);
    mul_arr(result, result, tmp);
    add_arr(result, result, y_coord_start);
}


int contains_1_i(int *a)
{
    return (a[0]) | (a[1]) | (a[2]) | (a[3]);
}

void neg_arr_i(int result[4], const int a[4])
{
    for (int i = 0; i < 4; i++) {result[i] = !a[i];}
}

void mult_arr_i(int result[4], const int a[4], const int b[4])
{
    for (int i = 0; i < 4; i++) {result[i] = a[i] * b[i];}
}

void set_diff_arr_i(int result[4], const int v0, const int v1, const int v2, const int v3)
{
    result[0] = v0;
    result[1] = v1;
    result[2] = v2;
    result[3] = v3;
}

void set_arr_i(int dst[4], const int value)
{
    for(int i = 4; i < 4; i++) {dst[i] = value;}
}

void cpy_arr_i(int dst[4], const int src[4])
{
    for(int i = 0; i < 4; i++) {dst[i] = src[i];}
}

int calculate_pixel_repeats(int *x, int *y, int x_center, int y_center, double zoom, int repeats[4]) 
{
    double x_coord_start[4] = {};
    set_start_coord(x_coord_start, x, x_center, zoom);
    double y_coord_start[4] = {};
    set_start_coord(y_coord_start, y, y_center, zoom);
    double x_coord[4] = {};
    double y_coord[4] = {};
    cpy_arr(x_coord, x_coord_start);
    cpy_arr(y_coord, y_coord_start);

    double x_new[4] = {};
    double y_new[4] = {};

    double r_arr[4] = {};
    calculate_r(x_coord, y_coord, r_arr);


    int is_inside[4] = {1, 1, 1, 1};

    while (contains_1_i(is_inside))
    {
        calculate_new_x_coord(x_new, x_coord, y_coord, x_coord_start);
        calculate_new_y_coord(y_new, x_coord, y_coord, y_coord_start);
        cpy_arr(x_coord, x_new);
        cpy_arr(y_coord, y_new);

        calculate_r(x_coord, y_coord, r_arr);
        arr_cmp(is_inside, max_quadr_Range, r_arr);
        add_arr_i(repeats, repeats, is_inside);
    }

    neg_arr_i(is_inside, is_inside);
    mult_arr_i(repeats, repeats, is_inside);
    return 0;        
}

int main()
{
    int x_center = 0.0;
    int y_center = 0.0;
    double zoom = 20.0;
    int repeats[4] = {};
    int x[4] = {};
    int y[4] = {};

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

    int screen[x_size][y_size] = {};

    for(int i = 0; i < num_of_frames; i++)
    {
        for (int x_start = 0; x_start < x_size; x_start++)
        {
            set_arr_i(x, x_start);
            for (int y_start = 0; y_start < y_size; y_start+=4)
            {
                set_diff_arr_i(y, y_start, y_start+1, y_start+2, y_start+3);
                calculate_pixel_repeats(x, y, x_center, y_center, zoom, repeats);

                cpy_arr_i((int *)&screen[x_start][y_start], repeats);
            }
        }

        redraw_Screen(renderer, (int *)&screen, x_size, y_size);
    }  
}


void redraw_Screen(SDL_Renderer *renderer, int *screen, int x_size, int y_size)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (size_t x = 0; x < x_size; x++)
    {
        for (size_t y = 0; y < y_size; y++)
        {
            if (!screen[x + y * x_size])
                SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    
    SDL_RenderPresent(renderer);
}
