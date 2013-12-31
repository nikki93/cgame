#ifndef SCRIPT_H_GYSDQCZ0
#define SCRIPT_H_GYSDQCZ0

#include <stdio.h>

void script_init_all();
void script_deinit_all();
void script_update_all(float dt);
void script_draw_all();
void script_load_all(FILE *f);
void script_save_all(FILE *f);

/*
 * surround declarations with SCRIPT(modulename, ...) (check transform.h for
 * an example) to make them visible from Lua -- you'll also need to edit
 * cgame_ffi.h as explained in the comments there
 *
 * each SCRIPT(...) mention must have a different modulename
 *
 * the declarations will continue to function as usual C declarations too
 * so you can #include into other files and use them normally
 */
#ifdef __CGAME_FFI__
#define SCRIPT(name, arg) \
    static const char *cgame_ffi_##name = #arg; \
    arg
#else
#define SCRIPT(name, ...) __VA_ARGS__
#endif

/*
 * place this in front of functions -- MSVC doesn't export functions by
 * default and they won't be visible from luajit otherwise
 */
#ifdef _MSC_VER
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#endif

