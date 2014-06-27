#include "prefab.h"

#include "system.h"

static Entity saved_root;

void prefab_save(const char *filename, Entity root)
{
    Store *s;

    saved_root = root;
    s = store_open();
    system_save_all(s);
    store_write_file(s, filename);
    store_close(s);
    saved_root = entity_nil;
}
Entity prefab_load(const char *filename)
{
    Store *s;
    Entity root;

    s = store_open_file(filename);
    system_load_all(s);
    store_close(s);
    root = saved_root;
    saved_root = entity_nil;

    return root;
}

void prefab_save_all(Store *s)
{
    Store *t;

    if (store_child_save(&t, "prefab", s))
        entity_save(&saved_root, "saved_root", t);
}
void prefab_load_all(Store *s)
{
    Store *t;

    if (store_child_load(&t, "prefab", s))
        entity_load(&saved_root, "saved_root", entity_nil, t);
}
