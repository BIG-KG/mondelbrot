#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


const int repeats_to_set = 50;
const double max_quadr_Range = 100.0;


int calculate_pixel_color(int x, int y, int x_center, int y_center, double zoom) 
{
    double x_coord_start = ((double)x - (double)x_center) / zoom;
    double y_coord_start = ((double)y - (double)y_center) / zoom;
    double x_coord = x_coord_start;
    double y_coord = y_coord_start;
    double r = x_coord * x_coord + y_coord * y_coord;

    int repeats = 0;

    while (r < max_quadr_Range && repeats < repeats_to_set)
    {
        x_coord = x_coord * x_coord - y_coord * y_coord + x_coord_start;
        y_coord = 2 * x_coord * y_coord + y_coord_start;
        r = x_coord * x_coord + y_coord * y_coord;
        repeats++;
    }

    if (repeats >= repeats_to_set)
    {
        return repeats;
    }

    return 0;        
}

int main()
{
    int x_size = 800;
    int y_size = 600;
    int x_center = 0.0;
    int y_center = 0.0;
    double zoom = 20.0;

    for (int x = 0; x < x_size; x++)
    {
        for (int y = 0; y < y_size; y++)
        {
            int pixel_color = calculate_pixel_color(x, y, x_center, y_center, zoom);
            printf("%d ", pixel_color);
        }
        printf("\n");
    }
    
}
