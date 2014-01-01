#define __CGAME_FFI__

#include "script.h"

#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "dirs.h"

#include "cgame_ffi.h"

static lua_State *L;

#define errcheck(...) \
    do \
        if (__VA_ARGS__) \
        { \
            fprintf(stderr, "lua: %s\n", lua_tostring(L, -1)); \
            lua_pop(L, 1); \
        } \
    while (0)


static void _push_event(const char *event)
{
    lua_getglobal(L, "cgame");
    lua_getfield(L, -1, "__fire_event");
    lua_remove(L, -2);
    lua_pushstring(L, event);
}

/* LuaJIT parser doesn't like 'EXPORT' -- make it whitespace */
static void _fix_exports(char *s)
{
    static const char keyword[] = "EXPORT";
    unsigned int i;

    while ((s = strstr(s, keyword)))
        for (i = 0; i < sizeof(keyword) - 1; ++i)
            *s++ = ' ';
}

/* 
 * equivalent to:
 *
 *     ffi = require 'ffi'
 *     ffi.cdef(cgame_ffi[0] .. cgame_ffi[1] .. ...)
 *
 * with 'EXPORT's fixed
 */
static void _load_cgame_ffi()
{
    unsigned int i;
    char *fixed;
    luaL_Buffer buf; /* will accumulate cgame_ffi cdefs onto here */

    lua_getglobal(L, "require");
    lua_pushstring(L, "ffi");
    errcheck(lua_pcall(L, 1, 0, 0));
    lua_getfield(L, lua_gettop(L), "cdef");

    luaL_buffinit(L, &buf);
    for (i = 0; i < n_cgame_ffi; ++i)
    {
        fixed = malloc(strlen(*cgame_ffi[i]) + 1);
        strcpy(fixed, *cgame_ffi[i]);
        _fix_exports(fixed);
        luaL_addstring(&buf, fixed);
        free(fixed);
    }
    luaL_pushresult(&buf);

    errcheck(lua_pcall(L, 1, 0, 0));
}

void script_init()
{
    L = lua_open();
    luaL_openlibs(L);

    /* load ffi so that cgame.lua can bind it */
    _load_cgame_ffi();

    /* set cgame_data_path to data_path root and run main.lua */
    lua_pushstring(L, data_path(""));
    lua_setglobal(L, "cgame_data_path");
    errcheck(luaL_loadfile(L, data_path("main.lua")));
    errcheck(lua_pcall(L, 0, 0, 0));

    /* fire init event */
    _push_event("init");
    errcheck(lua_pcall(L, 1, 0, 0));
}

void script_deinit()
{
    _push_event("deinit");
    errcheck(lua_pcall(L, 1, 0, 0));

    lua_close(L);
}

void script_update_all(float dt)
{
    _push_event("update_all");
    lua_pushnumber(L, dt);
    errcheck(lua_pcall(L, 2, 0, 0));
}

void script_draw_all()
{
    _push_event("draw_all");
    errcheck(lua_pcall(L, 1, 0, 0));
}

void script_load_all(FILE *f)
{
}

void script_save_all(FILE *f)
{
}

