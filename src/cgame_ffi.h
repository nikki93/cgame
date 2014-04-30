/*
 * C interface visible from Lua
 *
 * all declarations in SCRIPT(...) blocks from modules listed below are
 * available under 'cgame.' in Lua
 */

#define __CGAME_FFI__

#include "scalar.h"
#include "game.h"
#include "saveload.h"
#include "vec2.h"
#include "mat3.h"
#include "bbox.h"
#include "color.h"
#include "system.h"
#include "input.h"
#include "entity.h"
#include "prefab.h"
#include "timing.h"
#include "transform.h"
#include "camera.h"
#include "gui.h"
#include "sprite.h"
#include "console.h"
#include "physics.h"
#include "edit.h"

#include "test/keyboard_controlled.h"

/*
 * to include a module for Lua bindings, add here &cgame_ffi_NAME where NAME
 * is from SCRIPT(NAME, ...) as used in the definition of the module, and also
 * make sure to #include the relevant file above
 *
 * respect dependencies in the order -- for example 'transform' mentions 'Mat3'
 * and so 'mat3' must come before 'transform'
 */
static const char **cgame_ffi[] =
{
    &cgame_ffi_scalar,
    &cgame_ffi_saveload,
    &cgame_ffi_vec2,
    &cgame_ffi_mat3,
    &cgame_ffi_bbox,
    &cgame_ffi_color,
    &cgame_ffi_game,
    &cgame_ffi_system,
    &cgame_ffi_input,
    &cgame_ffi_entity,
    &cgame_ffi_prefab,
    &cgame_ffi_timing,
    &cgame_ffi_transform,
    &cgame_ffi_camera,
    &cgame_ffi_sprite,
    &cgame_ffi_gui,
    &cgame_ffi_console,
    &cgame_ffi_physics,
    &cgame_ffi_edit,

    &cgame_ffi_keyboard_controlled,
};

static const unsigned int n_cgame_ffi = sizeof(cgame_ffi)
    / sizeof(cgame_ffi[0]);

