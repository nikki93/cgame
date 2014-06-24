#include "saveload.h"

#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "array.h"

#include "error.h"

/* node in a tree of sections */
typedef struct Node Node;
struct Node
{
    char *name;
    char *data;

    Node *parent;
    Node *child;
    Node *sibling;

    Node *iterchild; /* next child to visit when iterating through children,
                        only really used on deserialization */
};

/* growable string stream */
typedef struct Stream Stream;
struct Stream
{
    char *buf;
    size_t pos;
};

struct Serializer
{
    Node *curr;
    char *filename; /* string target if NULL */
    char *buf; /* result of last serializer_get_str(...) call */
};

struct Deserializer
{
    Node *curr;
};

/* ------------------------------------------------------------------------- */

static void _stream_printf(Stream *sm, const char *fmt, ...)
{
    va_list ap1, ap2;
    size_t new_pos;

    va_start(ap1, fmt);
    va_copy(ap2, ap1);

    new_pos = sm->pos + vsnprintf(NULL, 0, fmt, ap2);
    va_end(ap2);

    sm->buf = realloc(sm->buf, new_pos + 1);
    vsprintf(sm->buf + sm->pos, fmt, ap1);
    sm->pos = new_pos;
    va_end(ap1);
}

static void _write_string(const char *s, unsigned int d, Stream *sm)
{
    if (s)
        _stream_printf(sm, "%*s%d %s", d, "", (int) strlen(s), s);
    else
        _stream_printf(sm, "%*s-1", d, "");
}

static const char *_read_string(char **dest, const char *s)
{
    int l, r;

    if (sscanf(s, "%d %n", &l, &r) != 1)
        error("corrupt save");
    s += r;

    if (l >= 0)
    {
        *dest = malloc(l + 1);
        strncpy(*dest, s, l);
        (*dest)[l] = '\0';
        s += l;
    }
    else
        *dest = NULL;

    return s;
}

/* name, data unset */
static Node *_node_new(Node *parent)
{
    Node *n = malloc(sizeof(Node));
    n->parent = parent;
    n->child = NULL;
    n->sibling = n->parent ? n->parent->child : NULL;
    if (n->parent)
        n->parent->iterchild = n->parent->child = n;
    n->iterchild = NULL;
    return n;
}

/* free subtree at node */
static void _node_free(Node *n)
{
    Node *m;

    while (n->child)
    {
        m = n->child->sibling;
        _node_free(n->child);
        n->child = m;
    }

    free(n->name);
    free(n->data);
}

static void _node_write(Node *n, unsigned int d, Stream *sm)
{
    Node *c;

    /* compress leaves to one line */

    _stream_printf(sm, "%*s{", d, "");
    if (n->child)
        _stream_printf(sm, "\n");

    _write_string(n->name, n->child ? d + 4 : 1, sm);
    _stream_printf(sm, " ", d);
    _write_string(n->data, 0, sm);
    if (n->child)
        _stream_printf(sm, "\n");

    for (c = n->child; c; c = c->sibling)
        _node_write(c, d + 4, sm);

    _stream_printf(sm, "%*s}\n", n->child ? d : 1, "");
}

static const char *_node_read(Node *parent, const char *s)
{
    Node *n = _node_new(parent);

    /* skip open brace */
    if (*s != '{')
        error("corrupt save");
    while (isspace(*++s));

    /* name, data */
    s = _read_string(&n->name, s);
    s = _read_string(&n->data, s);

    /* children */
    for (;;)
    {
        /* end? */
        while (isspace(*s))
            ++s;
        if (*s == '}')
        {
            ++s;
            break;
        }

        if (*s == '{')
            s = _node_read(n, s);
        else
            error("corrupt save");
    }

    return s;
}

/* ------------------------------------------------------------------------- */

Serializer *serializer_open_str()
{
    Serializer *s = malloc(sizeof(Serializer));
    s->curr = _node_new(NULL);
    s->curr->name = NULL;
    s->curr->data = NULL;
    s->buf = NULL;
    s->filename = NULL;
    return s;
}

Serializer *serializer_open_file(const char *filename)
{
    Serializer *s = serializer_open_str();
    s->filename = malloc(strlen(filename) + 1);
    strcpy(s->filename, filename);
    return s;
}


const char *serializer_get_str(Serializer *s)
{
    Stream sm[1];

    error_assert(!s->curr->parent, "must be at root section -- can't break out"
                 " of serializer_enter_section(...) blocks");

    sm->buf = NULL;
    sm->pos = 0;
    _stream_printf(sm, "");

    _node_write(s->curr, 0, sm);

    free(s->buf);
    s->buf = sm->buf;
    return s->buf;
}

void serializer_close(Serializer *s)
{
    FILE *f;
    const char *str;
    unsigned int l;

    /* write file? */
    if (s->filename)
    {
        f = fopen(s->filename, "w");
        if (!f)
            error("failed to open file '%s' for writing", s->filename);
        str = serializer_get_str(s);
        l = strlen(str);
        fprintf(f, "%u\n", l);
        fwrite(str, 1, l, f);
        fclose(f);
    }

    /* might not be at root if bad save */
    while (s->curr->parent)
        s->curr = s->curr->parent;
    _node_free(s->curr);

    free(s->buf);
    free(s->filename);
    free(s);
}

Deserializer *deserializer_open_str(const char *str)
{
    Deserializer *s;
    Node fake[1];

    _node_read(fake, str); /* root gets created as child of fake */
    fake->child->parent = NULL;

    s = malloc(sizeof(Deserializer));
    s->curr = fake->child;
    return s;
}

Deserializer *deserializer_open_file(const char *filename)
{
    char *str;
    FILE *f;
    unsigned int l;
    Deserializer *d;

    f = fopen(filename, "r");
    if (!f)
        error("failed to open file '%s' for reading", filename);

    fscanf(f, "%u\n", &l);
    str = malloc(l + 1);
    fread(str, 1, l, f);
    fclose(f);
    d = deserializer_open_str(str);
    free(str);

    return d;
}

void deserializer_close(Deserializer *s)
{
    /* might not be at root if bad load */
    while (s->curr->parent)
        s->curr = s->curr->parent;
    _node_free(s->curr);
    free(s);
}

/* ------------------------------------------------------------------------- */

void serializer_begin_section(const char *name, Serializer *s)
{
    Node *n = _node_new(s->curr);

    if (name)
    {
        n->name = malloc(strlen(name) + 1);
        strcpy(n->name, name);
    }
    else
        n->name = NULL;

    n->data = NULL;

    s->curr = n;
}

void serializer_end_section(Serializer *s)
{
    s->curr = s->curr->parent;
}

bool deserializer_begin_section(const char *name, Deserializer *s)
{
    Node *n;

    /* if NULL name pick next child to visit */
    if (!name)
    {
        if (s->curr->iterchild)
        {
            n = s->curr->iterchild;
            s->curr->iterchild = s->curr->iterchild->sibling;
            s->curr = n;
            return true;
        }
        return false;
    }

    /* check next child, quick if sections are in order */
    if (s->curr->iterchild && !strcmp(s->curr->iterchild->name, name))
    {
        n = s->curr->iterchild;
        s->curr->iterchild = s->curr->iterchild->sibling;
        s->curr = n;
        return true;
    }

    /* just search all children */
    for (n = s->curr->child; n; n = n->sibling)
        if (!strcmp(n->name, name))
        {
            s->curr = n;
            return true;
        }
    return false;
}

void deserialization_end_section(Deserializer *s)
{
    s->curr = s->curr->parent;
}

/* ------------------------------------------------------------------------- */

/* assumes only one write per section, use multiple sections for more */
static void _serializer_printf(Serializer *s, const char *fmt, ...)
{
    va_list ap1, ap2;
    unsigned int n;

    error_assert(!s->curr->data, "section '%s' shouldn't already have data",
                 s->curr->name);

    va_start(ap1, fmt);
    va_copy(ap2, ap1);

    /* how much space do we need? */
    n = vsnprintf(NULL, 0, fmt, ap2);
    va_end(ap2);

    /* allocate, sprintf */
    s->curr->data = malloc(n + 1);
    vsprintf(s->curr->data, fmt, ap1);
    va_end(ap1);
}

static int _deserializer_scanf(Deserializer *s, const char *fmt, ...)
{
    va_list ap;
    int r;

    error_assert(s->curr->data, "section '%s' should have data",
                 s->curr->name);

    va_start(ap, fmt);
    r = vsscanf(s->curr->data, fmt, ap);
    va_end(ap);
    return r;
}

void scalar_save(const Scalar *f, const char *n, Serializer *s)
{
    serializer_section(n, s)
    {
        if (*f == SCALAR_INFINITY)
            _serializer_printf(s, "inf");
        else
            _serializer_printf(s, "%f", *f);
    }
}
void scalar_load(Scalar *f, const char *n, Scalar d, Deserializer *s)
{
    deserializer_section(n, s)
    {
        if (s->curr->data[0] == 'i')
            *f = SCALAR_INFINITY;
        else
            _deserializer_scanf(s, "%f", f);
    }
    else
        *f = d;
}

void uint_save(const unsigned int *u, const char *n, Serializer *s)
{
    serializer_section(n, s)
        _serializer_printf(s, "%u", *u);
}
void uint_load(unsigned int *u, const char *n, unsigned int d, Deserializer *s)
{
    deserializer_section(n, s)
        _deserializer_scanf(s, "%u", u);
    else
        *u = d;
}

void int_save(const int *i, const char *n, Serializer *s)
{
    serializer_section(n, s)
        _serializer_printf(s, "%d", *i);
}
void int_load(int *i, const char *n, int d, Deserializer *s)
{
    deserializer_section(n, s)
        _deserializer_scanf(s, "%d", i);
    else
        *i = d;
}

void bool_save(const bool *b, const char *n, Serializer *s)
{
    serializer_section(n, s)
        _serializer_printf(s, "%d", *b == true);
}
void bool_load(bool *b, const char *n, bool d, Deserializer *s)
{
    int i = d;
    deserializer_section(n, s)
        _deserializer_scanf(s, "%d", &i);
    *b = i ? true : false;
}

void string_save(const char **c, const char *n, Serializer *s)
{
    serializer_section(n, s)
    {
        s->curr->data = malloc(strlen(*c) + 1);
        strcpy(s->curr->data, *c);
    }
}
void string_load(char **c, const char *n, const char *d, Deserializer *s)
{
    deserializer_section(n, s)
    {
        error_assert(s->curr->data, "section '%s' should have data",
                     s->curr->name);
        *c = malloc(strlen(s->curr->data) + 1);
        strcpy(*c, s->curr->data);
    }
    else
    {
        *c = malloc(strlen(d) + 1);
        strcpy(*c, d);
    }
}

#if 1

typedef enum TestEnum TestEnum;
enum TestEnum { A, B, C };

int main()
{
    Serializer *s;
    Deserializer *d;
    char *c;
    Scalar f;
    bool u;
    TestEnum e;

    s = serializer_open_file("test.sav");
    {
        serializer_section("sprite", s)
        {
            f = 3.14;
            scalar_save(&f, "glob1", s);

            serializer_section("stuff", s)
            {
                serializer_section(NULL, s)
                {
                    c = "spr1";
                    string_save(&c, "name", s);
                }

                serializer_section(NULL, s)
                {
                    c = "spr2";
                    string_save(&c, "name", s);
                }

                serializer_section(NULL, s)
                {
                    c = "spr3";
                    string_save(&c, "name", s);
                }
            }
        }

        serializer_section("transform", s);
    }
    serializer_close(s);

    d = deserializer_open_file("test.sav");
    {
        deserializer_section("sprite", d)
        {
            printf("%s\n", d->curr->name);

            scalar_load(&f, "glob1", 4.2, d);
            printf("    glob1: %f\n", f);

            scalar_load(&f, "glob3", 4.2, d);
            printf("    glob3: %f\n", f);

            enum_load(&e, "glob2", B, d);
            switch (e)
            {
                case A: printf("    glob2: A\n"); break;
                case B: printf("    glob2: B\n"); break;
                case C: printf("    glob2: C\n"); break;
            }

            deserializer_section("stuff", d)
                deserializer_section_loop(d)
                {
                    string_load(&c, "name", "noname", d);
                    printf("        name: %s\n", c);
                    free(c);
                }
        }
        else
        {
            printf("oops\n");
        }

        deserializer_section("transform", d)
            printf("%s\n", d->curr->name);
    }

    /* serializer_close(s); */
    /* deserializer_close(d); */

    return 0;
}

#endif
