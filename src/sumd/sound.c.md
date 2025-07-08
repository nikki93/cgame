# Summary: sound.c

## Table of Contents

- [Sound System Implementation and Entity Management](#sound-system-implementation-and-entity-management)
- [Sound System State Loading Function](#sound-system-state-loading-function)

This section implements a complete sound system for managing audio playback in a game engine using the Gorilla Audio library. The system provides entity-based sound management with features like looping, seeking, volume control, and automatic cleanup.

## Core Data Structures

• **Sound struct**: Main sound entity containing:
  - `EntityPoolElem pool_elem`: Entity pool management
  - `char *path`: File path to sound file
  - `ga_Handle *handle`: Gorilla Audio handle for playback control
  - `gau_SampleSourceLoop *loop_src`: Loop source for repeat functionality
  - `bool finish_destroy`: Flag to auto-destroy when playback finishes
  - `bool loop`: Loop enable/disable flag

• **Static global variables**:
  - `EntityPool *pool`: Entity pool for managing Sound instances
  - `gau_Manager *mgr`: Gorilla Audio manager
  - `ga_Mixer *mixer`: Audio mixer
  - `ga_StreamManager *stream_mgr`: Stream manager for file playback

## Internal Helper Functions

• **_release(Sound *sound)**: Cleanup function that:
  - Frees the path string
  - Destroys the Gorilla Audio handle
  - Sets pointers to NULL

• **_format(const char *path)**: Format detection that:
  - Scans path string for last dot character
  - Returns file extension as format string
  - Errors if no extension found

• **_update_loop(Sound *sound)**: Loop state synchronization that:
  - Sets infinite loop (-1) if sound->loop is true
  - Clears loop if sound->loop is false

• **_set_path(Sound *sound, const char *path)**: Core sound loading function that:
  - Saves current playing state
  - Determines audio format from file extension
  - Creates appropriate handle (buffered for OGG, direct for others)
  - Releases old resources and sets new path
  - Updates loop state
  - Restores playing state if previously playing

## Public API Functions

### Entity Management
• **sound_add(Entity ent)**: Adds sound component to entity with default "default.wav" file
• **sound_remove(Entity ent)**: Removes sound component and cleans up resources
• **sound_has(Entity ent)**: Checks if entity has sound component

### Sound Properties
• **sound_set_path/sound_get_path**: Set/get sound file path
• **sound_set_playing/sound_get_playing**: Control/query playback state
• **sound_set_seek/sound_get_seek**: Control/query playback position
• **sound_set_finish_destroy/sound_get_finish_destroy**: Control auto-destruction behavior
• **sound_set_loop/sound_get_loop**: Control/query loop behavior
• **sound_set_gain/sound_get_gain**: Control/query volume level

## System Management Functions

• **sound_init()**: System initialization that:
  - Initializes Gorilla Audio (`gc_initialize`)
  - Creates manager, mixer, and stream manager
  - Creates entity pool

• **sound_deinit()**: System cleanup that:
  - Releases all sound resources
  - Frees entity pool
  - Destroys Gorilla Audio manager
  - Shuts down Gorilla Audio

• **sound_update_all()**: Per-frame update that:
  - Destroys finished sounds with `finish_destroy` flag set
  - Removes destroyed entities from pool
  - Updates Gorilla Audio manager

## Persistence System

• **sound_save_all(Store *s)**: Serializes all sound state including:
  - File paths and configuration flags
  - Current playing state and seek position
  - Volume gain settings

• **sound_load_all(Store *s)**: Deserializes sound state and:
  - Recreates sound handles from saved paths
  - Restores playback state and position
  - Reapplies volume settings

## Key Dependencies

• **Gorilla Audio library**: `ga.h` and `gau.h` for audio playback
• **entitypool**: For entity-component system management
• **dirs**: For `data_path()` function to locate default sounds
• **error**: For error handling and assertions
• **Store system**: For save/load functionality

The system supports OGG and WAV formats, with OGG files using buffered streaming and other formats loaded directly into memory.

## Sections

### Sound System Implementation and Entity Management
Source: [sound.c:1-303](../sound.c#L1-L303)

This section implements a comprehensive sound system using the Gorilla Audio library for managing audio playback in an entity-component system architecture.

## Core Data Structure

• **Sound struct**: Main component containing:
  - `EntityPoolElem pool_elem`: Entity pool integration
  - `char *path`: File path to sound resource
  - `ga_Handle *handle`: Gorilla Audio handle for playback control
  - `gau_SampleSourceLoop *loop_src`: Loop source for repeating audio
  - `bool finish_destroy`: Flag to auto-destroy entity when sound finishes
  - `bool loop`: Whether sound should loop continuously

## Global System State

• **Static variables** managing system-wide audio state:
  - `EntityPool *pool`: Entity pool for Sound components
  - `gau_Manager *mgr`: Gorilla Audio manager
  - `ga_Mixer *mixer`: Audio mixer for combining sounds
  - `ga_StreamManager *stream_mgr`: Streaming manager for buffered audio

## Internal Helper Functions

• **_release(Sound *sound)**: Cleanup function that:
  - Frees the path string
  - Destroys the Gorilla Audio handle
  - Sets pointers to NULL

• **_format(const char *path)**: File format detection that:
  - Scans path string for last dot character
  - Returns extension after the dot
  - Throws error if no extension found

• **_update_loop(Sound *sound)**: Loop state synchronization that:
  - Sets infinite loop (-1) if sound->loop is true
  - Clears loop if sound->loop is false

• **_set_path(Sound *sound, const char *path)**: Core audio loading function that:
  - Checks if current sound is playing
  - Determines file format from extension
  - Loads audio using appropriate method:
    - OGG files: `gau_create_handle_buffered_file()`
    - Other formats: `gau_load_sound_file()` then `gau_create_handle_sound()`
  - Releases old resources
  - Copies new path string (if different)
  - Updates loop configuration
  - Resumes playback if previously playing

## Public API Functions

### Entity Management
• **sound_add(Entity ent)**: Creates new Sound component with default "default.wav" file
• **sound_remove(Entity ent)**: Removes Sound component and releases resources
• **sound_has(Entity ent)**: Checks if entity has Sound component

### Audio Properties
• **sound_set_path/sound_get_path**: Sets/gets audio file path
• **sound_set_playing/sound_get_playing**: Controls/queries playback state
• **sound_set_seek/sound_get_seek**: Controls/queries playback position
• **sound_set_loop/sound_get_loop**: Controls/queries loop behavior
• **sound_set_gain/sound_get_gain**: Controls/queries volume level
• **sound_set_finish_destroy/sound_get_finish_destroy**: Controls auto-destruction behavior

## System Management Functions

• **sound_init()**: System initialization that:
  - Initializes Gorilla Audio with `gc_initialize()`
  - Creates manager, mixer, and stream manager
  - Initializes entity pool

• **sound_deinit()**: System cleanup that:
  - Releases all Sound components
  - Destroys entity pool and audio manager
  - Shuts down Gorilla Audio

• **sound_update_all()**: Per-frame update that:
  - Identifies finished sounds with `finish_destroy` flag set
  - Destroys corresponding entities
  - Removes destroyed entities from pool
  - Updates Gorilla Audio manager

• **sound_save_all(Store *s)**: Serialization function that saves:
  - File paths
  - Finish destroy and loop flags
  - Current playing state and seek position
  - Gain/volume settings

## Key Dependencies

• **External libraries**: Gorilla Audio (`gorilla/ga.h`, `gorilla/gau.h`)
• **Internal modules**: 
  - `entitypool.h`: Entity-component system integration
  - `dirs.h`: File path utilities (used in `data_path()`)
  - `error.h`: Error handling and assertions
  - `input.h`, `array.h`: Additional system dependencies

## Audio Format Support

• **Supported formats**: OGG (streamed), other formats loaded via `gau_load_sound_file()`
• **Loading strategy**: OGG files use buffered streaming, others are fully loaded into memory

### Sound System State Loading Function
Source: [sound.c:308-334](../sound.c#L308-L334)

This function loads and restores the complete state of all sound objects from a data store, including their playback properties and audio settings.

## Function Definition
- **Function name**: `sound_load_all`
- **Parameters**: `Store *s` - the data store to load from
- **Return type**: `void`

## Core Functionality
The function performs a complete restoration of the sound system state by:

• **Store Navigation**: 
  - Loads a child store named "sound" from the input store `s`
  - Uses `store_child_load` to access the sound data section

• **Sound Object Iteration**:
  - Iterates through all sound objects using `entitypool_load_foreach`
  - Accesses each sound from a "pool" section within the sound store
  - Variables: `sound` (current Sound object), `sound_s` (current sound's store), `pool` (entity pool reference)

• **Per-Sound State Restoration** (for each sound object):
  - **Path Loading**: Loads file path using `string_load` with key "path"
  - **Property Loading**:
    - `finish_destroy` boolean (default: false)
    - `loop` boolean (default: false)
  - **Audio Handle Setup**:
    - Initializes `sound->path` and `sound->handle` to NULL
    - Calls `_set_path(sound, path)` to properly configure the sound path and handle
  - **Playback State**:
    - Loads `playing` boolean (default: false)
    - If playing is true, starts playback via `ga_handle_play(sound->handle)`
  - **Audio Parameters**:
    - Loads `seek` position (default: 0) and applies with `ga_handle_seek`
    - Loads `gain` value (default: 1) and applies with `ga_handle_setParamf` using `GA_HANDLE_PARAM_GAIN`

## Key Dependencies
- **Data Types**: `Store`, `Sound`, `Scalar`
- **Loading Functions**: `store_child_load`, `entitypool_load_foreach`, `string_load`, `bool_load`, `int_load`, `scalar_load`
- **Internal Function**: `_set_path` (for configuring sound path and handle)
- **Audio Library Functions**: `ga_handle_play`, `ga_handle_seek`, `ga_handle_setParamf`
- **Audio Constants**: `GA_HANDLE_PARAM_GAIN`

## Local Variables
- `t`, `sound_s`: Store pointers for navigation
- `sound`: Current Sound object being processed
- `seek`: Integer for playback position
- `playing`: Boolean for playback state
- `path`: String for sound file path
- `gain`: Scalar for audio volume level


### Related
- [Parent](../)

---
Hash: 3778ef62d02212a3f953717a717638ff94567805fad93e36ad5d59b245b263f3
Generated: 2025-07-07T20:21:19-04:00
