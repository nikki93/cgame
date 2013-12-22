#include "entity.h"

#include <assert.h>

static unsigned int top = 0;

Entity entity_gen()
{
    assert(top < ENTITY_MAX);
    return top++;
}

