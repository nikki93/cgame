# Summary: error.h

## Table of Contents

- [Error Handling Header with Macro-based Formatting](#error-handling-header-with-macro-based-formatting)

This header file defines a comprehensive error handling system that provides printf-style error formatting with automatic file and line number inclusion. The system is designed to cause exceptions in scripts when errors occur in C code called from scripts, otherwise it aborts execution.

## Core Function
- **`errorf(const char *fmt, ...)`**: Main error function that accepts printf-style format strings and variable arguments for flexible error message formatting

## Macro System for Line Information
The file implements a three-level macro system to automatically include file and line number information:

- **`line_str__(line)`**: Bottom-level macro that concatenates `__FILE__`, a colon, the stringified line number, and a colon-space suffix
  - Uses `#line` to stringify the line number parameter
  - Produces format: `"filename:linenumber: "`

- **`line_str_(line)`**: Middle-level macro that calls `line_str__(line)`
  - Serves as an indirection layer for proper macro expansion

- **`line_str()`**: Top-level macro that calls `line_str_(__LINE__)`
  - Uses `__LINE__` to automatically capture the current line number
  - This is the macro typically used by other macros

## High-Level Error Macros
- **`error(...)`**: Primary error macro that combines automatic file/line information with custom error messages
  - Calls `errorf()` with `line_str()` prepended to the variable arguments
  - Accepts printf-style format strings and arguments via `__VA_ARGS__`

- **`error_assert(cond, ...)`**: Assertion macro that checks a condition and reports errors with context
  - Evaluates the condition `cond`
  - If condition is true, returns 0 (no error)
  - If condition is false, calls `error()` with:
    - Automatic file/line information
    - The literal condition text (via `#cond`)
    - Custom error message arguments
  - Message format: `"assertion 'condition' failed ... [custom message]"`

## Dependencies
- Relies on standard C preprocessor features: `__FILE__`, `__LINE__`, `#` (stringification), and `__VA_ARGS__`
- Uses include guards (`#ifndef ERROR_H`, `#define ERROR_H`, `#endif`) to prevent multiple inclusions

## Sections

### Error Handling Header with Macro-based Formatting
Source: [error.h:1-28](../error.h#L1-L28)

This header file defines a comprehensive error handling system that provides printf-style error formatting with automatic file and line number inclusion. The system is designed to cause exceptions in scripts when errors occur in C code called from scripts, otherwise it aborts execution.

## Core Functions and Macros Defined

• **`errorf(const char *fmt, ...)`** - Main error function that accepts printf-style format strings and variable arguments for error reporting

• **Line number string generation macros** (three-level macro expansion):
  - **`line_str__(line)`** - Lowest level macro that converts `__FILE__` and the line parameter into a string literal in format `"filename:linenumber: "`
  - **`line_str_(line)`** - Middle level macro that calls `line_str__(line)` to enable proper macro expansion
  - **`line_str()`** - Top level macro that calls `line_str_(__LINE__)` to capture the current line number

• **`error(...)`** - Convenience macro that combines line string generation with `errorf()`, automatically prepending filename and line number to any error message

• **`error_assert(cond, ...)`** - Assertion macro that:
  - Evaluates the condition `cond`
  - If condition is true, returns 0 (no error)
  - If condition is false, calls `error()` with a formatted message that includes:
    - The stringified condition using `#cond`
    - The text "assertion 'condition' failed ... "
    - Any additional printf-style arguments provided

## Key Design Features

• **Automatic location tracking** - The three-level macro expansion ensures that `__LINE__` is properly evaluated at the call site rather than at the macro definition site

• **Consistent error formatting** - All errors automatically include file and line information in a standardized format

• **Flexible messaging** - Both `error()` and `error_assert()` accept variable arguments for custom error messages

• **Script integration** - The system is specifically designed to integrate with a scripting environment, throwing exceptions when called from scripts

The header uses standard C preprocessor techniques and relies on the `__FILE__` and `__LINE__` built-in macros for automatic location tracking.


### Related
- [Parent](../)

---
Hash: 98b85afdc595f1940238995b8c1cef201fe824f2bf41dce9f21c6720ddec11b3
Generated: 2025-07-07T20:21:19-04:00
