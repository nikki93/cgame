#include "console.h"

#include <string.h>
#include <stdlib.h>

#include "text.h"

static const char *init_buf = "";
static char *buf;
static Text text;

static void _update_text()
{
    text_set_str(text, buf);
}

void console_puts(const char *s)
{
    unsigned int len;

    len = strlen(buf) + strlen(s) + 2; /* 2 for newline, null */
    buf = realloc(buf, len);
    strcat(buf, s);
    strcat(buf, "\n");

    _update_text();
}

void console_init()
{
    buf = malloc(strlen(init_buf) + 1);
    strcpy(buf, init_buf);

    text_add(vec2_zero, init_buf);
}
void console_deinit()
{
    free(buf);
}

