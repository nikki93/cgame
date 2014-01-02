#include "entity.h"

#include <assert.h>
#include <stdlib.h>

#include "saveload.h"

static unsigned int top = 0;

/* ------------------------------------------------------------------------- */

Entity entity_new()
{
    return top++;
}

void entity_save(Entity *ent, FILE *file)
{
    uint_save(ent, file);
}
void entity_load(Entity *ent, FILE *file)
{
    uint_load(ent, file);
}

