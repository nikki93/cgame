#ifndef CONSOLE_H
#define CONSOLE_H

void console_puts(const char *s);
void console_printf(const char *fmt, ...);

void console_init();
void console_deinit();

#endif
