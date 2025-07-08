# Summary: timing.h

## Table of Contents

- [Time Management System Header](#time-management-system-header)

This header file defines the timing system interface for managing game time, including delta time tracking, time scaling, and pause functionality.

## External Dependencies
- `stdbool.h` - For boolean type support
- `scalar.h` - For `Scalar` type definitions
- `script_export.h` - For `SCRIPT` and `EXPORT` macros
- `saveload.h` - For `Store` type used in save/load operations

## Exported Variables (Script-Accessible)
- `timing_dt` - Current delta time value, affected by scaling and pause
- `timing_true_dt` - Actual unmodified delta time, unaffected by scale or pause operations

## Exported Functions (Script-Accessible)
- `timing_set_scale(Scalar s)` - Sets the time scale multiplier
- `timing_get_scale()` - Returns the current time scale value
- `timing_set_paused(bool p)` - Pauses/unpauses the timing system
  - When paused: sets scale to 0 and stores previous scale
  - When resumed: restores the previous scale value
- `timing_get_paused()` - Returns current pause state as boolean

## Internal Functions (C-Only)
- `timing_update()` - Updates the timing system (likely called each frame)
- `timing_save_all(Store *s)` - Saves timing state to storage
- `timing_load_all(Store *s)` - Loads timing state from storage

## Key Features
- **Time Scaling**: Allows speeding up or slowing down game time
- **Pause System**: Intelligent pause that preserves scale settings
- **Delta Time Tracking**: Maintains both scaled and unscaled time measurements
- **Script Integration**: Core timing functions exposed to scripting system
- **Persistence**: Save/load support for timing state

The system uses the `SCRIPT()` macro to define which entities are exported to the scripting interface, making timing control available to game scripts while keeping internal update and persistence functions private to the C code.

## Sections

### Time Management System Header
Source: [timing.h:1-29](../timing.h#L1-L29)

This header file defines a comprehensive time management system for controlling game timing, scaling, and pausing functionality.

## Header Guard and Dependencies
- Uses standard header guard pattern with `TIME_H`
- Depends on:
  - `stdbool.h` for boolean type support
  - `scalar.h` for the `Scalar` type
  - `script_export.h` for the `SCRIPT` macro
  - `saveload.h` for the `Store` type

## Script-Exported Timing Interface
The file uses a `SCRIPT(timing, ...)` macro to export timing functionality to scripts:

### Global Variables
- `timing_dt`: Main delta time variable (affected by scaling/pausing)
- `timing_true_dt`: Actual unmodified delta time that remains unaffected by time scale or pause state

### Time Scale Control Functions
- `timing_set_scale(Scalar s)`: Sets the time scale multiplier
- `timing_get_scale()`: Returns the current time scale value

### Pause Control Functions
- `timing_set_paused(bool p)`: Controls pause state
  - When pausing: sets scale to 0 and saves the previous scale
  - When resuming: restores the previously saved scale
- `timing_get_paused()`: Returns current pause state as boolean

## Internal System Functions
Functions not exported to scripts:

### Core Update Function
- `timing_update()`: Updates the timing system (likely called each frame)

### Save/Load System Integration
- `timing_save_all(Store *s)`: Saves all timing state to a data store
- `timing_load_all(Store *s)`: Loads timing state from a data store

## Key Design Features
- **Dual delta time system**: Maintains both scaled (`timing_dt`) and unscaled (`timing_true_dt`) time values
- **Pause mechanism**: Implements pause by temporarily setting scale to zero rather than stopping time completely
- **Script integration**: Core timing functionality is accessible from scripting systems
- **Persistence support**: Timing state can be saved and loaded for game state management


### Related
- [Parent](../)

---
Hash: 342f62b2b2db17369efbfcfba1da9cf6fbd397773483ce9a19aa67b2d2492a60
Generated: 2025-07-07T20:21:19-04:00
