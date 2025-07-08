# Summary: timing.c

## Table of Contents

- [Timing System Implementation](#timing-system-implementation)

This section implements a timing system that manages delta time calculations with pause and scaling functionality for game/simulation timing control.

## Global Variables Defined
- `timing_dt`: Current scaled delta time (public)
- `timing_true_dt`: Actual unscaled delta time (public)
- `scale`: Time scaling factor (static, default 1.0f)
- `paused`: Pause state flag (static, default false)

## Functions Defined

### Time Scale Management
- `timing_set_scale(Scalar s)`: Sets the time scaling factor
- `timing_get_scale()`: Returns the current time scaling factor

### Pause Control
- `timing_set_paused(bool p)`: Sets the pause state
- `timing_get_paused()`: Returns the current pause state

### Core Timing Logic
- `_dt_update()`: Internal delta time calculation function
  - Uses static variable `last_time` to track previous frame time
  - On first call (when `last_time < 0`), initializes `last_time` with current time
  - Calculates `timing_true_dt` as difference between current and last time
  - Sets `timing_dt` to 0 if paused, otherwise scales `timing_true_dt` by `scale`
  - Updates `last_time` for next frame

- `timing_update()`: Public interface that calls `_dt_update()`

### Save/Load System
- `timing_save_all(Store *s)`: Saves timing state to storage
  - Creates child store named "timing"
  - Saves `scale` value with key "scale"
- `timing_load_all(Store *s)`: Loads timing state from storage
  - Loads from child store named "timing"
  - Loads `scale` value with key "scale" and default value 1

## Key Dependencies
- `timing.h`: Header file for timing system declarations
- `glew_glfw.h`: For `glfwGetTime()` function used in time measurement
- `Store` type: Used for save/load functionality
- `Scalar` type: Used for time values
- `store_child_save()`, `store_child_load()`: Storage system functions
- `scalar_save()`, `scalar_load()`: Scalar value persistence functions

## Algorithm Overview
The timing system works by:
1. Tracking real elapsed time between frames using GLFW's time function
2. Applying pause logic (setting delta time to 0 when paused)
3. Applying time scaling to create gameplay effects (slow motion, fast forward)
4. Providing both scaled and unscaled delta time values for different use cases

## Sections

### Timing System Implementation
Source: [timing.c:1-61](../timing.c#L1-L61)

This section implements a timing system that manages time scaling and pausing functionality for what appears to be a game or simulation engine.

## Global Variables Defined
- `timing_dt`: Scalar representing the scaled delta time
- `timing_true_dt`: Scalar representing the actual unscaled delta time
- `scale`: Static scalar for time scaling factor (initialized to 1.0f)
- `paused`: Static boolean for pause state (initialized to false)

## Functions Defined

### Time Scale Management
- `timing_set_scale(Scalar s)`: Sets the time scaling factor
- `timing_get_scale()`: Returns the current time scaling factor

### Pause Management  
- `timing_set_paused(bool p)`: Sets the pause state
- `timing_get_paused()`: Returns the current pause state

### Core Timing Logic
- `_dt_update()`: Private function that calculates delta time
  - Uses static variable `last_time` to track previous frame time
  - On first call (when `last_time < 0`), initializes `last_time` with current time
  - Calculates `timing_true_dt` as the difference between current and last time
  - Sets `timing_dt` to 0 if paused, otherwise scales `timing_true_dt` by the scale factor
  - Updates `last_time` for next frame
- `timing_update()`: Public interface that calls `_dt_update()`

### Persistence Functions
- `timing_save_all(Store *s)`: Saves timing state to storage
  - Creates child store named "timing"
  - Saves the `scale` value with key "scale"
- `timing_load_all(Store *s)`: Loads timing state from storage  
  - Loads child store named "timing"
  - Loads the `scale` value with key "scale" and default value 1

## Key Dependencies
- `timing.h`: Header file for this module
- `glew_glfw.h`: For OpenGL/GLFW functionality
- `glfwGetTime()`: GLFW function used to get current time
- `Store`, `store_child_save()`, `store_child_load()`: Storage system for persistence
- `scalar_save()`, `scalar_load()`: Scalar value persistence functions
- `Scalar`: Type definition for floating-point values (defined elsewhere)

## Algorithm Overview
The timing system maintains two delta time values: a true delta time representing actual elapsed time, and a scaled delta time that can be modified by scaling factors and pause states. This allows for game-like time manipulation such as slow motion, fast forward, and pause functionality.


### Related
- [Parent](../)

---
Hash: a3f8aab95bab5ffa796420502840cc9235d42da5a3c19417181fb3be6f89ddcd
Generated: 2025-07-07T20:21:19-04:00
