#include "entity.h"

#include <assert.h>
#include <stdbool.h>

#include "saveload.h"
#include "entitymap.h"
#include "array.h"
#include "entitypool.h"

typedef struct DestroyEntry DestroyEntry;
struct DestroyEntry
{
    Entity ent;
    unsigned int pass;
};

Entity entity_nil = { 0 };
static unsigned int counter = 1;

typedef struct ExistsPoolElem ExistsPoolElem;
struct ExistsPoolElem
{
    EntityPoolElem pool_elem;
};

static EntityPool *exists_pool; /* all existing entities */

static EntityMap *destroyed_map; /* whether entity is destroyed */
static Array *destroyed; /* array of DestroyEntry for destroyed objects */

static EntityMap *unused_map; /* whether has entry in unused array */
static Array *unused; /* id put here after _remove(), can reuse */

static EntityMap *load_map; /* map of saved ids --> real ids */

typedef enum SaveFilter SaveFilter;
enum SaveFilter
{
    SF_SAVE,    /* save this Entity */
    SF_NO_SAVE, /* do not save this Entity */
    SF_UNSET,   /* filter wasn't set -- use default */
};
static EntityMap *save_filter_map;
static SaveFilter save_filter_default = SF_SAVE;

/*
 * life cycle
 * ----------
 *
 *     (1) doesn't exist (or removed):
 *         destroyed_map[ent] = false
 *         ent not in destroyed
 *
 *         on entity_create() go to exists
 *
 *     (2) exists:
 *         destroyed_map[ent] = false
 *         ent not in destroyed
 *
 *         on entity_destroy() go to destroyed
 *
 *     (3) destroyed:
 *         destroyed_map[ent] = true
 *         { ent, pass } in destroyed
 *
 *         each update if ++pass >= 2, _remove() (go to (1) "doesn't exist")
 */

/* ------------------------------------------------------------------------- */

static Entity _generate_id()
{
    Entity ent;

    /* compute new id */
    if (array_length(unused) > 0)
    {
        /* have an unused id -- claim it */
        ent = array_top_val(Entity, unused);
        entitymap_set(unused_map, ent, false);
        array_pop(unused);
    }
    else
        ent.id = counter++;
    assert(!entity_eq(ent, entity_nil));

    entitypool_add(exists_pool, ent);
    return ent;
}

Entity entity_create()
{
    Entity ent;

    ent = _generate_id();

    return ent;
}

/* actually remove an entity entirely */
static void _remove(Entity ent)
{
    /* move it from destroyed to unused list */
    entitymap_set(destroyed_map, ent, false);
    array_add_val(Entity, unused) = ent;
    entitymap_set(unused_map, ent, true);
}

void entity_destroy(Entity ent)
{
    if (!entitypool_get(exists_pool, ent))
        return;
    if (entitymap_get(unused_map, ent))
        return;
    if (entitymap_get(destroyed_map, ent))
        return; /* already noted */

    entitypool_remove(exists_pool, ent);

    /* mark it as destroyed but don't 'remove' it yet */
    entitymap_set(destroyed_map, ent, true);
    array_add_val(DestroyEntry, destroyed) = (DestroyEntry) { { ent.id }, 0 };
}

void entity_destroy_all()
{
    ExistsPoolElem *exists;
    entitypool_foreach(exists, exists_pool)
        entity_destroy(exists->pool_elem.ent);
}

bool entity_destroyed(Entity ent)
{
    return entitymap_get(destroyed_map, ent);
}

void entity_set_save_filter(Entity ent, bool filter)
{
    if (filter)
    {
        entitymap_set(save_filter_map, ent, SF_SAVE);
        save_filter_default = SF_NO_SAVE;
    }
    else
        entitymap_set(save_filter_map, ent, SF_NO_SAVE);
}
bool entity_get_save_filter(Entity ent)
{
    SaveFilter filter = entitymap_get(save_filter_map, ent);
    if (filter == SF_UNSET)
        filter = save_filter_default; /* not set, use default */
    return filter == SF_SAVE;
}
void entity_clear_save_filters()
{
    entitymap_clear(save_filter_map);
    save_filter_default = SF_SAVE;
}

/* ------------------------------------------------------------------------- */

void entity_init()
{
    exists_pool = entitypool_new(ExistsPoolElem);
    destroyed_map = entitymap_new(false);
    destroyed = array_new(DestroyEntry);
    unused_map = entitymap_new(false);
    unused = array_new(Entity);
    save_filter_map = entitymap_new(SF_UNSET);
}
void entity_deinit()
{
    entitymap_free(save_filter_map);
    array_free(unused);
    entitymap_free(unused_map);
    array_free(destroyed);
    entitymap_free(destroyed_map);
    entitypool_free(exists_pool);
}

void entity_update_all()
{
    unsigned int i;
    DestroyEntry *entry;

    /* check destroyeds -- first pass we let go, second pass we remove */
    for (i = 0; i < array_length(destroyed); )
    {
        entry = array_get(destroyed, i);
        if (entry->pass == 0)
        {
            ++entry->pass;
            ++i;
        }
        else
        {
            _remove(entry->ent);
            array_quick_remove(destroyed, i);
        }
    }
}

void entity_save(Entity *ent, Serializer *s)
{
    if (!entity_eq(*ent, entity_nil))
        assert("filtered-out entity referenced" && entity_get_save_filter(*ent));
    uint_save(&ent->id, s);
}
void entity_load(Entity *ent, Deserializer *s)
{
    Entity sav;

    uint_load(&sav.id, s);
    if (entity_eq(sav, entity_nil))
    {
        *ent = entity_nil;
        return;
    }

    /* build a map of saved id --> new id to allow merging */
    ent->id = entitymap_get(load_map, sav);
    if (entity_eq(*ent, entity_nil))
    {
        *ent = _generate_id(); /* new sav */
        entitymap_set(load_map, sav, ent->id);
    }
}

void entity_load_all_begin()
{
    load_map = entitymap_new(entity_nil.id);
}
void entity_load_all_end()
{
    entitymap_free(load_map);
    entity_clear_save_filters();
}

#undef entity_eq
bool entity_eq(Entity e, Entity f)
{
    return e.id == f.id;
}

void entity_save_all(Serializer *s)
{
    ExistsPoolElem *exists;
    entitypool_save_foreach(exists, exists_pool, s);
}
void entity_load_all(Deserializer *s)
{
    ExistsPoolElem *exists;
    entitypool_load_foreach(exists, exists_pool, s);
}
