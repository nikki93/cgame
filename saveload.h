#ifndef SAVELOAD_H_8VFZRR65
#define SAVELOAD_H_8VFZRR65

#include <stdio.h>
#include <stdbool.h>

void scalar_save(float *f, FILE *file);
void scalar_load(float *f, FILE *file);

void uint_save(unsigned int *u, FILE *file);
void uint_load(unsigned int *u, FILE *file);

void bool_save(bool *b, FILE *file);
void bool_load(bool *b, FILE *file);

#endif

