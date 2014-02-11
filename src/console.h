#ifndef CONSOLE_H
#define CONSOLE_H

#include "script_export.h"

SCRIPT(console,

       EXPORT void console_puts(const char *s);
       EXPORT void console_printf(const char *fmt, ...);

    )

void console_init();
void console_deinit();

#endif
