#ifndef SAVELOAD_H
#define SAVELOAD_H

#include <stdbool.h>

#include "scalar.h"
#include "script_export.h"

/*
 * in all *_save() functions, 'n' is the name,
 *
 * in all *_load() functions, 'n' is the name, 'd' is the default
 * value, and the return value is whether found (i.e., not defaulted)
 */

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

/* sectioning for backwards-compatible serialization */

void serializer_begin_section(const char *name, Serializer *s);
void serializer_end_section(Serializer *s);
bool deserializer_begin_section(const char *name, Deserializer *s);
void deserialization_end_section(Deserializer *s);

bool deserializer_section_found(Deserializer *s);

#define serializer_section(n, s)                                \
    for (int __c = (serializer_begin_section(n, s), 2); --__c;  \
         serializer_end_section(s))
#define deserializer_section(n, s)                                      \
    if (deserializer_begin_section(n, s))                               \
        for (int __c = 2; --__c; deserialization_end_section(s))
#define deserializer_section_default else
#define deserializer_section_loop(s)                                    \
    for (; deserializer_begin_section(NULL, s); deserialization_end_section(s))

void scalar_save(const Scalar *f, const char *n, Serializer *s);
bool scalar_load(Scalar *f, const char *n, Scalar d, Deserializer *s);

void uint_save(const unsigned int *u, const char *n, Serializer *s);
bool uint_load(unsigned int *u, const char *n, unsigned int d, Deserializer *s);

void int_save(const int *i, const char *n, Serializer *s);
bool int_load(int *i, const char *n, int d, Deserializer *s);

#define enum_save(val, n, s)                                            \
    do { int e__; e__ = *(val); int_save(&e__, n, (s)); } while (0)
#define enum_load(val, n, d, s)                                         \
    do { int e__; int_load(&e__, n, d, (s)); *(val) = e__; } while (0)

void bool_save(const bool *b, const char *n, Serializer *s);
bool bool_load(bool *b, const char *n, bool d, Deserializer *s);

void string_save(const char **c, const char *n, Serializer *s);
bool string_load(char **c, const char *n, const char *d, Deserializer *s);
    /* must free(*c) later, copy of d if not found */

#endif

