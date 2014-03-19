#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec2.h"

void texture_load(const char *filename);
void texture_bind(const char *filename);
Vec2 texture_get_size(const char *filename); /* (width, height) */

void texture_init();
void texture_deinit();

#endif

