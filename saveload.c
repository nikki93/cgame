#include <saveload.h>

void save_scalar(float *f, FILE *file)
{
    fprintf(file, "%f\n", *f);
}
void load_scalar(float *f, FILE *file)
{
    fscanf(file, "%f\n", f);
}

void save_uint(unsigned int *u, FILE *file)
{
    fprintf(file, "%u\n", *u);
}
void load_uint(unsigned int *u, FILE *file)
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

