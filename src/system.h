#ifndef SYSTEM_H_JM82NXDP
#define SYSTEM_H_JM82NXDP

#include <saveload.h>
#include <stdio.h>

#include "scalar.h"

void system_init();
void system_deinit();
void system_update_all(Scalar dt);
void system_draw_all();
void system_load_all(Deserializer *f);
void system_save_all(Serializer *f);

#endif

