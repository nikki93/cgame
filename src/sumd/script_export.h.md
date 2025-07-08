# Summary: script_export.h

## Table of Contents

- [Script Export Header - Lua Integration and Platform Export Macros](#script-export-header---lua-integration-and-platform-export-macros)

This header file provides macros for integrating C code with Lua scripting and handling function exports across different platforms.

## Key Macros Defined

### SCRIPT(name, ...)
- **Purpose**: Makes C declarations visible from Lua while preserving normal C functionality
- **Conditional behavior based on `__CGAME_FFI__` preprocessor flag**:
  - **When `__CGAME_FFI__` is defined**: 
    - Creates a static string constant named `cgame_ffi_##name` containing the stringified version of the declarations
    - Also includes the original declarations unchanged
  - **When `__CGAME_FFI__` is not defined**:
    - Simply expands to the original declarations without modification
- **Requirements**:
  - Each `SCRIPT(...)` usage must have a unique `name` parameter
  - Users must also edit `cgame_ffi.h` (referenced as having explanatory comments)
- **Reference**: Points to `transform.h` as an example of usage

### EXPORT
- **Purpose**: Ensures function visibility from LuaJIT across different compilers
- **Platform-specific behavior**:
  - **MSVC (`_MSC_VER` defined)**: Expands to `__declspec(dllexport)` to explicitly export functions
  - **Other compilers**: Expands to nothing (empty), relying on default export behavior

## Dependencies and References
- **cgame_ffi.h**: Must be edited in conjunction with using `SCRIPT()` macro
- **transform.h**: Provided as example implementation
- **LuaJIT**: The target scripting engine for exported functions

## Usage Pattern
The file enables a dual-purpose approach where:
- C declarations can be used normally in C code through standard `#include`
- The same declarations are automatically made available to Lua when properly configured
- Functions are properly exported for dynamic linking regardless of compiler

## Sections

### Script Export Header - Lua Integration and Platform Export Macros
Source: [script_export.h:1-32](../script_export.h#L1-L32)

This header file provides macro definitions for integrating C code with Lua scripting and handling platform-specific function exports. It serves as a bridge between C/C++ code and Lua runtime environments.

## Key Macro Definitions

• **SCRIPT(name, ...)** - Dual-purpose macro for Lua integration:
  - When `__CGAME_FFI__` is defined:
    - Creates a static string constant `cgame_ffi_##name` containing the stringified version of the declarations
    - Expands to the actual declarations (`__VA_ARGS__`)
    - Enables FFI (Foreign Function Interface) string generation for Lua binding
  - When `__CGAME_FFI__` is not defined:
    - Simply expands to the declarations without FFI string generation
    - Functions as normal C declarations

• **EXPORT** - Platform-specific function export macro:
  - On Microsoft Visual C++ (`_MSC_VER` defined): Expands to `__declspec(dllexport)`
  - On other platforms: Expands to nothing (empty)
  - Purpose: Makes functions visible from LuaJIT on Windows where functions aren't exported by default

## Usage Instructions and Dependencies

• **Integration Requirements**:
  - Declarations must be surrounded with `SCRIPT(modulename, ...)` to be visible from Lua
  - Each `SCRIPT(...)` usage must have a unique 'name' parameter
  - References `transform.h` as an example implementation
  - Requires editing `cgame_ffi.h` as explained in comments within that file

• **Compatibility Design**:
  - Declarations continue to function as normal C declarations
  - Header can be included in other files and used normally
  - No interference with standard C/C++ compilation and linking

## Header Guard

• Uses standard include guard pattern with `SCRIPT_EXPORT_H` to prevent multiple inclusions


### Related
- [Parent](../)

---
Hash: ace50f5b4c15de824a5264b64910889570e177c2a8ce9b04f8af9dd23ca1a960
Generated: 2025-07-07T20:21:19-04:00
