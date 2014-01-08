#include "saveload.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

/* *n will store number of characters read, needs to also come after ... */
static void _deserializer_scanf_(Deserializer *s, const char *fmt, int *n, ...)
{
    va_list ap;
    char *newfmt;

    newfmt = malloc(strlen(fmt) + strlen("%n") + 1);
    strcpy(newfmt, fmt);
    strcat(newfmt, "%n");

    va_start(ap, n);

    switch (s->type)
    {
        case SER_STRING:
            vsscanf(s->ptr, newfmt, ap);
            s->ptr += *n;
            break;

        case SER_FILE:
            vfscanf(s->file, fmt, ap);
            break;
    }

    va_end(ap);

    free(newfmt);
}
#define _deserializer_scanf(s, fmt, ...) \
    do \
    { \
        int n_read__; \
        _deserializer_scanf_(s, fmt, &n_read__, __VA_ARGS__, &n_read__); \
    } while (0)

Serializer *serializer_open_str()
{
    Serializer *s = malloc(sizeof(Serializer));
    s->type = SER_STRING;
    s->strbuf.buf = malloc(1);
    s->strbuf.pos = 0;
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
    assert(s->type == SER_STRING);
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

