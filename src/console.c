#include "console.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "text.h"
#include "input.h"

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
    char *buf, *c, *r;

    /* accumulate non-empty lines and set text string */

    buf = malloc(NUM_LINES * LINE_LEN);
    for (i = 1, c = buf; i <= NUM_LINES; ++i)
        for (r = lines[(top + i) % NUM_LINES]; *r; ++r)
            *c++ = *r;
    *c = '\0';

    text_set_str(text, buf);

    free(buf);
}

static void _print(const char *s)
{
    static unsigned int curs = 0; /* cursor position */

    /* copy to stdout */
    printf("%s", s);
    fflush(stdout);

    while (*s)
    {
        /* print any char, but prioritise newline space */
        if (curs < LINE_LEN - 2 || (*s == '\n' && curs < LINE_LEN - 1))
            lines[top][curs++] = *s;

        /* if newline, close this line and go to next */
        if (*s == '\n')
        {
            lines[top][curs] = '\0';

            top = (top + 1) % NUM_LINES;
            curs = 0;
        }

        ++s;
    }

    /* close this line */
    lines[top][curs] = '\0';

    _update_text();
}

void console_puts(const char *s)
{
    _print(s);
    _print("\n");
}

void console_printf(const char *fmt, ...)
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
    _print(s);
    free(s);
}

static void _keydown(KeyCode key)
{
    if (key == KC_ENTER && input_key_down(KC_LEFT_SHIFT))
        text_set_visible(text, !text_get_visible(text));
}

void console_init()
{
    unsigned int i;

    /* initially all empty */
    for (i = 0; i < NUM_LINES; ++i)
        lines[i][0] = '\0';
    text = text_add(vec2_zero, "");
    _update_text();

    /* listen to keys */
    input_add_key_down_callback(_keydown);

    /* print a nice message */
    console_puts("welcome to cgame!");
}
void console_deinit()
{
}

