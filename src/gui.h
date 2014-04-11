#ifndef GUI_H
#define GUI_H

#include "entity.h"
#include "color.h"
#include "script_export.h"
#include "vec2.h"
#include "input.h"

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
       EXPORT void gui_set_visible(Entity ent, bool visible);
       EXPORT bool gui_get_visible(Entity ent);
       EXPORT void gui_set_focusable(Entity ent, bool focusable);
       EXPORT bool gui_get_focusable(Entity ent);

       typedef enum GuiAlign GuiAlign;
       enum GuiAlign
       {
           GA_MIN,   /* h: left, v: bottom */
           GA_MID,   /* h: center, v: center */
           GA_MAX,   /* h: right, v: top */
           GA_TABLE, /* h: left-right table, v: top-down table */
           GA_NONE,  /* manual position */
       };

       EXPORT void gui_set_halign(Entity ent, GuiAlign align);
       EXPORT GuiAlign gui_get_halign(Entity ent);
       EXPORT void gui_set_valign(Entity ent, GuiAlign align);
       EXPORT GuiAlign gui_get_valign(Entity ent);
       EXPORT void gui_set_padding(Entity ent, Vec2 padding); /* h, v */
       EXPORT Vec2 gui_get_padding(Entity ent); /* h, v */

       /* entity_nil for no focus */
       EXPORT void gui_set_focused_entity(Entity ent);
       EXPORT Entity gui_get_focused_entity();
       EXPORT void gui_set_focus(Entity ent, bool focus);
       EXPORT bool gui_get_focus(Entity ent);
       EXPORT bool gui_has_focus(); /* whether any gui is focused */

       EXPORT bool gui_event_focus_enter(Entity ent);
       EXPORT bool gui_event_focus_exit(Entity ent);
       EXPORT bool gui_event_changed(Entity ent); /* input value changed */
       EXPORT MouseCode gui_event_mouse_down(Entity ent);
       EXPORT MouseCode gui_event_mouse_up(Entity ent);

       /* gui_rect */

       EXPORT void gui_rect_add(Entity ent);
       EXPORT void gui_rect_remove(Entity ent);
       EXPORT void gui_rect_set_size(Entity ent, Vec2 size);
       EXPORT Vec2 gui_rect_get_size(Entity ent);

       EXPORT void gui_rect_set_hfit(Entity ent, bool fit);
       EXPORT bool gui_rect_get_hfit(Entity ent);
       EXPORT void gui_rect_set_vfit(Entity ent, bool fit);
       EXPORT bool gui_rect_get_vfit(Entity ent);

       EXPORT void gui_rect_set_hfill(Entity ent, bool fill);
       EXPORT bool gui_rect_get_hfill(Entity ent);
       EXPORT void gui_rect_set_vfill(Entity ent, bool fill);
       EXPORT bool gui_rect_get_vfill(Entity ent);

       /* gui_text */

       EXPORT void gui_text_add(Entity ent);
       EXPORT void gui_text_remove(Entity ent);
       EXPORT void gui_text_set_str(Entity ent, const char *str);
       EXPORT const char *gui_text_get_str(Entity ent);
       EXPORT void gui_text_set_cursor(Entity ent, int cursor);

       EXPORT void gui_textedit_set_numerical(Entity ent, bool numerical);
       EXPORT bool gui_textedit_get_numerical(Entity ent);
       EXPORT Scalar gui_textedit_get_num(Entity ent); /* 0 if not numerical */

       /* gui_textedit */

       EXPORT void gui_textedit_add(Entity ent);
       EXPORT void gui_textedit_remove(Entity ent);

    )

void gui_event_clear();

void gui_init();
void gui_deinit();
void gui_update_all();
void gui_draw_all();
void gui_key_down(KeyCode key);
void gui_key_up(KeyCode key);
void gui_mouse_down(MouseCode mouse);
void gui_mouse_up(MouseCode mouse);
void gui_save_all(Serializer *s);
void gui_load_all(Deserializer *s);

#endif
