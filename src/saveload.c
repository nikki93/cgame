#include "saveload.h"

#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "error.h"

enum Type
{
    SER_STRING,
    SER_FILE
};

struct Serializer
{
    int type;

    union
    {
        FILE *file;
        struct
        {
            char *buf;
            size_t pos;
        } strbuf;
    };
};

struct Deserializer
{
    int type;

    union
    {
        FILE *file;
        const char *ptr;
    };
};

static void _serializer_printf(Serializer *s, const char *fmt, ...)
{
    va_list ap1, ap2;
    size_t new_pos;

    va_start(ap1, fmt);

    switch (s->type)
    {
        case SER_STRING:
            va_copy(ap2, ap1);

            new_pos = s->strbuf.pos + vsnprintf(NULL, 0, fmt, ap1);
            s->strbuf.buf = realloc(s->strbuf.buf, new_pos + 1);
            vsprintf(s->strbuf.buf + s->strbuf.pos, fmt, ap2);
            s->strbuf.pos = new_pos;

            va_end(ap2);
            break;

        case SER_FILE:
            vfprintf(s->file, fmt, ap1);
            break;
    }

    va_end(ap1);
}

/*
 * scanf is tricky because we need to move forward by number of
 * scanned characters for string stream deserializer -- *n will store
 * number of characters read, needs to also be put at end of
 * parameter list (see _deserializer_scanf(...) macro)
 */
static void _deserializer_scanf_(Deserializer *s, const char *fmt, int *n, ...)
{
    va_list ap;
    char *newfmt;


    va_start(ap, n);

    switch (s->type)
    {
        case SER_STRING:
            /* add %n at end of fmt to write number of characters to *n */
            newfmt = malloc(strlen(fmt) + strlen("%n") + 1);
            strcpy(newfmt, fmt);
            strcat(newfmt, "%n");

            vsscanf(s->ptr, newfmt, ap);
            s->ptr += *n;

            free(newfmt);
            break;

        case SER_FILE:
            vfscanf(s->file, fmt, ap);
            break;
    }

    va_end(ap);
}
#define _deserializer_scanf(s, fmt, ...)                                \
    do                                                                  \
    {                                                                   \
        int n_read__;                                                   \
        _deserializer_scanf_(s, fmt, &n_read__, ##__VA_ARGS__, &n_read__); \
    } while (0)

/* return next character in stream but don't move ahead */
static char _deserializer_peek(Deserializer *s)
{
    int c;

    switch (s->type)
    {
        case SER_STRING:
            c = *s->ptr;
            break;

        case SER_FILE:
            c = fgetc(s->file);
            ungetc(c, s->file);
            break;
    }

    return c;
}

Serializer *serializer_open_str()
{
    Serializer *s = malloc(sizeof(Serializer));
    s->type = SER_STRING;
    s->strbuf.pos = 0;
    s->strbuf.buf = malloc(1);
    s->strbuf.buf[s->strbuf.pos] = '\0'; /* empty to begin */
    return s;
}
Serializer *serializer_open_file(const char *filename)
{
    Serializer *s = malloc(sizeof(Serializer));
    s->type = SER_FILE;
    s->file = fopen(filename, "w");
    return s;
}
const char *serializer_get_str(Serializer *s)
{
    error_assert(s->type == SER_STRING);
    return s->strbuf.buf;
}
void serializer_close(Serializer *s)
{
    switch (s->type)
    {
        case SER_STRING:
            free(s->strbuf.buf);
            break;

        case SER_FILE:
            fclose(s->file);
            break;
    }

    free(s);
}

Deserializer *deserializer_open_str(const char *str)
{
    Deserializer *s = malloc(sizeof(Deserializer));
    s->type = SER_STRING;
    s->ptr = str;
    return s;
}
Deserializer *deserializer_open_file(const char *filename)
{
    Deserializer *s = malloc(sizeof(Deserializer));
    s->type = SER_FILE;
    s->file = fopen(filename, "r");
    return s;
}
void deserializer_close(Deserializer *s)
{
    if (s->type == SER_FILE)
        fclose(s->file);
    free(s);
}

void loop_continue_save(Serializer *s)
{
    static bool yes = true;
    bool_save(&yes, s);
}
void loop_end_save(Serializer *s)
{
    static bool no = false;
    bool_save(&no, s);
}
bool loop_continue_load(Deserializer *s)
{
    bool cont;
    bool_load(&cont, s);
    return cont;
}

/* printf/scanf for SCALAR_INFINITY doesn't work out on MSVC */
void scalar_save(const Scalar *f, Serializer *s)
{
    if (*f == SCALAR_INFINITY)
        _serializer_printf(s, "inf\n");
    else
        _serializer_printf(s, "%f\n", *f);
}
void scalar_load(Scalar *f, Deserializer *s)
{
    if (_deserializer_peek(s) == 'i')
    {
        *f = SCALAR_INFINITY;
        _deserializer_scanf(s, "inf\n");
    }
    else
        _deserializer_scanf(s, "%f\n", f);
}

void uint_save(const unsigned int *u, Serializer *s)
{
    _serializer_printf(s, "%u\n", *u);
}
void uint_load(unsigned int *u, Deserializer *s)
{
    _deserializer_scanf(s, "%u\n", u);
}

void int_save(const int *i, Serializer *s)
{
    _serializer_printf(s, "%d\n", *i);
}
void int_load(int *i, Deserializer *s)
{
    _deserializer_scanf(s, "%d\n", i);
}

void bool_save(const bool *b, Serializer *s)
{
    _serializer_printf(s, "%d\n", *b ? 1 : 0);
}
void bool_load(bool *b, Deserializer *s)
{
    int i;
    _deserializer_scanf(s, "%d\n", &i);
    *b = i ? true : false;
}

void string_save(const char **c, Serializer *s)
{
    unsigned int len;

    len = strlen(*c);
    uint_save(&len, s);

    switch (s->type)
    {
        case SER_STRING:
            /* just concatenate the string */
            s->strbuf.buf = realloc(s->strbuf.buf, s->strbuf.pos + len + 1);
            strncpy(s->strbuf.buf + s->strbuf.pos, *c, len);
            s->strbuf.pos += len;
            s->strbuf.buf[s->strbuf.pos] = '\0';
            break;

        case SER_FILE:
            /* write the string as a buffer */
            fwrite(*c, sizeof(char), len, s->file);
            fflush(s->file);
            fprintf(s->file, "\n");
            break;
    }
}
void string_load(char **c, Deserializer *s)
{
    unsigned int len;

    uint_load(&len, s);
    *c = malloc(len + 1);

    switch (s->type)
    {
        case SER_STRING:
            /* just copy */
            strncpy(*c, s->ptr, len);
            s->ptr += len;
            break;

        case SER_FILE:
            /* read as a buffer */
            fread(*c, sizeof(char), len, s->file);
            fscanf(s->file, "\n");
            break;
    }
    (*c)[len] = '\0';
}

#ifdef SERIALIZER_TEST

void test_str()
{
    Serializer *s;
    Deserializer *d;

    s = serializer_open_str();

    printf("str: %s\n", serializer_get_str(s));

    _serializer_printf(s, "%d,", 3);
    _serializer_printf(s, " %s\n", "hello");

    printf("str: %s\n", serializer_get_str(s));

    d = deserializer_open_str(serializer_get_str(s));

    int n;
    _deserializer_scanf(d, "%d, ", &n);
    printf("des: %d\n", n);

    printf("des remain: '%s'\n", d->ptr);

    char str[512];
    _deserializer_scanf(d, "%s\n", &str);
    printf("des: %s\n", str);

    serializer_close(s);
    deserializer_close(d);
}

void test_file()
{
    Serializer *s;
    Deserializer *d;

    /* --- */

    s = serializer_open_file("tmp.sav");

    _serializer_printf(s, "%d,", 3);
    _serializer_printf(s, " %s\n", "hello");

    serializer_close(s);

    /* --- */

    d = deserializer_open_file("tmp.sav");

    int n;
    _deserializer_scanf(d, "%d, ", &n);
    printf("des: %d\n", n);

    char str[512];
    _deserializer_scanf(d, "%s\n", &str);
    printf("des: %s\n", str);

    deserializer_close(d);

    /* --- */
}

int main()
{
    test_str();
    printf("\n---\n\n");
    test_file();
    return 0;
}

#endif

