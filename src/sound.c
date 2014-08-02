#include "sound.h"

#include <stdlib.h>
#include <string.h>
#include <gorilla/ga.h>
#include <gorilla/gau.h>

#include "dirs.h"
#include "input.h"
#include "entitypool.h"
#include "array.h"
#include "error.h"

typedef struct Sound Sound;
struct Sound
{
    EntityPoolElem pool_elem;

    char *path;
    ga_Handle *handle;
    bool finish_destroy;
};

static EntityPool *pool;

static gau_Manager *mgr;
static ga_Mixer *mixer;
static ga_StreamManager *stream_mgr;

static void _release(Sound *sound)
{
    /* path */
    free(sound->path);
    sound->path = NULL;

    /* handle */
    if (sound->handle)
        ga_handle_destroy(sound->handle);
    sound->handle = NULL;
}

/* figure out format from path -- uses extension */
static const char *_format(const char *path)
{
    const char *dot = NULL, *c;
    for (c = path; *c; ++c)
        if (*c == '.')
            dot = c;
    if (dot)
        return dot + 1;
    error("unknown sound format for file '%s'", path);
}

static void _set_path(Sound *sound, const char *path)
{
    const char *format;
    bool prev_playing;
    ga_Sound *src;

    prev_playing = sound->handle && ga_handle_playing(sound->handle);
    _release(sound);

    /* copy path */
    sound->path = malloc(strlen(path) + 1);
    strcpy(sound->path, path);

    /* create handle */
    format = _format(path);
    if (!strcmp(format, "ogg"))
    {
        sound->handle = gau_create_handle_buffered_file(
            mixer, stream_mgr, path, _format(path),
            NULL, NULL, NULL
            );
    }
    else
    {
        src = gau_load_sound_file(path, format);
        if (src)
            sound->handle = gau_create_handle_sound(
                mixer, src,
                NULL, NULL, NULL
                );
    }
    if (!sound->handle)
    {
        _release(sound);
        error("couldn't load sound from path '%s', check path and format",
              path);
    }

    if (prev_playing)
        ga_handle_play(sound->handle);
}

void sound_add(Entity ent)
{
    Sound *sound;

    if (entitypool_get(pool, ent))
        return;

    sound = entitypool_add(pool, ent);
    sound->path = NULL;
    sound->handle = NULL;
    sound->finish_destroy = true;
}

void sound_remove(Entity ent)
{
    Sound *sound;

    sound = entitypool_get(pool, ent);
    if (!sound)
        return;

    sound = entitypool_get(pool, ent);
    _release(sound);
    entitypool_remove(pool, ent);
}

bool sound_has(Entity ent)
{
    return entitypool_get(pool, ent) != NULL;
}

void sound_set_path(Entity ent, const char *path)
{
    Sound *sound;
    const char *format;
    bool prev_playing;
    ga_Sound *src;

    sound =  entitypool_get(pool, ent);
    error_assert(sound, "entity must be in sound system");

    /* check if currently playing, release */
    prev_playing = sound->handle && ga_handle_playing(sound->handle);
    _release(sound);

    /* copy path */
    sound->path = malloc(strlen(path) + 1);
    strcpy(sound->path, path);

    /* create handle */
    format = _format(path);
    if (!strcmp(format, "ogg"))
    {
        sound->handle = gau_create_handle_buffered_file(
            mixer, stream_mgr, path, _format(path),
            NULL, NULL, NULL
            );
    }
    else
    {
        src = gau_load_sound_file(path, format);
        if (src)
            sound->handle = gau_create_handle_sound(
                mixer, src,
                NULL, NULL, NULL
                );
    }
    if (!sound->handle)
    {
        _release(sound);
        error("couldn't load sound from path '%s', check path and format",
              path);
    }

    /* play new sound if old one was playing */
    if (prev_playing)
        ga_handle_play(sound->handle);
}
const char *sound_get_path(Entity ent)
{
    Sound *sound = entitypool_get(pool, ent);
    error_assert(sound, "entity must be in sound system");
    return sound->path;
}

void sound_set_playing(Entity ent, bool playing)
{
    Sound *sound = entitypool_get(pool, ent);
    error_assert(sound, "entity must be in sound system");
    if (playing)
        ga_handle_play(sound->handle);
    else
        ga_handle_stop(sound->handle);
}
bool sound_get_playing(Entity ent)
{
    Sound *sound = entitypool_get(pool, ent);
    error_assert(sound, "entity must be in sound system");
    return ga_handle_playing(sound->handle);
}

void sound_set_seek(Entity ent, int seek)
{
    Sound *sound = entitypool_get(pool, ent);
    error_assert(sound, "entity must be in sound system");
    ga_handle_seek(sound->handle, seek);
}
int sound_get_seek(Entity ent)
{
    Sound *sound = entitypool_get(pool, ent);
    error_assert(sound, "entity must be in sound system");
    return ga_handle_tell(sound->handle, GA_TELL_PARAM_CURRENT);
}

void sound_set_finish_destroy(Entity ent, bool finish_destroy)
{
    Sound *sound = entitypool_get(pool, ent);
    error_assert(sound, "entity must be in sound system");
    sound->finish_destroy = finish_destroy;
}
bool sound_get_finish_destroy(Entity ent)
{
    Sound *sound = entitypool_get(pool, ent);
    error_assert(sound, "entity must be in sound system");
    return sound->finish_destroy;
}

void sound_init()
{
    gc_initialize(NULL);
    mgr = gau_manager_create();

    mixer = gau_manager_mixer(mgr);
    stream_mgr = gau_manager_streamManager(mgr);

    pool = entitypool_new(Sound);
}
void sound_deinit()
{
    Sound *sound;

    entitypool_foreach(sound, pool)
        _release(sound);
    entitypool_free(pool);

    gau_manager_destroy(mgr);
    gc_shutdown();
}

void sound_update_all()
{
    Sound *sound;

    /* destroy finished sounds that have finish_destroy set */
    entitypool_foreach(sound, pool)
        if (sound->finish_destroy
            && sound->handle && ga_handle_finished(sound->handle))
            entity_destroy(sound->pool_elem.ent);

    entitypool_remove_destroyed(pool, sound_remove);

    gau_manager_update(mgr);
}
