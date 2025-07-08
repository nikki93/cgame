# Summary: script.c

## Table of Contents

- [Lua Script Integration and FFI Binding System](#lua-script-integration-and-ffi-binding-system)

This file implements a Lua scripting engine that provides the core bridge between the C game engine and Lua scripts, handling initialization, event dispatching, and FFI bindings.

## Core Components

### Global State and Error Handling
- **L**: Static lua_State pointer that maintains the global Lua interpreter instance
- **errcheck**: Macro that handles Lua error checking, prints errors to console via `console_printf`, and cleans up the Lua stack
- **_traceback**: Internal function that provides enhanced error reporting with stack traces for Lua errors
- **_pcall**: Protected call wrapper that adds traceback functionality to standard lua_pcall

### Script Execution Functions
- **script_run_string**: Executes Lua code from a string parameter
- **script_run_file**: Loads and executes Lua code from a file
- **script_error**: Triggers a Lua error with specified message

### Data Exchange System
- **_push_cdata**: Pushes C data structures to Lua as FFI cdata objects
  - Takes a type specifier string and void pointer
  - Calls `cg.__deref_cdata(t, p)` to convert C pointers to Lua cdata
  - Example: `_push_cdata("Vec2 *", &v)` creates a Vec2 cdata on the Lua stack

### Event System
- **_push_event**: Helper function that prepares event firing by calling `cg.__fire_event`
- **Event handler functions** that bridge C events to Lua:
  - `script_key_down` / `script_key_up`: Keyboard events with KeyCode parameters
  - `script_mouse_down` / `script_mouse_up`: Mouse button events with MouseCode parameters  
  - `script_mouse_move`: Mouse movement with Vec2 position
  - `script_scroll`: Scroll wheel events with Vec2 scroll data
  - `script_update_all` / `script_post_update_all` / `script_draw_all`: Game loop events

### Initialization System
- **_forward_args**: Copies command line arguments to Lua global `cgame_args` table
  - Uses `game_get_argc()` and `game_get_argv()` to access arguments
- **_set_paths**: Sets up path globals for Lua scripts
  - `cgame_data_path`: Set via `data_path("")`
  - `cgame_usr_path`: Set via `usr_path("")`
- **_fix_exports**: String processing function that replaces "EXPORT" keywords with whitespace for FFI compatibility
- **_load_cgame_ffi**: Sets up FFI bindings for C interop
  - Requires the 'ffi' module
  - Accumulates all `cgame_ffi` definitions into a buffer
  - Processes exports and calls `ffi.cdef()` with the combined definitions

### Lifecycle Management
- **script_init**: Complete initialization sequence
  - Creates Lua state with `lua_open()` and `luaL_openlibs()`
  - Loads FFI bindings, forwards args, sets paths
  - Executes `data_path("script/main.lua")`
  - Fires "init" event
- **script_deinit**: Cleanup sequence
  - Fires "deinit" event
  - Closes Lua state with `lua_close()`

### Save/Load System
- **script_save_all**: Serializes Lua state to Store
  - Calls `cg.__save_all()` to get serialized string from Lua
  - Saves string to child store named "script"
- **script_load_all**: Deserializes Lua state from Store
  - Loads string from "script" child store
  - Calls `cg.__load_all(str)` to restore Lua state

## Key Dependencies
- **cgame_ffi.h**: Provides FFI definitions array (`cgame_ffi`, `n_cgame_ffi`)
- **Lua libraries**: lua.h, lauxlib.h, lualib.h for core Lua functionality
- **Game modules**: dirs.h, saveload.h, game.h, input.h, console.h for engine integration
- **Lua-side dependencies**: Expects `cg` global table with `__deref_cdata`, `__fire_event`, `__save_all`, `__load_all` functions

## Sections

### Lua Script Integration and FFI Binding System
Source: [script.c:1-292](../script.c#L1-L292)

This section implements a comprehensive Lua scripting system with FFI (Foreign Function Interface) bindings for a C game engine. The code provides a bridge between C game engine functionality and Lua scripting capabilities.

## Core Components

### Global State and Error Handling
- **Global variable `L`**: Static `lua_State *` that maintains the Lua interpreter state
- **`errcheck` macro**: Error handling macro that prints Lua errors to console and pops error from stack
- **`_traceback` function**: Lua C function that generates proper stack traces for errors
- **`_pcall` function**: Protected call wrapper that includes traceback functionality

### Script Execution Functions
- **`script_run_string(const char *s)`**: Executes Lua code from a string
- **`script_run_file(const char *filename)`**: Executes Lua code from a file
- **`script_error(const char *s)`**: Triggers a Lua error with given message

### FFI Integration System
- **`_push_cdata(const char *t, void *p)`**: Core FFI function that pushes C data to Lua as cdata
  - Takes FFI type specifier string and pointer to data
  - Calls `cg.__deref_cdata(t, p)` to create proper cdata object
  - Example: `_push_cdata("Vec2 *", &v)` creates Vec2 cdata on Lua stack
- **`_fix_exports(char *s)`**: Preprocesses C header definitions by replacing "EXPORT" keywords with whitespace for LuaJIT FFI parser compatibility
- **`_load_cgame_ffi()`**: Initializes FFI bindings by:
  - Loading the `ffi` module
  - Accumulating all `cgame_ffi` definitions into a buffer
  - Fixing export keywords in each definition
  - Calling `ffi.cdef()` with the complete definition string

### Event System
- **`_push_event(const char *event)`**: Helper function that prepares event calls by pushing `cg.__fire_event` and event name onto stack
- **Event handler functions** that bridge C events to Lua:
  - `script_key_down(KeyCode key)` / `script_key_up(KeyCode key)`
  - `script_mouse_down(MouseCode mouse)` / `script_mouse_up(MouseCode mouse)`
  - `script_mouse_move(Vec2 pos)`
  - `script_scroll(Vec2 scroll)`
  - `script_update_all()` / `script_post_update_all()` / `script_draw_all()`

### Initialization and Lifecycle
- **`script_init()`**: Complete initialization sequence:
  - Creates new Lua state with `lua_open()`
  - Opens standard Lua libraries
  - Loads cgame FFI definitions
  - Sets up command line arguments as `cgame_args` global
  - Sets up file paths as `cgame_data_path` and `cgame_usr_path` globals
  - Loads and executes `main.lua` from data path
  - Fires "init" event
- **`script_deinit()`**: Cleanup sequence that fires "deinit" event and closes Lua state

### Configuration Setup
- **`_forward_args()`**: Creates `cgame_args` Lua table containing command line arguments from `game_get_argc()` and `game_get_argv()`
- **`_set_paths()`**: Sets up global path variables:
  - `cgame_data_path`: from `data_path("")`
  - `cgame_usr_path`: from `usr_path("")`

### Save/Load System
- **`script_save_all(Store *s)`**: Serialization function that:
  - Creates child store named "script"
  - Calls `cg.__save_all()` to get serialized state string from Lua
  - Saves the string to the store
- **`script_load_all(Store *s)`**: Deserialization function that:
  - Loads string from "script" child store
  - Calls `cg.__load_all(str)` to restore Lua state

## Key Dependencies
- **External modules**: `cgame_ffi.h`, `script.h`, `dirs.h`, `saveload.h`, `game.h`, `input.h`, `console.h`
- **Lua libraries**: `lua.h`, `lauxlib.h`, `lualib.h`
- **Referenced types**: `KeyCode`, `MouseCode`, `Vec2`, `Store`
- **External functions**: `data_path()`, `usr_path()`, `game_get_argc()`, `game_get_argv()`, `console_printf()`

This system provides a complete bridge between C game engine functionality and Lua scripting, enabling the game to be largely driven by Lua scripts while maintaining performance-critical operations in C.


### Related
- [Parent](../)

---
Hash: ea78476015e307bbf24ae570a31300a70a88009373026f8669d97a5b8b8a009e
Generated: 2025-07-07T20:21:19-04:00
