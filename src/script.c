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

static int _traceback(lua_State *L)
{
    if (!lua_isstring(L, 1))
    {
        if (lua_isnoneornil(L, 1) || !luaL_callmeta(L, 1, "__tostring")
            || !lua_isstring(L, -1))
            return 1;
        lua_remove(L, 1);
    }
    luaL_traceback(L, L, lua_tostring(L, 1), 1);
    return 1;
}

int _pcall(lua_State *L, int nargs, int nresults)
{
    int r, errfunc;

    /* put _traceback under function and args */
    errfunc = lua_gettop(L) - nargs;
    lua_pushcfunction(L, _traceback);
    lua_insert(L, errfunc);

    /* call, remove _traceback */
    r = lua_pcall(L, nargs, nresults, errfunc);
    lua_remove(L, errfunc);
    return r;
}

void script_run_string(const char *s)
{
    errcheck(luaL_dostring(L, s));
}
void script_run_file(const char *filename)
{
    errcheck(luaL_dofile(L, filename));
}

/*
 * push an object as cdata, t must be the FFI type specifier as a string -- so
 * to push a Vec2 you'd do _push_cdata("Vec2 *", &v), and the result is a Vec2
 * cdata on stack (NOT a pointer)
 */
static void _push_cdata(const char *t, void *p)
{
    /* just call __deref_cdata(t, p) */
    lua_getglobal(L, "cgame");
    lua_getfield(L, -1, "__deref_cdata");
    lua_remove(L, -2);
    lua_pushstring(L, t);
    lua_pushlightuserdata(L, p);
    errcheck(_pcall(L, 2, 1));
}

static void _push_event(const char *event)
{
    /* call cgame.__fire_event(event, ...) */
    lua_getglobal(L, "cgame");
    lua_getfield(L, -1, "__fire_event");
    lua_remove(L, -2);
    lua_pushstring(L, event);
}

/* forward commandline args as cgame_args[0], cgame_args[1], ... */
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

/* LuaJIT FFI parser doesn't like 'EXPORT' -- make it whitespace */
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
 * with 'EXPORT's fixed -- after this cgame.lua can bind the FFI
 */
static void _load_cgame_ffi()
{
    unsigned int i;
    char *fixed;
    luaL_Buffer buf; /* will accumulate cgame_ffi cdefs onto here */

    /* get ffi.cdef */
    lua_getglobal(L, "require");
    lua_pushstring(L, "ffi");
    errcheck(_pcall(L, 1, 1));
    lua_getfield(L, lua_gettop(L), "cdef");

    /* accumulate cgame_ffi cdefs */
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

    /* call it */
    errcheck(_pcall(L, 1, 0));
}

void script_init()
{
    L = lua_open();
    luaL_openlibs(L);

    _load_cgame_ffi();
    _forward_args();
    _set_paths();

    /* run main.lua */
    errcheck(luaL_loadfile(L, data_path("script/main.lua")));
    errcheck(_pcall(L, 0, 0));

    /* fire init event */
    _push_event("init");
    errcheck(_pcall(L, 1, 0));
}

void script_deinit()
{
    _push_event("deinit");
    errcheck(_pcall(L, 1, 0));

    lua_close(L);
}

void script_update_all()
{
    _push_event("update_all");
    errcheck(_pcall(L, 1, 0));
}

void script_post_update_all()
{
    _push_event("post_update_all");
    errcheck(_pcall(L, 1, 0));
}

void script_draw_all()
{
    _push_event("draw_all");
    errcheck(_pcall(L, 1, 0));
}

void script_key_down(KeyCode key)
{
    _push_event("key_down");
    _push_cdata("KeyCode *", &key);
    errcheck(_pcall(L, 2, 0));
}
void script_key_up(KeyCode key)
{
    _push_event("key_up");
    _push_cdata("KeyCode *", &key);
    errcheck(_pcall(L, 2, 0));
}

void script_mouse_down(MouseCode mouse)
{
    _push_event("mouse_down");
    _push_cdata("MouseCode *", &mouse);
    errcheck(_pcall(L, 2, 0));
}
void script_mouse_up(MouseCode mouse)
{
    _push_event("mouse_up");
    _push_cdata("MouseCode *", &mouse);
    errcheck(_pcall(L, 2, 0));
}

void script_mouse_move(Vec2 pos)
{
    _push_event("mouse_move");
    _push_cdata("Vec2 *", &pos);
    errcheck(_pcall(L, 2, 0));
}

void script_save_all(Serializer *s)
{
    const char *str;

    /* get string from Lua */
    lua_getglobal(L, "cgame");
    lua_getfield(L, -1, "__save_all");
    lua_remove(L, -2);
    errcheck(_pcall(L, 0, 1));
    str = lua_tostring(L, -1);

    /* save it */
    string_save(&str, s);

    /* release */
    lua_pop(L, 1);
}

void script_load_all(Deserializer *s)
{
    char *str;

    /* load the string */
    string_load(&str, s);

    /* send it to Lua */
    lua_getglobal(L, "cgame");
    lua_getfield(L, -1, "__load_all");
    lua_remove(L, -2);
    lua_pushstring(L, str);
    errcheck(_pcall(L, 1, 0));

    /* release */
    free(str);
}

