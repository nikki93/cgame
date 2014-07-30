#ifndef SOUND_H
#define SOUND_H

#include "entity.h"
#include "script_export.h"

SCRIPT(sound,

       EXPORT void sound_add(Entity ent);
       EXPORT void sound_remove(Entity ent);
       EXPORT bool sound_has(Entity ent);

       EXPORT void sound_add_sound(Entity ent, const char *name);
       EXPORT void sound_remove_sound(Entity ent, const char *name);

       EXPORT void sound_set_path(Entity ent, const char *name, const char *path);
       EXPORT const char *sound_get_path(Entity ent, const char *name);

       EXPORT void sound_play(Entity ent, const char *name);
       EXPORT void sound_stop(Entity ent, const char *name);
       EXPORT bool sound_playing(Entity ent, const char *name);

    )

void sound_init();
void sound_deinit();
void sound_update_all();

#endif
