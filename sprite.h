#ifndef SPRITE_H_L45BNMPA
#define SPRITE_H_L45BNMPA

#include <stdio.h>

#include "entity.h"
#include "vec2.h"

/* script_begin */

void sprite_add(Entity ent);
void sprite_remove(Entity ent);

void sprite_set_cell(Entity ent, Vec2 cell);
void sprite_set_size(Entity ent, Vec2 size);

/* script_end */

void sprite_init();
void sprite_deinit();
void sprite_update_all();
void sprite_draw_all();
void sprite_save_all(FILE *file);
void sprite_load_all(FILE *file);

#endif

