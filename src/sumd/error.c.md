# Summary: error.c

## Table of Contents

- [Error Handling Module with Formatted Output](#error-handling-module-with-formatted-output)

This section implements a simple error handling system that provides formatted error message capabilities and delegates actual error processing to a script module.

## Defined Functions

• **`_error(const char *s)`** - Static helper function
  - Takes a single string parameter
  - Calls `script_error(s)` to delegate error handling to the script module
  - Serves as an internal wrapper for the external error handling mechanism

• **`errorf(const char *fmt, ...)`** - Public formatted error function
  - Accepts printf-style format string and variable arguments
  - Implements a two-pass approach for safe formatted string creation:
    1. **Size calculation pass**: Uses `vsnprintf(NULL, 0, fmt, ap2)` to determine required buffer size
    2. **String creation pass**: Allocates exact buffer size, formats string with `vsprintf()`
  - **Memory management**: Dynamically allocates buffer with `malloc(n + 1)` and properly frees it
  - **Variable argument handling**: Uses `va_list`, `va_start`, `va_copy`, and `va_end` for safe argument processing
  - Calls `_error(s)` to process the formatted error message

## Dependencies

• **External headers**: 
  - `stdarg.h` - for variable argument list handling
  - `stdio.h` - for formatted string functions
  - `stdlib.h` - for memory allocation
  - `error.h` - module's own header
  - `script.h` - for script error handling integration

• **External function references**:
  - `script_error()` - external function that handles the actual error processing

## Key Implementation Details

• Uses `va_copy()` to safely duplicate variable argument lists, allowing multiple passes over the same arguments
• Employs the standard technique of calling `vsnprintf()` with NULL buffer to determine required space
• Ensures proper cleanup by freeing allocated memory after error processing
• Separates formatting logic from error handling logic through the `_error()` abstraction layer

## Sections

### Error Handling Module with Formatted Output
Source: [error.c:1-30](../error.c#L1-L30)

This section implements a formatted error handling system that provides printf-style error reporting functionality.

## Defined Entities

• **`_error(const char *s)`** - Static helper function that forwards error messages to the script error handler
• **`errorf(const char *fmt, ...)`** - Public variadic function that formats error messages using printf-style formatting

## Dependencies

• **External headers**: `stdarg.h`, `stdio.h`, `stdlib.h`, `"error.h"`, `"script.h"`
• **External function**: `script_error(s)` - Called to actually handle/display the error message

## Implementation Details

### `_error` function
• Simple wrapper that calls `script_error()` with the provided string
• Serves as an abstraction layer between formatting logic and actual error handling

### `errorf` function algorithm
The function implements a two-pass approach to handle variable-length formatted strings:

1. **Memory calculation phase**:
   - Declares two `va_list` variables (`ap1`, `ap2`) for argument processing
   - Starts both argument lists with `va_start(ap1, fmt)` and `va_copy(ap2, ap1)`
   - Calls `vsnprintf(NULL, 0, fmt, ap2)` to determine required buffer size
   - Ends the copied argument list with `va_end(ap2)`

2. **Formatting and output phase**:
   - Allocates memory buffer of size `n + 1` bytes using `malloc()`
   - Formats the string into the buffer using `vsprintf(s, fmt, ap1)`
   - Ends the original argument list with `va_end(ap1)`
   - Passes the formatted string to `_error()` for processing
   - Frees the allocated memory with `free(s)`

## Key Features

• **Memory-safe formatting**: Calculates exact buffer size needed before allocation
• **Standard printf formatting**: Supports all standard printf format specifiers through `vsprintf`
• **Clean resource management**: Properly manages `va_list` objects and dynamically allocated memory
• **Modular design**: Separates formatting logic from error handling implementation


### Related
- [Parent](../)

---
Hash: 72388a7887e4c206eef8bbda84066b88774c2d0fade373c4f9bf40c21e30ecab
Generated: 2025-07-07T20:21:19-04:00
