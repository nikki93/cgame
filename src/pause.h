#ifndef PAUSE_H_MDBVBEY2
#define PAUSE_H_MDBVBEY2

#include <stdbool.h>

#include "saveload.h"

SCRIPT(pause,

        EXPORT void pause_set(bool pause);
        EXPORT bool pause_get();

      )

void pause_save_all(Serializer *s);
void pause_load_all(Deserializer *s);

#endif

