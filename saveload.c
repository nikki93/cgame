#include <saveload.h>

void scalar_save(float *f, FILE *file)
{
    fprintf(file, "%f\n", *f);
}
void scalar_load(float *f, FILE *file)
{
    fscanf(file, "%f\n", f);
}

void uint_save(unsigned int *u, FILE *file)
{
    fprintf(file, "%u\n", *u);
}
void uint_load(unsigned int *u, FILE *file)
{
    fscanf(file, "%u\n", u);
}

void bool_save(bool *b, FILE *file)
{
    fprintf(file, "%d\n", (int) *b);
}
void bool_load(bool *b, FILE *file)
{
    int i;
    fscanf(file, "%d\n", &i);
    *b = i ? true : false;
}

