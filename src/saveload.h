#ifndef SAVELOAD_H_8VFZRR65
#define SAVELOAD_H_8VFZRR65

#include <stdio.h>
#include <stdbool.h>

void scalar_save(const float *f, FILE *file);
void scalar_load(float *f, FILE *file);

void size_t_save(const size_t *s, FILE *file);
void size_t_load(size_t *s, FILE *file);
void uint_save(const unsigned int *u, FILE *file);
void uint_load(unsigned int *u, FILE *file);

void bool_save(const bool *b, FILE *file);
void bool_load(bool *b, FILE *file);

void string_save(const char **s, FILE *file);
void string_load(char **s, FILE *file); /* must free(*s) later */

#endif

