#ifndef SAVELOAD_H
#define SAVELOAD_H

#include <stdbool.h>

#include "scalar.h"
#include "script_export.h"

SCRIPT(saveload,

       /* remember to *_close(...) when done to free resources! */

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

void serializer_begin_section(const char *name, Serializer *s);
void serializer_end_section(Serializer *s);
bool deserializer_begin_section(const char *name, Deserializer *s);
void deserialization_end_section(Deserializer *s);

#define serializer_section(n, s)                                \
    for (int __c = (serializer_begin_section(n, s), 2); --__c;  \
         serializer_end_section(s))

#define deserializer_section(n, s)                                      \
    if (deserializer_begin_section(n, s))                               \
        for (int __c = 2; --__c; deserialization_end_section(s))
#define deserializer_section_default else
#define deserializer_section_loop(s)                                    \
    for (; deserializer_begin_section(NULL, s); deserialization_end_section(s))

/*
 * can be used to simplify the save/load loop of a collection -- check
 * transform.c, sprite.c etc. for examples
 */
void loop_continue_save(Serializer *s);
void loop_end_save(Serializer *s);
bool loop_continue_load(Deserializer *s);

void scalar_save(const Scalar *f, Serializer *s);
void scalar_load(Scalar *f, Deserializer *s);

void uint_save(const unsigned int *u, Serializer *s);
void uint_load(unsigned int *u, Deserializer *s);

void int_save(const int *i, Serializer *s);
void int_load(int *i, Deserializer *s);

#define enum_save(val, s)                                               \
    do { int e__; e__ = *(val); int_save(&e__, (s)); } while (0)
#define enum_load(val, s)                                               \
    do { int e__; int_load(&e__, (s)); *(val) = e__; } while (0)

void bool_save(const bool *b, Serializer *s);
void bool_load(bool *b, Deserializer *s);

void string_save(const char **c, const char *n, Serializer *s);
void string_load(char **c, const char *n, const char *d, Deserializer *s);
    /* must free(*c) later, copy of d if not found */

#endif

