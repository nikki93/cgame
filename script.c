#include "script.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

lua_State *L;

/* macrohax to make lua error handling simpler -- replace soon */
#define errbegin \
    do \
    { \
        switch (0) \
        { \
            default:
#define errcheck(p) \
                if (p) break
#define errend \
                continue; \
        } \
        fprintf(stderr, "lua: %s\n", lua_tostring(L, -1)); \
        lua_pop(L, 1); \
    } while (0)

static void _push_event(const char *event)
{
    lua_getglobal(L, "cgame");
    lua_getfield(L, lua_gettop(L), "__fire_event");
    lua_pushstring(L, event);
}

void script_init_all()
{
    L = lua_open();
    luaL_openlibs(L);

    errbegin
    {
        /* run main.lua */
        errcheck(luaL_loadfile(L, "main.lua"));
        errcheck(lua_pcall(L, 0, 0, 0));

        /* fire init event */
        _push_event("init");
        errcheck(lua_pcall(L, 1, 0, 0));
    }
    errend;
}

void script_deinit_all()
{
    errbegin
    {
        _push_event("deinit");
        errcheck(lua_pcall(L, 1, 0, 0));
    }
    errend;

    lua_close(L);
}

void script_update_all(float dt)
{
    errbegin
    {
        _push_event("update_all");
        lua_pushnumber(L, dt);
        errcheck(lua_pcall(L, 2, 0, 0));
    }
    errend;
}

void script_draw_all()
{
    errbegin
    {
        _push_event("draw_all");
        errcheck(lua_pcall(L, 1, 0, 0));
    }
    errend;
}

void script_load_all(FILE *f)
{
}

void script_save_all(FILE *f)
{
}

