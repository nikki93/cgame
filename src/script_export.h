#ifndef SCRIPT_EXPORT_H_OXLKH5DG
#define SCRIPT_EXPORT_H_OXLKH5DG

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
#define SCRIPT(name, ...)                               \
    static const char *cgame_ffi_##name = #__VA_ARGS__; \
    __VA_ARGS__
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

