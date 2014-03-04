#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdio.h>

#include "saveload.h"
#include "script_export.h"

SCRIPT(system,

       EXPORT void system_clear();

    )

void system_init();
void system_deinit();
void system_update_all();
void system_draw_all();
void system_load_all(Deserializer *f);
void system_save_all(Serializer *f);

#endif

