#ifndef EDIT_H
#define EDIT_H

#include <stdbool.h>

#include "entity.h"
#include "bbox.h"
#include "script_export.h"

SCRIPT(edit,

       EXPORT void edit_set_enabled(bool e);
       EXPORT bool edit_get_enabled();

       /* non-editable entities cannot be selected */
       EXPORT void edit_set_editable(Entity ent, bool editable);
       EXPORT bool edit_get_editable(Entity ent);

       EXPORT void edit_bboxes_clear();
       EXPORT void edit_bboxes_update(Entity ent, BBox bbox); /* merge bbox */
       EXPORT unsigned int edit_bboxes_get_num();
       typedef struct EntityBBoxPair EntityBBoxPair;
       struct EntityBBoxPair { Entity ent; BBox bbox; };
       EXPORT EntityBBoxPair edit_bboxes_get_nth(unsigned int n);
       EXPORT void edit_bboxes_set_selected(Entity ent, bool selected);

       EXPORT void edit_select_clear();
       EXPORT void edit_select_add(Entity ent);
       EXPORT void edit_select_remove(Entity ent);
       EXPORT bool edit_select_has(Entity ent);

       EXPORT void edit_init();
       EXPORT void edit_deinit();
       EXPORT void edit_update_all();
       EXPORT void edit_draw_all();

    )

#endif
