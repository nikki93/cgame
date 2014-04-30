#include "prefab.h"

#include "system.h"

static Entity saved_root;

void prefab_save(const char *filename, Entity root)
{
    Serializer *s;

    saved_root = root;
    s = serializer_open_file(filename);
    system_save_all(s);
    serializer_close(s);
    saved_root = entity_nil;
}
Entity prefab_load(const char *filename)
{
    Deserializer *d;
    Entity root;

    d = deserializer_open_file(filename);
    system_load_all(d);
    deserializer_close(d);
    root = saved_root;
    saved_root = entity_nil;

    return root;
}

void prefab_save_all(Serializer *s)
{
    entity_save(&saved_root, s);
}
void prefab_load_all(Deserializer *s)
{
    entity_load(&saved_root, s);
}
