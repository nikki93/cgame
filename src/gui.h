#ifndef GUI_H
#define GUI_H

#include "entity.h"
#include "color.h"
#include "script_export.h"

SCRIPT(gui,

       EXPORT void gui_add(Entity ent);
       EXPORT void gui_remove(Entity ent);
       EXPORT void gui_set_color(Entity ent, Color color);
       EXPORT Color gui_gui_color(Entity ent);

       EXPORT void gui_text_add(Entity ent);
       EXPORT void gui_text_remove(Entity ent);
       EXPORT void gui_text_set_str(Entity ent, const char *str);

    )

void gui_init();
void gui_deinit();
void gui_update_all();
void gui_draw_all();
void gui_save_all(Serializer *s);
void gui_load_all(Deserializer *s);

#endif
