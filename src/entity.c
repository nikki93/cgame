#include "entity.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "saveload.h"
#include "entitymap.h"
#include "array.h"

typedef struct DestroyEntry DestroyEntry;
struct DestroyEntry
{
    Entity ent;
    unsigned int pass;
};

Entity entity_nil = 0;
static unsigned int counter = 1;
static EntityMap *destroyed_map; /* whether entity is destroyed */
static Array *destroyed; /* array of DestroyEntry for destroyed objects */
static EntityMap *unused_map; /* whether has entry in unused array */
static Array *unused; /* id put here after _remove(), can reuse */
static EntityMap *load_map; /* map of saved ids --> real ids */

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

Entity entity_create()
{
    Entity ent;

    if (array_length(unused) > 0)
    {
        ent = array_top_val(Entity, unused);
        entitymap_set(unused_map, ent, false);
        array_pop(unused);
    }
    else
        ent = counter++;

    assert(ent != entity_nil);
    return ent;
}

/* actually remove an entity entirely */
static void _remove(Entity ent)
{
    entitymap_set(destroyed_map, ent, false);
    array_add_val(Entity, unused) = ent;
    entitymap_set(unused_map, ent, true);
}

void entity_destroy(Entity ent)
{
    if (entitymap_get(unused_map, ent))
        return;
    if (entitymap_get(destroyed_map, ent))
        return; /* already noted */

    entitymap_set(destroyed_map, ent, true);
    array_add_val(DestroyEntry, destroyed) = (DestroyEntry) { ent, 0 };
}

bool entity_destroyed(Entity ent)
{
    return entitymap_get(destroyed_map, ent);
}

/* ------------------------------------------------------------------------- */

void entity_init()
{
    destroyed_map = entitymap_new(false);
    destroyed = array_new(DestroyEntry);
    unused_map = entitymap_new(false);
    unused = array_new(Entity);
}
void entity_deinit()
{
    array_free(unused);
    entitymap_free(unused_map);
    array_free(destroyed);
    entitymap_free(destroyed_map);
}

void entity_clear()
{
    counter = 1;
    array_reset(destroyed, 0);
    entitymap_clear(destroyed_map);
    array_reset(unused, 0);
    entitymap_clear(unused_map);
}

void entity_update_all()
{
    DestroyEntry *entry;

    /* check destroyeds -- first pass we let go, second pass we remove */
    for (unsigned int i = 0; i < array_length(destroyed); )
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
    uint_save(ent, s);
}
void entity_load(Entity *ent, Deserializer *s)
{
    Entity sav;

    uint_load(&sav, s);
    if (sav == entity_nil)
    {
        *ent = entity_nil;
        return;
    }

    /* build a map of saved id --> new id to allow merging */
    *ent = entitymap_get(load_map, sav);
    if (*ent == entity_nil)
    {
        *ent = entity_create(); /* new sav */
        entitymap_set(load_map, sav, *ent);
    }
}

void entity_load_all_begin()
{
    load_map = entitymap_new(entity_nil);
}
void entity_load_all_end()
{
    entitymap_free(load_map);
}

