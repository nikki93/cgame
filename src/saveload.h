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

       typedef struct Store Store;

       EXPORT Store *store_open(); /* empty to begin */
       EXPORT Store *store_open_str(const char *str);
       EXPORT const char *store_write_str(Store *s);
       EXPORT Store *store_open_file(const char *filename);
       EXPORT void store_write_file(Store *s, const char *filename);
       EXPORT void store_close(Store *s);

    )

/* store trees help with backwards-compatible save/load */
bool store_child_save(Store **sp, const char *name, Store *parent);
bool store_child_load(Store **sp, const char *name, Store *parent);

void scalar_save(const Scalar *f, const char *n, Store *s);
bool scalar_load(Scalar *f, const char *n, Scalar d, Store *s);

void uint_save(const unsigned int *u, const char *n, Store *s);
bool uint_load(unsigned int *u, const char *n, unsigned int d, Store *s);

void int_save(const int *i, const char *n, Store *s);
bool int_load(int *i, const char *n, int d, Store *s);

#define enum_save(val, n, s)                                            \
    do { int e__; e__ = *(val); int_save(&e__, n, (s)); } while (0)
#define enum_load(val, n, d, s)                                         \
    do { int e__; int_load(&e__, n, d, (s)); *(val) = e__; } while (0)

void bool_save(const bool *b, const char *n, Store *s);
bool bool_load(bool *b, const char *n, bool d, Store *s);

void string_save(const char **c, const char *n, Store *s);
bool string_load(char **c, const char *n, const char *d, Store *s);
    /* must free(*c) later, copy of d (possibly NULL) if not found */

#endif

