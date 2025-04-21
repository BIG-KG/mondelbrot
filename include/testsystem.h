#ifndef TESTSYSTEM_H
#define TESTSYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


float save_result(float*data, int size, char *file_name, float AVG_N, float AVG_T);
float print_minimal_quadr(float *times, int size, float AVG_N, float AVG_T);

#endif