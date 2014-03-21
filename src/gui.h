#ifndef GUI_H
#define GUI_H

#include "entity.h"
#include "color.h"
#include "script_export.h"

SCRIPT(gui,

       EXPORT void gui_text_add(Entity ent);
       EXPORT void gui_text_remove(Entity ent);
       EXPORT void gui_text_set_str(Entity ent, const char *str);
       EXPORT void gui_text_set_color(Entity ent, Color color);
       EXPORT Color gui_text_get_color(Entity ent);

    )

void gui_init();
void gui_deinit();
void gui_update_all();
void gui_draw_all();

#endif
