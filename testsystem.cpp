#include <stdio.h>
#include <stdlib.h>

int save_result(float *data, int size, char *file_name)
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

    return 0;
}