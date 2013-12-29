#define __CGAME_FFI__

#include "script.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "dirs.h"

#include "cgame_ffi.h"

static lua_State *L;

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
    lua_getfield(L, -1, "__fire_event");
    lua_remove(L, -2);
    lua_pushstring(L, event);
}

void script_init_all()
{
    L = lua_open();
    luaL_openlibs(L);

    errbegin
    {
        /*
         * send data path to lua, main.lua should update package.path with
         * this
         */
        lua_pushstring(L, data_path(""));
        lua_setglobal(L, "cgame_data_path");

        /* 
         * load cgame ffi -- equivalent to
         *
         *     ffi = require 'ffi'
         *     ffi.cdef(cgame_ffi[0] .. cgame_ffi[1] .. ...)
         *
         */

        lua_getglobal(L, "require");
        lua_pushstring(L, "ffi");
        errcheck(lua_pcall(L, 1, 0, 0));
        lua_getfield(L, lua_gettop(L), "cdef");

        luaL_Buffer buf;
        luaL_buffinit(L, &buf);
        for (unsigned int i = 0; i < n_cgame_ffi; ++i)
            luaL_addstring(&buf, *cgame_ffi[i]);
        luaL_pushresult(&buf);

        errcheck(lua_pcall(L, 1, 0, 0));

        /*
         * load 'cgame' module as a global -- this makes it easier to fire
         * events and also makes it so we don't have to "require 'cgame'" in
         * scripts
         *
         * TODO: fix this
         */
        /*
        lua_getglobal(L, "require");
        lua_pushstring(L, "cgame");
        errcheck(lua_pcall(L, 1, 0, 0));
        lua_setglobal(L, "cgame");
        */

        /* run main.lua */
        errcheck(luaL_loadfile(L, data_path("main.lua")));
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

