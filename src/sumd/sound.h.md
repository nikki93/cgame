# Summary: sound.h

## Table of Contents

- [Sound System Header - Entity-Based Audio Management](#sound-system-header---entity-based-audio-management)

This header file defines the sound system component interface for managing audio entities in what appears to be an entity-component-system (ECS) architecture.

## Key Dependencies
- `entity.h` - Provides the `Entity` type used throughout the interface
- `script_export.h` - Provides the `SCRIPT` macro and `EXPORT` functionality for script bindings

## Script-Exported Functions (Public API)
The following functions are exported to scripting systems via the `SCRIPT` macro:

### Entity Management
- `sound_add(Entity ent)` - Adds sound component to an entity
- `sound_remove(Entity ent)` - Removes sound component from an entity  
- `sound_has(Entity ent)` - Checks if entity has sound component

### Audio File Management
- `sound_set_path(Entity ent, const char *path)` - Sets audio file path
- `sound_get_path(Entity ent)` - Gets audio file path

### Playback Control
- `sound_set_playing(Entity ent, bool playing)` - Controls play/pause state
- `sound_get_playing(Entity ent)` - Gets current play/pause state
- `sound_set_seek(Entity ent, int seek)` - Sets playback position
- `sound_get_seek(Entity ent)` - Gets current playback position

### Playback Behavior
- `sound_set_finish_destroy(Entity ent, bool finish_destroy)` - Sets whether entity should be destroyed when audio finishes
- `sound_get_finish_destroy(Entity ent)` - Gets finish_destroy setting
- `sound_set_loop(Entity ent, bool loop)` - Sets looping behavior
- `sound_get_loop(Entity ent)` - Gets looping setting

### Audio Properties
- `sound_set_gain(Entity ent, Scalar gain)` - Sets audio volume/gain level
- `sound_get_gain(Entity ent)` - Gets current gain level

## System Management Functions (Internal API)
These functions are not exported to scripts and handle system-level operations:

- `sound_init()` - Initializes the sound system
- `sound_deinit()` - Cleans up the sound system
- `sound_update_all()` - Updates all sound entities (likely called each frame)
- `sound_save_all(Store *s)` - Serializes all sound components to storage
- `sound_load_all(Store *s)` - Deserializes sound components from storage

## Referenced Types
- `Entity` - Entity identifier type from entity system
- `Scalar` - Floating-point type for gain values
- `Store` - Storage/serialization system type

The header follows standard C practices with include guards (`#ifndef SOUND_H`) and provides a complete interface for managing audio components in an entity-based game or application system.

## Sections

### Sound System Header - Entity-Based Audio Management
Source: [sound.h:1-39](../sound.h#L1-L39)

This header file defines a comprehensive sound system API built around an entity-component architecture. The sound system provides audio playback capabilities with full control over playback state, positioning, looping, and volume management.

## Key Dependencies
- **`entity.h`**: Provides the `Entity` type used throughout the API
- **`script_export.h`**: Provides the `SCRIPT` macro and `EXPORT` functionality for script bindings
- **`Store`**: Referenced for serialization (defined elsewhere)
- **`Scalar`**: Used for gain/volume values (defined elsewhere)

## Main API Functions (Script-Exported)

The core API is wrapped in a `SCRIPT()` macro, making all functions available to scripting systems:

### Entity Management
- **`sound_add(Entity ent)`**: Adds sound component to an entity
- **`sound_remove(Entity ent)`**: Removes sound component from an entity  
- **`sound_has(Entity ent)`**: Checks if entity has sound component

### Audio File Management
- **`sound_set_path(Entity ent, const char *path)`**: Sets audio file path
- **`sound_get_path(Entity ent)`**: Returns current audio file path

### Playback Control
- **`sound_set_playing(Entity ent, bool playing)`**: Start/stop audio playback
- **`sound_get_playing(Entity ent)`**: Check if audio is currently playing
- **`sound_set_seek(Entity ent, int seek)`**: Set playback position
- **`sound_get_seek(Entity ent)`**: Get current playback position

### Playback Behavior
- **`sound_set_finish_destroy(Entity ent, bool finish_destroy)`**: Auto-destroy entity when audio finishes
- **`sound_get_finish_destroy(Entity ent)`**: Check auto-destroy setting
- **`sound_set_loop(Entity ent, bool loop)`**: Enable/disable looping
- **`sound_get_loop(Entity ent)`**: Check loop setting

### Volume Control
- **`sound_set_gain(Entity ent, Scalar gain)`**: Set audio volume/gain
- **`sound_get_gain(Entity ent)`**: Get current volume/gain

## System Management Functions (Internal)

These functions are not script-exported and handle system-level operations:

- **`sound_init()`**: Initialize the sound system
- **`sound_deinit()`**: Clean up and shut down sound system
- **`sound_update_all()`**: Update all sound entities (likely called per frame)
- **`sound_save_all(Store *s)`**: Serialize all sound data to storage
- **`sound_load_all(Store *s)`**: Deserialize sound data from storage

## Architecture Notes

- Uses entity-component pattern where sounds are components attached to entities
- Provides complete getter/setter pairs for all sound properties
- Supports both one-shot and looping audio playback
- Includes automatic cleanup functionality via `finish_destroy` flag
- Integrates with a serialization system for save/load functionality
- Designed for both C/C++ and scripting language access


### Related
- [Parent](../)

---
Hash: 417dd62a01a8826866ae63a4d9323d90df976ed06277dbf4ef02c2c2733a62ce
Generated: 2025-07-07T20:21:19-04:00
