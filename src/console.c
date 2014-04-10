#include "console.h"

#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "vec2.h"
#include "game.h"
#include "input.h"
#include "gui.h"
#include "transform.h"

#define LINE_LEN 128 /* including newline, null char */
#define NUM_LINES 20

/* circular buffer of lines, 'top' is current top line */
static char lines[NUM_LINES][LINE_LEN] = { { 0 } };
static int top = 0;

/* text that displays console contents */
static Entity text;

static void _update_text()
{
    unsigned int i;
    char *buf, *c, *r;

    /* entity exists? */
    if (entity_eq(text, entity_nil))
        return;

    /* accumulate non-empty lines and set text string */

    buf = malloc(NUM_LINES * LINE_LEN);
    for (i = 1, c = buf; i <= NUM_LINES; ++i)
        for (r = lines[(top + i) % NUM_LINES]; *r; ++r)
            *c++ = *r;
    *c = '\0';

    gui_text_set_str(text, buf);

    free(buf);
}

void console_set_entity(Entity ent)
{
    text = ent;
    gui_text_add(text);
    _update_text();
}
Entity console_get_entity()
{
    return text;
}

void console_set_visible(bool visible)
{
    if (!entity_eq(text, entity_nil))
        gui_set_visible(text, visible);
}
bool console_get_visible()
{
    if (!entity_eq(text, entity_nil))
        return gui_get_visible(text);
    return false;
}

/* write a string to console with wrapping */
static void _write(const char *s)
{
    static unsigned int curs = 0; /* cursor position */
    static const char wrap_prefix[] = { 26, ' ', '\0' };
    unsigned int width;
    char c;
    bool wrap;

    /* wrap at window width, but max out at alloc'd size */
    width = game_get_window_size().x / 10;
    if (width > LINE_LEN - 2)
        width = LINE_LEN - 2;

    while (*s)
    {
        /* write char */
        wrap = curs >= width && *s != '\n';
        c = wrap ? '\n' : *s++;
        lines[top][curs++] = c;

        /* if newline, close this line and go to next */
        if (c == '\n')
        {
            lines[top][curs] = '\0';

            top = (top + 1) % NUM_LINES;
            curs = 0;
        }

        /* add a prefix to wrapped lines */
        if (wrap)
            _write(wrap_prefix);
    }

    /* close this line */
    lines[top][curs] = '\0';

    _update_text();
}

/* write a string to both stdout and the console */
static void _print(const char *s)
{
    /* copy to stdout */
    printf("%s", s);
    fflush(stdout);

    /* write it */
    _write(s);
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
    /* if (key == KC_ENTER && input_key_down(KC_LEFT_SHIFT)) */
    /*     text_set_visible(text, !text_get_visible(text)); */
}

void console_init()
{
    unsigned int i;

    text = entity_nil;

    /* listen to keys */
    input_add_key_down_callback(_keydown);

    /* print a nice message */
    console_puts("welcome to cgame!");
}
void console_deinit()
{
}

