#include "entity.h"

#include <assert.h>
#include <stdlib.h>

#include "saveload.h"
#include "entitymap.h"
#include "array.h"

static int dummy;
static void *dummy_ptr = &dummy; /* put in EntityMap to differentiate from
                                    NULL */

static EntityMap *emap;

static EntityMap *destroyed_map; /* whether entity is destroyed */
typedef struct DestroyEntry DestroyEntry;
struct DestroyEntry
{
    Entity ent;
    unsigned int pass;
};
static Array *destroyed; /* destroyed objects that we will _remove() soon */
static Array *unused; /* id put here after _remove(), can reuse */

/*
 * life cycle
 * ----------
 *
 *     doesn't exist (or removed):
 *         emap[ent] = NULL
 *         destroyed_map[ent] = NULL
 *         ent not in destroyed
 *
 *         on entity_create() go to exists
 *
 *     exists:
 *         emap[ent] = dummy_ptr
 *         destroyed_map[ent] = NULL
 *         ent not in destroyed
 *
 *         on entity_destroy() go to destroyed
 *
 *     destroyed:
 *         emap[ent] = NULL
 *         destroyed_map[ent] = dummy_ptr
 *         { ent, pass } in destroyed
 *
 *         each update if ++pass >= 2 then _remove() (go back to doesn't exist)
 */

/* ------------------------------------------------------------------------- */

static inline bool _exists(Entity ent)
{
    return entitymap_get(emap, ent) == dummy_ptr;
}

Entity entity_create()
{
    Entity ent;
    static unsigned int top = 0;

    if (array_length(unused) > 0)
    {
        ent = array_top_val(Entity, unused);
        array_pop(unused);
    }
    else
        ent = top++;

    entitymap_set(emap, ent, dummy_ptr);

    printf("new id: %u\n", ent);
    return ent;
}

/* actually remove an entity entirely */
static inline void _remove(Entity ent)
{
    entitymap_set(emap, ent, NULL);
    entitymap_set(destroyed_map, ent, NULL);
    array_add_val(Entity, unused) = ent;
}

void entity_destroy(Entity ent)
{
    DestroyEntry *entry;

    if (!_exists(ent))
        return;
    if (entitymap_get(destroyed_map, ent) == dummy_ptr)
        return; /* already noted */

    entitymap_set(destroyed_map, ent, dummy_ptr);
    array_add_val(DestroyEntry, destroyed) = (DestroyEntry) { ent, 0 };
}

bool entity_destroyed(Entity ent)
{
    return entitymap_get(destroyed_map, ent) == dummy_ptr;
}

/* ------------------------------------------------------------------------- */

void entity_init()
{
    emap = entitymap_new(NULL);
    destroyed_map = entitymap_new(NULL);
    destroyed = array_new(DestroyEntry);
    unused = array_new(Entity);
}
void entity_deinit()
{
    array_free(unused);
    array_free(destroyed);
    entitymap_free(destroyed_map);
    entitymap_free(emap);
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

