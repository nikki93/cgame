#ifndef SAVELOAD_H_8VFZRR65
#define SAVELOAD_H_8VFZRR65

#include <stdio.h>
#include <stdbool.h>

#include "script.h"

SCRIPT(saveload,

        typedef struct Serializer Serializer;
        typedef struct Deserializer Deserializer;

        EXPORT Serializer *serializer_open_str();
        EXPORT Serializer *serializer_open_file(const char *filename);
        EXPORT const char *serializer_get_str(Serializer *s);
        EXPORT void serializer_close(Serializer *s);

        EXPORT Deserializer *deserializer_open_str(const char *str);
        EXPORT Deserializer *deserializer_open_file(const char *filename);
        EXPORT void deserializer_close(Deserializer *s);

      )

void scalar_save(const float *f, Serializer *s);
void scalar_load(float *f, Deserializer *s);

void uint_save(const unsigned int *u, Serializer *s);
void uint_load(unsigned int *u, Deserializer *s);

void bool_save(const bool *b, Serializer *s);
void bool_load(bool *b, Deserializer *s);

void string_save(const char **c, Serializer *s);
void string_load(char **c, Deserializer *s); /* must free(*c) later */

#endif

