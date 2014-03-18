#ifndef EDIT_H
#define EDIT_H

#include <stdbool.h>

#include "entity.h"
#include "bbox.h"
#include "script_export.h"

SCRIPT(edit,

       void edit_set_enabled(bool e);
       bool edit_get_enabled();

       void edit_clear_bboxes();
       void edit_update_bbox(Entity ent, BBox bbox); /* merge bbox */

       void edit_select_clear();
       void edit_select_add(Entity ent);
       void edit_select_remove(Entity ent);
       bool edit_select_has(Entity ent);

       void edit_init();
       void edit_deinit();
       void edit_update_all();
       void edit_draw_all();

    )

#endif
