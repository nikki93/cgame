#ifndef TEXT_H_RZDC6BGS
#define TEXT_H_RZDC6BGS

#include "vec2.h"

typedef unsigned int Text;

Text text_add(Vec2 pos, const char *str); /* pos is pixels from bottom left */
void text_remove(Text text);
void text_set_pos(Text text, Vec2 pos);
void text_set_str(Text text, const char *str);

void text_init();
void text_deinit();
void text_draw_all();

#endif

