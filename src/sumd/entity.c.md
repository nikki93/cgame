# Summary: entity.c

## Table of Contents

- [Entity Management System Implementation](#entity-management-system-implementation)

This section implements a comprehensive entity management system that handles the complete lifecycle of entities from creation through destruction, including save/load functionality and ID reuse optimization.

## Core Data Structures

- **Entity**: Base entity type with an `id` field
- **DestroyEntry**: Structure containing an `Entity ent` and `unsigned int pass` for tracking destruction phases
- **ExistsPoolElem**: Wrapper structure containing `EntityPoolElem pool_elem` for the entity pool
- **SaveFilter**: Enum with values `SF_SAVE`, `SF_NO_SAVE`, `SF_UNSET` for controlling save behavior

## Global State Management

- **entity_nil**: Global null entity constant with ID 0
- **counter**: Static counter starting at 1 for generating new entity IDs
- **exists_pool**: EntityPool tracking all currently existing entities
- **destroyed_map**: EntityMap tracking which entities are marked for destruction
- **destroyed**: Array of DestroyEntry objects for entities in destruction process
- **unused_map**: EntityMap tracking entities available for ID reuse
- **unused**: Array of Entity IDs that can be reused after removal
- **load_map**: EntityMap for mapping saved IDs to real IDs during loading
- **save_filter_map**: EntityMap controlling which entities should be saved
- **save_filter_default**: Default save filter behavior (initially SF_SAVE)

## Entity Lifecycle Management

The system implements a three-state lifecycle with delayed destruction:

1. **Doesn't exist/removed state**:
   - `destroyed_map[ent] = false`
   - Entity not in destroyed array
   - Transitions to "exists" on `entity_create()`

2. **Exists state**:
   - `destroyed_map[ent] = false` 
   - Entity not in destroyed array
   - Transitions to "destroyed" on `entity_destroy()`

3. **Destroyed state**:
   - `destroyed_map[ent] = true`
   - Entity entry `{ent, pass}` added to destroyed array
   - After 2 update passes, `_remove()` called to return to state 1

## Key Functions

### Entity Creation and Destruction
- **entity_create()**: Creates new entity by calling `_generate_id()`
- **_generate_id()**: 
  - Reuses unused IDs from unused array if available
  - Otherwise increments counter for new ID
  - Adds entity to exists_pool
- **entity_destroy()**: Marks entity for destruction with 2-pass delay
- **entity_destroy_all()**: Destroys all entities in exists_pool
- **_remove()**: Actually removes entity and moves ID to unused list for reuse

### Save/Load System
- **entity_set_save_filter()**: Controls whether specific entities are saved
- **entity_get_save_filter()**: Checks if entity should be saved
- **entity_clear_save_filters()**: Resets all save filters to default
- **entity_save()**: Serializes entity to Store, validates save filter
- **entity_load()**: Deserializes entity from Store
- **_entity_resolve_saved_id()**: Maps saved IDs to real IDs during loading
- **entity_load_all_begin()**: Initializes load_map for loading session
- **entity_load_all_end()**: Cleans up load_map and save filters

### System Management
- **entity_init()**: Initializes all global data structures
- **entity_deinit()**: Frees all allocated memory
- **entity_update_all()**: Processes destroyed entities, implementing 2-pass removal
- **entity_eq()**: Compares two entities for equality by ID
- **entity_destroyed()**: Checks if entity is marked for destruction

### Bulk Save/Load Operations
- **entity_save_all()**: Saves complete entity system state including exists_pool and destroyed array
- **entity_load_all()**: Loads complete entity system state from saved data

## Dependencies
The system depends on several external modules:
- **saveload.h**: For Store-based serialization
- **entitymap.h**: For EntityMap data structure
- **array.h**: For dynamic Array implementation  
- **entitypool.h**: For EntityPool management
- **error.h**: For error handling and assertions

## Key Algorithms

**Two-Pass Destruction**: Entities marked for destruction remain in the destroyed array for 2 update cycles before actual removal, allowing other systems to clean up references.

**ID Reuse Optimization**: Destroyed entity IDs are recycled through the unused array to prevent unbounded ID growth.

**Save/Load ID Mapping**: During loading, a temporary mapping translates saved IDs to current runtime IDs, allowing saved entity references to be properly restored.

## Sections

### Entity Management System Implementation
Source: [entity.c:1-301](../entity.c#L1-L301)

This section implements a comprehensive entity management system that handles entity creation, destruction, lifecycle management, and save/load functionality.

## Core Data Structures

**Entity Types:**
- `Entity`: Basic entity type with an `id` field
- `entity_nil`: Global constant representing a null/invalid entity (id = 0)
- `DestroyEntry`: Contains an `Entity` and an unsigned int `pass` field for delayed destruction
- `ExistsPoolElem`: Wrapper struct containing an `EntityPoolElem pool_elem` field

**Static Storage Systems:**
- `exists_pool`: EntityPool tracking all currently existing entities
- `destroyed_map`: EntityMap indicating whether an entity is marked for destruction
- `destroyed`: Array of `DestroyEntry` objects for entities pending removal
- `unused_map`: EntityMap tracking whether an entity ID is in the unused array
- `unused`: Array of reusable entity IDs
- `load_map`: EntityMap for mapping saved entity IDs to real IDs during loading

## Entity Lifecycle Management

The system implements a three-state lifecycle:

1. **Doesn't exist/removed state:**
   - `destroyed_map[ent] = false`
   - Entity not in destroyed array
   - Transitions to "exists" via `entity_create()`

2. **Exists state:**
   - `destroyed_map[ent] = false` 
   - Entity not in destroyed array
   - Transitions to "destroyed" via `entity_destroy()`

3. **Destroyed state:**
   - `destroyed_map[ent] = true`
   - `{ent, pass}` entry in destroyed array
   - After 2 update passes, transitions back to "doesn't exist" via `_remove()`

## Key Functions

**Entity Creation:**
- `entity_create()`: Creates new entity by calling `_generate_id()`
- `_generate_id()`: Reuses unused IDs when available, otherwise increments global counter

**Entity Destruction:**
- `entity_destroy(Entity ent)`: Marks entity for delayed destruction (2-pass system)
- `entity_destroy_all()`: Destroys all existing entities
- `_remove(Entity ent)`: Actually removes entity and adds ID to unused pool
- `entity_destroyed(Entity ent)`: Checks if entity is marked for destruction

**Update System:**
- `entity_update_all()`: Processes destroyed entities array, incrementing pass counters and removing entities after 2 passes

## Save/Load System

**Save Filtering:**
- `SaveFilter` enum: `SF_SAVE`, `SF_NO_SAVE`, `SF_UNSET`
- `save_filter_map`: EntityMap storing save filter preferences
- `save_filter_default`: Global default filter setting
- `entity_set_save_filter(Entity ent, bool filter)`: Sets whether entity should be saved
- `entity_get_save_filter(Entity ent)`: Returns save filter status
- `entity_clear_save_filters()`: Resets all save filters

**Save/Load Operations:**
- `entity_save(Entity *ent, const char *n, Store *s)`: Saves individual entity
- `entity_load(Entity *ent, const char *n, Entity d, Store *s)`: Loads individual entity
- `entity_save_all(Store *s)`: Saves entire entity system state
- `entity_load_all(Store *s)`: Loads entire entity system state
- `entity_load_all_begin()`: Initializes load_map for loading session
- `entity_load_all_end()`: Cleans up load_map after loading
- `_entity_resolve_saved_id(unsigned int id)`: Maps saved IDs to real IDs during loading

## System Management

**Initialization/Cleanup:**
- `entity_init()`: Initializes all entity system data structures
- `entity_deinit()`: Frees all entity system data structures

**Utility:**
- `entity_eq(Entity e, Entity f)`: Compares two entities for equality

## Dependencies

The system depends on several external modules:
- `entitypool.h`: For EntityPool operations
- `entitymap.h`: For EntityMap operations  
- `array.h`: For Array operations
- `saveload.h`: For Store operations and save/load utilities
- `error.h`: For error handling and assertions


### Related
- [Parent](../)

---
Hash: 0cc135c4e47745673059147e4280c3e551216b273a0fac9d8c02694d61afbaaf
Generated: 2025-07-07T20:21:19-04:00
