#ifndef GUI_H
#define GUI_H

#include "entity.h"
#include "color.h"
#include "script_export.h"
#include "vec2.h"

SCRIPT(gui,

       EXPORT void gui_add(Entity ent);
       EXPORT void gui_remove(Entity ent);
       EXPORT void gui_set_color(Entity ent, Color color);
       EXPORT Color gui_gui_color(Entity ent);

       EXPORT void gui_rect_add(Entity ent);
       EXPORT void gui_rect_remove(Entity ent);
       EXPORT void gui_rect_set_size(Entity ent, Vec2 size);
       EXPORT Vec2 gui_rect_get_size(Entity ent);

       EXPORT void gui_text_add(Entity ent);
       EXPORT void gui_text_remove(Entity ent);
       EXPORT void gui_text_set_str(Entity ent, const char *str);
       EXPORT const char *gui_text_get_str(Entity ent);

    )

void gui_init();
void gui_deinit();
void gui_update_all();
void gui_draw_all();
void gui_save_all(Serializer *s);
void gui_load_all(Deserializer *s);

#endif
