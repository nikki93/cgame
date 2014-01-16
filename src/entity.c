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

static unsigned int counter = 0;
static EntityMap *destroyed_map; /* whether entity is destroyed */
static Array *destroyed; /* array of DestroyEntry for destroyed objects */
static EntityMap *unused_map; /* whether has entry in unused array */
static Array *unused; /* id put here after _remove(), can reuse */

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
    uint_load(ent, s);
}

void entity_save_all(Serializer *s)
{
    unsigned int n, i;
    DestroyEntry *entry;

    uint_save(&counter, s);

    n = array_length(destroyed);
    uint_save(&n, s);
    for (i = 0; i < n; ++i)
    {
        entry = array_get(destroyed, i);
        entity_save(&entry->ent, s);
        uint_save(&entry->pass, s);
    }

    n = array_length(unused);
    uint_save(&n, s);
    for (i = 0; i < n; ++i)
        entity_save(array_get(unused, i), s);
}
void entity_load_all(Deserializer *s)
{
    unsigned int n, i;
    DestroyEntry *entry;
    Entity ent;

    uint_load(&counter, s);

    uint_load(&n, s);
    array_reset(destroyed, n);
    entitymap_clear(destroyed_map);
    for (i = 0; i < n; ++i)
    {
        entry = array_get(destroyed, i);
        entity_load(&entry->ent, s);
        uint_load(&entry->pass, s);

        entitymap_set(destroyed_map, entry->ent, true);
    }

    uint_load(&n, s);
    array_reset(unused, n);
    entitymap_clear(unused_map);
    for (i = 0; i < n; ++i)
    {
        entity_load(&ent, s);
        array_get_val(Entity, unused, i) = ent;
        entitymap_set(unused_map, ent, true);
    }
}

