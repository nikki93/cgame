#include "cgame_ffi.h"

#include "script.h"

#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "dirs.h"
#include "saveload.h"
#include "game.h"
#include "input.h"
#include "console.h"

static lua_State *L;

#define errcheck(...)                                           \
    do                                                          \
        if (__VA_ARGS__)                                        \
        {                                                       \
            console_printf("lua: %s\n", lua_tostring(L, -1));   \
            lua_pop(L, 1);                                      \
        }                                                       \
    while (0)

void script_run_string(const char *s)
{
    errcheck(luaL_dostring(L, s));
}
void script_run_file(const char *filename)
{
    errcheck(luaL_dofile(L, filename));
}

/*
 * push an object as cdata, t must be the FFI type specifier as a string -- so to
 * push a Vec2 you'd do _push_cdata("Vec2 *", &v), and the result is a Vec2 cdata
 * on stack (NOT a pointer)
 */
static void _push_cdata(const char *t, void *p)
{
    lua_getglobal(L, "cgame");
    lua_getfield(L, -1, "__deref_cdata");
    lua_remove(L, -2);
    lua_pushstring(L, t);
    lua_pushlightuserdata(L, p);
    errcheck(lua_pcall(L, 2, 1, 0));
}

static void _push_event(const char *event)
{
    lua_getglobal(L, "cgame");
    lua_getfield(L, -1, "__fire_event");
    lua_remove(L, -2);
    lua_pushstring(L, event);
}

/* args are available as cgame_args[0], cgame_args[1], ... */
static void _forward_args()
{
    int i, argc;
    char **argv;

    argc = game_get_argc();
    argv = game_get_argv();

    lua_createtable(L, argc, 0);
    for (i = 0; i < argc; ++i)
    {
        lua_pushstring(L, argv[i]);
        lua_rawseti(L, -2, i);
    }
    lua_setglobal(L, "cgame_args");
}

/* let Lua know about paths */
static void _set_paths()
{
    lua_pushstring(L, data_path(""));
    lua_setglobal(L, "cgame_data_path");
    lua_pushstring(L, usr_path(""));
    lua_setglobal(L, "cgame_usr_path");
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
 * with 'EXPORT's fixed -- after this cgame.lua can bind the ffi
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

    _load_cgame_ffi();
    _forward_args();
    _set_paths();

    /* run main.lua */
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

void script_update_all()
{
    _push_event("update_all");
    errcheck(lua_pcall(L, 1, 0, 0));
}

void script_clear()
{
    _push_event("clear");
    errcheck(lua_pcall(L, 1, 0, 0));
}

void script_draw_all()
{
    _push_event("draw_all");
    errcheck(lua_pcall(L, 1, 0, 0));
}

void script_key_down(KeyCode key)
{
    _push_event("key_down");
    _push_cdata("KeyCode *", &key);
    errcheck(lua_pcall(L, 2, 0, 0));
}
void script_key_up(KeyCode key)
{
    _push_event("key_up");
    _push_cdata("KeyCode *", &key);
    errcheck(lua_pcall(L, 2, 0, 0));
}

void script_mouse_down(MouseCode mouse)
{
    _push_event("mouse_down");
    _push_cdata("MouseCode *", &mouse);
    errcheck(lua_pcall(L, 2, 0, 0));
}
void script_mouse_up(MouseCode mouse)
{
    _push_event("mouse_up");
    _push_cdata("MouseCode *", &mouse);
    errcheck(lua_pcall(L, 2, 0, 0));
}

void script_save_all(Serializer *s)
{
    const char *str;

    lua_getglobal(L, "cgame");
    lua_getfield(L, -1, "__save_all");
    lua_remove(L, -2);
    errcheck(lua_pcall(L, 0, 1, 0));
    str = lua_tostring(L, -1);

    string_save(&str, s);

    lua_pop(L, 1);
}

void script_load_all(Deserializer *s)
{
    char *str;

    string_load(&str, s);

    lua_getglobal(L, "cgame");
    lua_getfield(L, -1, "__load_all");
    lua_remove(L, -2);
    lua_pushstring(L, str);
    errcheck(lua_pcall(L, 1, 0, 0));

    free(str);
}

