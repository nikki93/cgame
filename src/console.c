#include "console.h"

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
    for (i = 0, c = buf; i < NUM_LINES; ++i)
        c = stpcpy(c, lines[(top + i) % NUM_LINES]);

    /* set Text string */
    text_set_str(text, buf);
}

void console_puts(const char *s)
{
    unsigned int i;

    for (;;)
    {
        /* copy, leaving space for newline and null */
        for (i = 0; i < LINE_LEN - 2 && *s && *s != '\n'; ++i)
            lines[top][i] = *s++;
        lines[top][i]     = '\n';
        lines[top][i + 1] = '\0';
        top = (top + 1) % NUM_LINES;

        /* skip to next line if exists, else stop */
        if ((s = strchr(s, '\n')))
            ++s;
        else
            break;
    }

    _update_text();
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

