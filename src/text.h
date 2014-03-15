#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>

#include "vec2.h"
#include "color.h"
#include "script_export.h"

SCRIPT(text,

       typedef unsigned int Text;

       EXPORT Text text_add(Vec2 pos, const char *str);
       EXPORT void text_remove(Text text);

       EXPORT void text_set_pos(Text text, Vec2 pos);

       EXPORT void text_set_str(Text text, const char *str);

       EXPORT void text_set_color(Text text, Color color);
       EXPORT Color text_get_color();

       EXPORT void text_set_visible(Text text, bool visible);
       EXPORT bool text_get_visible(Text text);

       EXPORT void text_init();
       EXPORT void text_deinit();
       EXPORT void text_draw_all();

    )

#endif

