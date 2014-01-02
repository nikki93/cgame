#ifndef SYSTEM_H_JM82NXDP
#define SYSTEM_H_JM82NXDP

#include <stdio.h>

void system_init();
void system_deinit();
void system_update_all(float dt);
void system_draw_all();
void system_load_all(FILE *f);
void system_save_all(FILE *f);

#endif

