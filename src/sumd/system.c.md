# Summary: system.c

## Table of Contents

- [Game Engine System Manager - Core Initialization and Control Flow](#game-engine-system-manager---core-initialization-and-control-flow)

This file implements the central system manager that coordinates all subsystems of the game engine. It serves as the main orchestrator for initialization, update cycles, rendering, and persistence operations.

## Key Functions Defined

• **system_init()** - Initializes all engine subsystems in proper dependency order
• **system_deinit()** - Cleans up all subsystems in reverse order
• **system_update_all()** - Executes the main game loop update cycle
• **system_draw_all()** - Handles all rendering operations
• **system_save_all(Store *s)** - Saves all subsystem state to storage
• **system_load_all(Store *s)** - Loads all subsystem state from storage

## Input Event Forwarding

The system implements input event delegation through static callback functions:

• **_key_down(KeyCode key)** - Forwards keyboard press events to GUI and script systems
• **_key_up(KeyCode key)** - Forwards keyboard release events to GUI and script systems
• **_char_down(unsigned int c)** - Forwards character input to GUI system
• **_mouse_down(MouseCode mouse)** - Forwards mouse press events to GUI and script systems
• **_mouse_up(MouseCode mouse)** - Forwards mouse release events to GUI and script systems
• **_mouse_move(Vec2 pos)** - Forwards mouse movement to script system
• **_scroll(Vec2 scroll)** - Forwards scroll events to script system

## Subsystem Dependencies

The system manages these subsystems in initialization order:
1. **input** - Input handling system
2. **entity** - Entity management system
3. **transform** - Transform/positioning system
4. **camera** - Camera system
5. **texture** - Texture management
6. **sprite** - Sprite rendering
7. **gui** - GUI system
8. **console** - Console/debugging
9. **sound** - Audio system
10. **physics** - Physics simulation
11. **edit** - Editor functionality
12. **script** - Scripting system

## Main Update Loop Sequence

**system_update_all()** executes subsystem updates in this order:
1. **edit_clear()** - Clear editor state
2. **timing_update()** - Update timing/frame information
3. **texture_update()** and **scratch_update()** - Update texture and scratch memory systems
4. **script_update_all()** - Execute script updates
5. **keyboard_controlled_update_all()** - Update keyboard-controlled entities
6. **physics_update_all()** - Update physics simulation
7. **transform_update_all()** - Update transforms
8. **camera_update_all()** - Update cameras
9. **gui_update_all()** - Update GUI elements
10. **sprite_update_all()** - Update sprites
11. **sound_update_all()** - Update audio
12. **edit_update_all()** - Update editor
13. **script_post_update_all()** and **physics_post_update_all()** - Post-update phases
14. **entity_update_all()** - Final entity updates
15. **gui_event_clear()** - Clear GUI events

## Rendering Pipeline

**system_draw_all()** renders in this order:
1. **script_draw_all()** - Script-driven rendering
2. **sprite_draw_all()** - Sprite rendering
3. **edit_draw_all()** - Editor overlay rendering
4. **physics_draw_all()** - Physics debug rendering
5. **gui_draw_all()** - GUI rendering (topmost layer)

## Save/Load System

The **_saveload_all()** helper function ensures consistent save/load ordering:
• Uses a macro pattern to call either save or load functions based on a boolean flag
• Maintains strict ordering: entity, prefab, timing, transform, camera, sprite, physics, gui, edit, sound, keyboard_controlled, script
• Wraps operations with **entity_load_all_begin()** and **entity_load_all_end()** for proper entity lifecycle management

## External Dependencies

• References **Store** type for persistence operations
• Uses **Vec2** type for 2D vector operations (mouse positions, scroll)
• Depends on **KeyCode** and **MouseCode** enums for input handling
• Includes test system **keyboard_controlled** for keyboard-controlled entities

## Sections

### Game Engine System Manager - Core Initialization and Control Flow
Source: [system.c:1-168](../system.c#L1-L168)

This section implements the main system manager for a game engine, providing centralized control over initialization, updates, rendering, and serialization of all engine subsystems.

## Top-Level Functions Defined

• **system_init()** - Initializes all engine subsystems and sets up input callbacks
• **system_deinit()** - Cleanly shuts down all subsystems in reverse order
• **system_update_all()** - Orchestrates the main game loop update cycle
• **system_draw_all()** - Handles rendering operations for all visual systems
• **system_save_all(Store *s)** - Saves state of all systems to storage
• **system_load_all(Store *s)** - Loads state of all systems from storage

## Input Event Handlers (Static Functions)

• **_key_down(KeyCode key)** - Dispatches keyboard press events to GUI and script systems
• **_key_up(KeyCode key)** - Dispatches keyboard release events to GUI and script systems
• **_char_down(unsigned int c)** - Handles character input for GUI system
• **_mouse_down(MouseCode mouse)** - Dispatches mouse press events to GUI and script systems
• **_mouse_up(MouseCode mouse)** - Dispatches mouse release events to GUI and script systems
• **_mouse_move(Vec2 pos)** - Handles mouse movement for script system
• **_scroll(Vec2 scroll)** - Handles scroll wheel input for script system

## Key Dependencies and Subsystems

The system manager coordinates 15+ engine subsystems:
• **Core systems**: entity, transform, timing, input, scratch
• **Rendering**: camera, texture, sprite, gui
• **Audio**: sound
• **Physics**: physics
• **Scripting**: script, prefab
• **Development tools**: console, edit
• **Game-specific**: keyboard_controlled (test system)

## Initialization Flow (system_init)

1. Initialize core input system first
2. Initialize entity management
3. Initialize transform, camera, and rendering systems (texture, sprite)
4. Initialize UI systems (gui, console)
5. Initialize audio (sound)
6. Initialize physics simulation
7. Initialize development tools (edit)
8. Initialize scripting system last
9. Register all input callback functions with the input system

## Update Loop Flow (system_update_all)

1. **Pre-update**: Clear edit system, update timing and textures
2. **Core updates**: Update scratch memory, scripts, keyboard controls
3. **Physics**: Update physics simulation and transforms
4. **Rendering prep**: Update camera and sprite systems
5. **UI updates**: Update GUI and sound systems
6. **Development**: Update edit system
7. **Post-processing**: Run script and physics post-updates
8. **Cleanup**: Update entities, clear GUI events

## Serialization System

• **_saveload_all(void *s, bool save)** - Internal helper using macro-based approach
  - Uses preprocessor macro to generate save/load calls for each system
  - Ensures consistent ordering between save and load operations
  - Wraps operations with `entity_load_all_begin()` and `entity_load_all_end()`

The serialization covers all major systems including entities, prefabs, timing, transforms, cameras, sprites, physics, GUI, editing tools, sound, keyboard controls, and scripts.


### Related
- [Parent](../)

---
Hash: 8bf15aa74b2568560fff47c5d3f5dd2e59b2e60942a26bba3c7a1067ab6abeaa
Generated: 2025-07-07T20:21:19-04:00
