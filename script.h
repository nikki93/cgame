#ifndef SCRIPT_H_GYSDQCZ0
#define SCRIPT_H_GYSDQCZ0

#include <stdio.h>

void script_init_all();
void script_deinit_all();
void script_update_all(float dt);
void script_draw_all();
void script_load_all(FILE *f);
void script_save_all(FILE *f);

#endif

