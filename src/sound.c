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
    gau_SampleSourceLoop *loop_src;
    bool finish_destroy;
    bool loop;
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

/* update gorilla loop state to actually reflect sound->loop */
static void _update_loop(Sound *sound)
{
    if (sound->loop)
        gau_sample_source_loop_set(sound->loop_src, -1, 0);
    else
        gau_sample_source_loop_clear(sound->loop_src);
}

/* precondition: path must be good or NULL, handle must be good or NULL,
   doesn't allocate new path string if sound->path == path */
static void _set_path(Sound *sound, const char *path)
{
    bool prev_playing;
    const char *format;
    ga_Sound *src;
    ga_Handle *handle;
    gau_SampleSourceLoop *loop_src;

    /* currently playing? */
    prev_playing = sound->handle && ga_handle_playing(sound->handle);

    /* try loading sound */
    format = _format(path);
    handle = NULL;
    if (!strcmp(format, "ogg"))
        handle = gau_create_handle_buffered_file(mixer, stream_mgr, path,
                                                 format, NULL, NULL,
                                                 &loop_src);
    else if ((src = gau_load_sound_file(path, format)))
        handle = gau_create_handle_sound(mixer, src, NULL, NULL, &loop_src);
    if (!handle)
        error("couldn't load sound from path '%s', check path and format",
              path);
    error_assert(loop_src, "handle must have valid loop source");

    /* set new */
    _release(sound);
    if (sound->path != path)
    {
        sound->path = malloc(strlen(path) + 1);
        strcpy(sound->path, path);
    }
    sound->handle = handle;

    /* update loop */
    sound->loop_src = loop_src;
    _update_loop(sound);

    /* play new sound if old one was playing */
    if (prev_playing)
        ga_handle_play(sound->handle);
}

static const char *default_path = data_path("default.wav");

void sound_add(Entity ent)
{
    Sound *sound;

    if (entitypool_get(pool, ent))
        return;

    sound = entitypool_add(pool, ent);
    sound->path = NULL;
    sound->handle = NULL;
    sound->loop_src = NULL;
    sound->loop = false;
    sound->finish_destroy = true;

    _set_path(sound, default_path);
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

    sound =  entitypool_get(pool, ent);
    error_assert(sound, "entity must be in sound system");
    _set_path(sound, path);
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
    error_assert(sound->handle, "sound must be valid");
    if (playing)
    {
        if (ga_handle_finished(sound->handle))
            _set_path(sound, sound->path); /* can't reuse finished handles */
        ga_handle_play(sound->handle);
    }
    else
        ga_handle_stop(sound->handle);
}
bool sound_get_playing(Entity ent)
{
    Sound *sound = entitypool_get(pool, ent);
    error_assert(sound, "entity must be in sound system");
    error_assert(sound->handle, "sound must be valid");
    return ga_handle_playing(sound->handle);
}

void sound_set_seek(Entity ent, int seek)
{
    Sound *sound = entitypool_get(pool, ent);
    error_assert(sound, "entity must be in sound system");
    error_assert(sound->handle, "sound must be valid");
    ga_handle_seek(sound->handle, seek);
}
int sound_get_seek(Entity ent)
{
    Sound *sound = entitypool_get(pool, ent);
    error_assert(sound, "entity must be in sound system");
    error_assert(sound->handle, "sound must be valid");
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

void sound_set_loop(Entity ent, bool loop)
{
    Sound *sound = entitypool_get(pool, ent);
    error_assert(sound, "entity must be in sound system");
    error_assert(sound->handle, "sound must be valid");
    sound->loop = loop;
    _update_loop(sound);
}
bool sound_get_loop(Entity ent)
{
    Sound *sound = entitypool_get(pool, ent);
    error_assert(sound, "entity must be in sound system");
    return sound->loop;
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

void sound_save_all(Store *s)
{
    Store *t, *sound_s;
    Sound *sound;
    int seek;
    bool playing;

    if (store_child_save(&t, "sound", s))
        entitypool_save_foreach(sound, sound_s, pool, "pool", t)
        {
            string_save((const char **) &sound->path, "path", sound_s);
            bool_save(&sound->finish_destroy, "finish_destroy", sound_s);
            bool_save(&sound->loop, "loop", sound_s);

            playing = sound->handle && ga_handle_playing(sound->handle);
            bool_save(&playing, "playing", sound_s);

            if (sound->handle)
            {
                seek = ga_handle_tell(sound->handle, GA_TELL_PARAM_CURRENT);
                int_save(&seek, "seek", sound_s);
            }
        }
}
void sound_load_all(Store *s)
{
    Store *t, *sound_s;
    Sound *sound;
    int seek;
    bool playing;
    char *path;

    if (store_child_load(&t, "sound", s))
        entitypool_load_foreach(sound, sound_s, pool, "pool", t)
        {
            string_load(&path, "path", NULL, sound_s);
            bool_load(&sound->finish_destroy, "finish_destroy", false, sound_s);
            bool_load(&sound->loop, "loop", false, sound_s);

            sound->path = NULL;
            sound->handle = NULL;
            _set_path(sound, path);

            bool_load(&playing, "playing", false, sound_s);
            if (sound->handle && playing)
                ga_handle_play(sound->handle);

            if (sound->handle)
            {
                int_load(&seek, "seek", 0, sound_s);
                ga_handle_seek(sound->handle, seek);
            }
        }
}
