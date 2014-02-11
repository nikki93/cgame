#include "console.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "text.h"

#define LINE_LEN 128 /* including newline, null char */
#define NUM_LINES 20

/* circular buffer of lines, 'top' is current top line */
static char lines[NUM_LINES][LINE_LEN];
static int top = 0;

/* Text that displays console contents */
static Text text;

static void _update_text()
{
    unsigned int i;
    char *buf, *c;

    /* accumulate non-empty lines */
    buf = malloc(NUM_LINES * LINE_LEN);
    for (i = 1, c = buf; i <= NUM_LINES; ++i)
        c = stpcpy(c, lines[(top + i) % NUM_LINES]);

    /* set Text string */
    text_set_str(text, buf);
}

static void _print(const char *s)
{
    static unsigned int curs = 0;

    while (*s)
    {
        /* print any char, but prioritise newline space */
        if (curs < LINE_LEN - 2 || (*s == '\n' && curs < LINE_LEN - 1))
        {
            lines[top][curs++] = *s;
            lines[top][curs] = '\0';
        }

        /* if newline go to next line */
        if (*s == '\n')
        {
            top = (top + 1) % NUM_LINES;
            curs = 0;
            lines[top][curs] = '\0';
        }

        ++s;
    }

    _update_text();
}

void console_puts(const char *s)
{
    _print(s);
    _print("\n");
}

void console_printf(const char *fmt, ...)
{
    va_list ap;
    char s[LINE_LEN];

    va_start(ap, fmt);
    vsnprintf(s, LINE_LEN, fmt, ap);
    va_end(ap);

    _print(s);
}

void console_init()
{
    unsigned int i;

    /* initially all empty */
    for (i = 0; i < NUM_LINES; ++i)
        lines[i][0] = '\0';
    text_add(vec2_zero, "");
    _update_text();

    /* print a nice message */
    console_puts("welcome to cgame!");
}
void console_deinit()
{
}

