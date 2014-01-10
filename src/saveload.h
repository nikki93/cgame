#ifndef SAVELOAD_H_8VFZRR65
#define SAVELOAD_H_8VFZRR65

#include <stdio.h>
#include <stdbool.h>

typedef struct Serializer Serializer;
typedef struct Deserializer Deserializer;

Serializer *serializer_open_str();
Serializer *serializer_open_file(const char *filename);
const char *serializer_get_str(Serializer *s); /* invalid after _close() */
void serializer_close(Serializer *s);

Deserializer *deserializer_open_str(const char *str);
Deserializer *deserializer_open_file(const char *filename);
void deserializer_close(Deserializer *s);

void scalar_save(const float *f, Serializer *s);
void scalar_load(float *f, Deserializer *s);

void uint_save(const unsigned int *u, Serializer *s);
void uint_load(unsigned int *u, Deserializer *s);

void bool_save(const bool *b, Serializer *s);
void bool_load(bool *b, Deserializer *s);

void string_save(const char **c, Serializer *s);
void string_load(char **c, Deserializer *s); /* must free(*c) later */

#endif

