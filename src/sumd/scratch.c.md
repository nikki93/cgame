# Summary: scratch.c

## Table of Contents

- [Scratch File Monitoring and Execution System](#scratch-file-monitoring-and-execution-system)

This section implements a file monitoring and execution system for a Lua scratch file, providing automatic reloading capabilities when the file is modified.

## Key Components Defined

**Static Variables:**
- `filename` - Static string set to `usr_path("scratch.lua")`, representing the path to the scratch file

**Helper Functions:**
- `_exists()` - Checks if the scratch file exists using `stat()` system call
- `_modified()` - Tracks whether the file has been modified since the last check using modification timestamps

**Public Interface Functions:**
- `scratch_run()` - Executes the scratch Lua file
- `scratch_update()` - Conditionally runs the scratch file if it has been modified

## Dependencies and References

**External Dependencies:**
- `scratch.h` - Header file for this module
- `dirs.h` - Provides `usr_path()` function for path resolution
- `script.h` - Provides `script_run_file()` function for Lua execution
- Standard libraries: `stdbool.h`, `sys/stat.h`, `time.h`

## Algorithm Details

**File Modification Tracking (`_modified()`):**
- Maintains static variables to track state:
  - `prev_time` - Previous modification timestamp (initialized to 0)
  - `first` - Boolean flag to handle first call (initialized to true)
- Logic flow:
  1. Attempts to get file stats using `stat()`
  2. If `stat()` fails (file doesn't exist or error), returns false and clears first flag
  3. If successful, compares current modification time (`st.st_mtime`) with previous time
  4. Returns true only if not the first call AND modification time has changed
  5. Updates `prev_time` with current modification time and clears first flag

**Update Mechanism (`scratch_update()`):**
- Simple conditional execution: calls `scratch_run()` only if `_modified()` returns true
- Provides automatic reload functionality for development workflows

**Execution (`scratch_run()`):**
- Delegates to `script_run_file()` with the scratch file path
- Provides a simple wrapper for executing the monitored Lua script

This module enables hot-reloading of a user's scratch Lua file, commonly used in development environments for rapid prototyping and testing.

## Sections

### Scratch File Monitoring and Execution System
Source: [scratch.c:1-47](../scratch.c#L1-L47)

This section implements a file monitoring system for a Lua scratch file that automatically detects changes and executes the file when modified.

## Defined Entities

**Static Variables:**
- `filename` - Static string constant set to `usr_path("scratch.lua")`, represents the path to the scratch Lua file

**Functions:**
- `_exists()` - Private helper function that checks if the scratch file exists
- `_modified()` - Private helper function that tracks file modification state
- `scratch_run()` - Public function that executes the scratch file
- `scratch_update()` - Public function that conditionally runs the scratch file if modified

## Key Dependencies

- `usr_path()` from "dirs.h" - Used to get the user directory path for the scratch file
- `script_run_file()` from "script.h" - Used to execute the Lua file
- Standard C libraries: `<stdbool.h>`, `<sys/stat.h>`, `<time.h>`

## Detailed Logic

**File Existence Check (`_exists()`):**
- Uses `stat()` system call to check if the scratch file exists
- Returns `true` if file exists, `false` otherwise

**Modification Tracking (`_modified()`):**
- Maintains static state across function calls:
  - `prev_time` - Stores the last known modification time
  - `first` - Tracks if this is the first call to the function
- Algorithm steps:
  1. Attempts to get file statistics using `stat()`
  2. If `stat()` fails (file doesn't exist or error), returns `false`
  3. Compares current modification time (`st.st_mtime`) with previously stored time
  4. Returns `true` only if:
     - This is not the first call (`!first`)
     - AND the modification time has changed
  5. Updates `prev_time` with current modification time
  6. Sets `first` to `false` for subsequent calls

**Public Interface:**
- `scratch_run()` - Simple wrapper that calls `script_run_file()` with the scratch filename
- `scratch_update()` - Conditional execution function that only runs the scratch file if `_modified()` returns `true`

This system enables automatic execution of a user's scratch Lua file whenever it's modified, providing a hot-reload capability for development workflows.


### Related
- [Parent](../)

---
Hash: 90b28c8395c28479f4f03dbd0302092f649b070fcedfea16e0b9cf3339e0b158
Generated: 2025-07-07T20:21:19-04:00
