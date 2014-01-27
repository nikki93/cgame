#include "pause.h"

static bool paused = false;

void pause_set(bool pause)
{
    paused = pause;
}
bool pause_get()
{
    return paused;
}

void pause_save_all(Serializer *s)
{
    bool_save(&paused, s);
}
void pause_load_all(Deserializer *s)
{
    bool_load(&paused, s);
}

