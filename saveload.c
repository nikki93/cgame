#include "saveload.h"

#include <stdlib.h>
#include <string.h>

void scalar_save(const float *f, FILE *file)
{
    fprintf(file, "%f\n", *f);
}
void scalar_load(float *f, FILE *file)
{
    fscanf(file, "%f\n", f);
}

void size_t_save(const size_t *s, FILE *file)
{
    fprintf(file, "%zu\n", *s);
}
void size_t_load(size_t *s, FILE *file)
{
    fscanf(file, "%zu\n", s);
}
void uint_save(const unsigned int *u, FILE *file)
{
    fprintf(file, "%u\n", *u);
}
void uint_load(unsigned int *u, FILE *file)
{
    fscanf(file, "%u\n", u);
}

void bool_save(const bool *b, FILE *file)
{
    fprintf(file, "%d\n", *b ? 1 : 0);
}
void bool_load(bool *b, FILE *file)
{
    int i;
    fscanf(file, "%d\n", &i);
    *b = i ? true : false;
}

void string_save(const char **s, FILE *file)
{
    size_t len;

    len = strlen(*s);
    size_t_save(&len, file);

    fwrite(*s, 1, len, file);

    fprintf(file, "\n");
}
void string_load(char **s, FILE *file)
{
    size_t len;
    
    size_t_load(&len, file);

    *s = malloc(len + 1);
    fread(*s, 1, len, file);
    (*s)[len] = '\0';

    fscanf(file, "\n");
}

