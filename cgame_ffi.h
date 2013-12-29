/* 
 * C interface visible from Lua
 * 
 * all declarations in SCRIPT(...) blocks from modules listed below are
 * available under 'cgame.' in Lua
 */

#include "game.h"
#include "vec2.h"
#include "mat3.h"
#include "entity.h"
#include "transform.h"
#include "camera.h"
#include "sprite.h"

#include "test/keyboard_controlled.h"

/*
 * to include a module, add here &cgame_ffi_NAME where NAME is from
 * SCRIPT(NAME, ...) as used in the definition of the module
 *
 * make sure to respect dependencies in the order -- for example 'transform'
 * mentions 'Mat3' and so 'mat3' must come before 'transform'
 */
static const char **cgame_ffi[] =
{
    &cgame_ffi_game,
    &cgame_ffi_vec2,
    &cgame_ffi_mat3,
    &cgame_ffi_entity,
    &cgame_ffi_transform,
    &cgame_ffi_camera,
    &cgame_ffi_sprite,

    &cgame_ffi_keyboard_controlled,
};

const unsigned int n_cgame_ffi = sizeof(cgame_ffi) / sizeof(cgame_ffi[0]);

