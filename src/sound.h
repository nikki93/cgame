#ifndef SOUND_H
#define SOUND_H

#include "entity.h"
#include "script_export.h"

SCRIPT(sound,

       EXPORT void sound_add(Entity ent);
       EXPORT void sound_remove(Entity ent);
       EXPORT bool sound_has(Entity ent);

       EXPORT void sound_set_path(Entity ent, const char *path);
       EXPORT const char *sound_get_path(Entity ent);

       EXPORT void sound_set_playing(Entity ent, bool playing);
       EXPORT bool sound_get_playing(Entity ent);

       EXPORT void sound_set_seek(Entity ent, int seek);
       EXPORT int sound_get_seek(Entity ent);

       EXPORT void sound_set_finish_destroy(Entity ent, bool finish_destroy);
       EXPORT bool sound_get_finish_destroy(Entity ent);

       EXPORT void sound_set_loop(Entity ent, bool loop);
       EXPORT bool sound_get_loop(Entity ent);

    )

void sound_init();
void sound_deinit();
void sound_update_all();
void sound_save_all(Store *s);
void sound_load_all(Store *s);

#endif
