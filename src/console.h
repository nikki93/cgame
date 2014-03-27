#ifndef CONSOLE_H
#define CONSOLE_H

#include "entity.h"
#include "script_export.h"

SCRIPT(console,

       /*
        * print to this entity, adds to gui_text system if not already in it,
        * set to entity_nil to disable
        */
       EXPORT void console_set_entity(Entity ent);
       EXPORT Entity console_get_entity(); /* entity_nil if not set */

       EXPORT void console_puts(const char *s);
       EXPORT void console_printf(const char *fmt, ...);

    )

void console_init();
void console_deinit();

#endif
