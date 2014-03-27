#ifndef GUI_H
#define GUI_H

#include "entity.h"
#include "color.h"
#include "script_export.h"
#include "vec2.h"

SCRIPT(gui,

       /*
        * get root entity of which all gui entites are descendants
        *
        * this entity's transform is set up so that all its children
        * have screen pixel coordinates and stay in the camera's view
        */
       EXPORT Entity gui_get_root();

       /* gui */

       EXPORT void gui_add(Entity ent);
       EXPORT void gui_remove(Entity ent);
       EXPORT void gui_set_color(Entity ent, Color color);
       EXPORT Color gui_get_color(Entity ent);

       typedef enum GuiAlign GuiAlign;
       enum GuiAlign
       {
           GA_MIN,   /* h: left, v: bottom */
           GA_MID,   /* h: center, v: center */
           GA_MAX,   /* h: right, v: top */
           GA_TABLE, /* h: ?, v: tabular */
           GA_NONE,  /* manual position */
       };

       EXPORT void gui_set_halign(Entity ent, GuiAlign align);
       EXPORT GuiAlign gui_get_halign(Entity ent);
       EXPORT void gui_set_valign(Entity ent, GuiAlign align);
       EXPORT GuiAlign gui_get_valign(Entity ent);
       EXPORT void gui_set_padding(Entity ent, Vec2 padding); /* h, v */
       EXPORT Vec2 gui_get_padding(Entity ent); /* h, v */

       /* gui_rect */

       EXPORT void gui_rect_add(Entity ent);
       EXPORT void gui_rect_remove(Entity ent);
       EXPORT void gui_rect_set_size(Entity ent, Vec2 size);
       EXPORT Vec2 gui_rect_get_size(Entity ent);

       /* gui_text */

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
