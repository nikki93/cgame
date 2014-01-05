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

static EntityMap *exists_map;
static EntityMap *destroyed_map; /* whether entity is destroyed */
static Array *destroyed; /* array of DestroyEntry for destroyed objects */
static Array *unused; /* id put here after _remove(), can reuse */

/*
 * life cycle
 * ----------
 *
 *     doesn't exist (or removed):
 *         exists_map[ent] = false
 *         destroyed_map[ent] = false
 *         ent not in destroyed
 *
 *         on entity_create() go to exists
 *
 *     exists:
 *         exists_map[ent] = true
 *         destroyed_map[ent] = false
 *         ent not in destroyed
 *
 *         on entity_destroy() go to destroyed
 *
 *     destroyed:
 *         exists_map[ent] = false
 *         destroyed_map[ent] = true
 *         { ent, pass } in destroyed
 *
 *         each update if ++pass >= 2 then _remove() (go back to doesn't exist)
 */

/* ------------------------------------------------------------------------- */

Entity entity_create()
{
    Entity ent;
    static unsigned int counter = 0;

    if (array_length(unused) > 0)
    {
        ent = array_top_val(Entity, unused);
        array_pop(unused);
    }
    else
        ent = counter++;

    entitymap_set(exists_map, ent, true);
    printf("new id: %u\n", ent);
    return ent;
}

/* actually remove an entity entirely */
static inline void _remove(Entity ent)
{
    entitymap_set(exists_map, ent, false);
    entitymap_set(destroyed_map, ent, false);
    array_add_val(Entity, unused) = ent;
}

void entity_destroy(Entity ent)
{
    DestroyEntry *entry;

    if (!entitymap_get(exists_map, ent))
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
    exists_map = entitymap_new(false);
    destroyed_map = entitymap_new(false);
    destroyed = array_new(DestroyEntry);
    unused = array_new(Entity);
}
void entity_deinit()
{
    array_free(unused);
    array_free(destroyed);
    entitymap_free(destroyed_map);
    entitymap_free(exists_map);
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
            printf("%u first pass\n", entry->ent);
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

void entity_save(Entity *ent, FILE *file)
{
    uint_save(ent, file);
}
void entity_load(Entity *ent, FILE *file)
{
    uint_load(ent, file);
}

