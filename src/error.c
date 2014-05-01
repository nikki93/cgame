#include "error.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "script.h"

static void _error(const char *s)
{
    script_error(s);
}

void errorf(const char *fmt, ...)
{
    va_list ap1, ap2;
    unsigned int n;
    char *s;

    va_start(ap1, fmt);
    va_copy(ap2, ap1);

    /* how much space do we need? */
    n = vsnprintf(NULL, 0, fmt, ap2);
    va_end(ap2);

    /* allocate, sprintf, print */
    s = malloc(n + 1);
    vsprintf(s, fmt, ap1);
    va_end(ap1);
    _error(s);
    free(s);
}
