#ifndef PREFAB_H
#define PREFAB_H

#include "entity.h"
#include "saveload.h"
#include "script_export.h"

SCRIPT(prefab,

       /* saves all filtered entites as a prefab, with 'root' as root */
       EXPORT void prefab_save(const char *filename, Entity root);

       /* loads a saved prefab, returns saved root entity */
       EXPORT Entity prefab_load(const char *filename);

    )

void prefab_save_all(Serializer *s);
void prefab_load_all(Deserializer *s);

#endif
