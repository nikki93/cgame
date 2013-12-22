#ifndef SPRITE_H_L45BNMPA
#define SPRITE_H_L45BNMPA

#include "entity.h"
#include "transform.h"

void sprite_add(Entity ent);
void sprite_remove(Entity ent);

void sprite_set_cell(Entity ent, Vec2 cell);
void sprite_set_size(Entity ent, Vec2 size);

void sprite_init();
void sprite_deinit();
void sprite_check_messages_all();
void sprite_update_all();
void sprite_draw_all();

#endif

