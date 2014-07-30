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

typedef struct SoundSource SoundSource;
struct SoundSource
{
    EntityPoolElem pool_elem;

    Array *sounds;
};

typedef struct Sound Sound;
struct Sound
{
    char *name;
    char *path;
    ga_Handle *handle;
};

typedef struct SoundCacheEntry SoundCacheEntry;
struct SoundCacheEntry
{
    char *path;
    ga_Sound *sound;
};

static EntityPool *pool;

static Array *cache_entries;

static gau_Manager *mgr;
static ga_Mixer *mixer;
static ga_StreamManager *stream_mgr;

void sound_add(Entity ent)
{
    SoundSource *source;

    if (entitypool_get(pool, ent))
        return;

    source = entitypool_add(pool, ent);
    source->sounds = array_new(Sound);
}

/* release resources, but don't remove from array */
static void _release_sound(Sound *sound)
{
    free(sound->name);
    free(sound->path);
    if (sound->handle)
        ga_handle_destroy(sound->handle);
}

/* release resources, but don't remove from pool */
static void _release_source(SoundSource *source)
{
    Sound *sound;

    array_foreach(sound, source->sounds)
        _release_sound(sound);
    array_free(source->sounds);
}

void sound_remove(Entity ent)
{
    SoundSource *source;

    source = entitypool_get(pool, ent);
    if (!source)
        return;

    _release_source(source);
    entitypool_remove(pool, ent);
}

bool sound_has(Entity ent)
{
    return entitypool_get(pool, ent) != NULL;
}

static Sound *_source_get_sound(SoundSource *source, const char *name)
{
    Sound *sound;
    array_foreach(sound, source->sounds)
        if (!strcmp(sound->name, name))
            return sound;
    error("no sound with name '%s' for this entity", name);
}

static SoundSource *_get_source(Entity ent)
{
    SoundSource *source = entitypool_get(pool, ent);
    error_assert(source, "entity must be in sound system");
    return source;
}

static Sound *_get_sound(Entity ent, const char *name)
{
    return _source_get_sound(_get_source(ent), name);
}

void sound_add_sound(Entity ent, const char *name)
{
    SoundSource *source;
    Sound *sound;

    source = _get_source(ent);
    sound = array_add(source->sounds);
    sound->name = malloc(strlen(name) + 1);
    strcpy(sound->name, name);
    sound->path = NULL;
    sound->handle = NULL;
}
void sound_remove_sound(Entity ent, const char *name)
{
    SoundSource *source;
    Sound *sound;

    source = _get_source(ent);
    sound = _source_get_sound(source, name);
    _release_sound(sound);
    array_quick_remove(source->sounds,
                       sound - (Sound *) array_begin(source->sounds));
}

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

void sound_set_path(Entity ent, const char *name, const char *path)
{
    Sound *sound = _get_sound(ent, name);

    free(sound->path);
    sound->path = malloc(strlen(path) + 1);
    strcpy(sound->path, path);

    if (sound->handle)
        ga_handle_destroy(sound->handle);
    sound->handle = gau_create_handle_buffered_file(
        mixer, stream_mgr, path, _format(path),
        NULL, NULL, NULL
        );
}
const char *sound_get_path(Entity ent, const char *name)
{
    return _get_sound(ent, name)->path;
}

void sound_play(Entity ent, const char *name)
{
    Sound *sound = _get_sound(ent, name);
    if (sound->handle)
        ga_handle_play(sound->handle);
}
void sound_stop(Entity ent, const char *name)
{
    Sound *sound = _get_sound(ent, name);
    if (sound->handle)
        ga_handle_stop(sound->handle);
}
bool sound_playing(Entity ent, const char *name)
{
    Sound *sound = _get_sound(ent, name);
    return sound->handle && ga_handle_playing(sound->handle);
}

void sound_init()
{
    gc_initialize(NULL);
    mgr = gau_manager_create();

    mixer = gau_manager_mixer(mgr);
    stream_mgr = gau_manager_streamManager(mgr);

    pool = entitypool_new(SoundSource);
}
void sound_deinit()
{
    SoundSource *source;

    /* entitypool_foreach(source, pool) */
    /*     _release_source(source); */
    entitypool_free(pool);

    gau_manager_destroy(mgr);
    gc_shutdown();
}

void sound_update_all()
{
    gau_manager_update(mgr);
}
