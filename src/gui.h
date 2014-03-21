#ifndef GUI_H
#define GUI_H

#include "entity.h"
#include "script_export.h"

SCRIPT(gui,

       void gui_text_add(Entity ent);
       void gui_text_remove(Entity ent);
       void gui_text_set_str(Entity ent, const char *str);

    )

void gui_init();
void gui_deinit();
void gui_update_all();
void gui_draw_all();

#endif
