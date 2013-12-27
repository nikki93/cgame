#ifndef SAVELOAD_H_8VFZRR65
#define SAVELOAD_H_8VFZRR65

#include <stdio.h>
#include <stdbool.h>

void save_scalar(float *f, FILE *file);
void load_scalar(float *f, FILE *file);

void save_uint(unsigned int *u, FILE *file);
void load_uint(unsigned int *u, FILE *file);

void bool_save(bool *b, FILE *file);
void bool_load(bool *b, FILE *file);

#endif

