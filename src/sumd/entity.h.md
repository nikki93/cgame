# Summary: entity.h

## Table of Contents

- [Entity System Header - Core Entity Management Interface](#entity-system-header---core-entity-management-interface)

This header file defines the core entity system for managing unique entity identifiers and their lifecycle. It provides a component-based architecture foundation where entities are lightweight ID containers.

## Core Data Structures

- **`Entity`**: A struct containing a single `unsigned int id` field representing a unique entity identifier
- **`entity_nil`**: A special Entity value that represents no valid entity (exported constant)

## Entity Lifecycle Management

- **`entity_create()`**: Allocates and returns a new unused Entity ID
- **`entity_destroy(Entity ent)`**: Releases a specific Entity ID for reuse
- **`entity_destroy_all()`**: Destroys all existing entities
- **`entity_destroyed(Entity ent)`**: Checks if an entity has been destroyed

## Entity Operations

- **`entity_eq(Entity e, Entity f)`**: Compares two entities for equality
  - Implemented as a C macro: `((e).id == (f).id)`

## Save/Load System Integration

The entity system integrates with a save/load system through multiple mechanisms:

### Save Filtering
- **`entity_set_save_filter(Entity ent, bool filter)`**: Marks entities for selective saving
- **`entity_get_save_filter(Entity ent)`**: Retrieves save filter status
- **`entity_clear_save_filters()`**: Removes all save filters
- **Save filtering logic**: When any entity has save filter set to true, only entities with true filters are saved; entities with false filters are excluded

### Serialization Functions
- **`entity_save(Entity *ent, const char *n, Store *s)`**: Saves entity ID data
- **`entity_load(Entity *ent, const char *n, Entity d, Store *s)`**: Loads entity ID data with default fallback
- **`_entity_resolve_saved_id(unsigned int id)`**: Internal function for resolving saved IDs during merge operations

### Bulk Operations
- **`entity_save_all(Store *s)`**: Saves all entities to storage
- **`entity_load_all(Store *s)`**: Loads all entities from storage
- **`entity_load_all_begin()`**: Initiates bulk loading process
- **`entity_load_all_end()`**: Finalizes bulk loading process

## System Management

- **`entity_init()`**: Initializes the entity system
- **`entity_deinit()`**: Cleans up the entity system
- **`entity_update_all()`**: Updates all entities (likely calls update logic for entity components)

## Dependencies

- **`saveload.h`**: Provides `Store` type for persistence operations
- **`script_export.h`**: Provides `SCRIPT()` and `EXPORT` macros for scripting language integration
- **`<stdbool.h>`**: Standard boolean type support

## Script Integration

The entire public API is wrapped in a `SCRIPT(entity, ...)` macro, indicating this system is exposed to a scripting language with all exported functions available for script access.

## Sections

### Entity System Header - Core Entity Management Interface
Source: [entity.h:1-52](../entity.h#L1-L52)

This header file defines the core entity management system for what appears to be a game engine or similar system. The file provides both a scripting interface and internal C functions for managing entities.

## Core Entity Structure
- **Entity**: A struct containing only an `unsigned int id` field
- **entity_nil**: A special constant representing an invalid/null entity value

## Entity Lifecycle Management
- **entity_create()**: Allocates and returns a new unique entity ID
- **entity_destroy(Entity ent)**: Releases a specific entity ID for reuse
- **entity_destroy_all()**: Destroys all existing entities
- **entity_destroyed(Entity ent)**: Checks if an entity has been destroyed
- **entity_eq(Entity e, Entity f)**: Compares two entities for equality (implemented as macro `((e).id == (f).id)`)

## Save/Load System
The entity system includes a sophisticated save/load mechanism:
- **Save filtering**: 
  - `entity_set_save_filter(Entity ent, bool filter)`: Controls whether specific entities are saved
  - `entity_get_save_filter(Entity ent)`: Retrieves save filter status
  - `entity_clear_save_filters()`: Resets all save filters
  - When any entity has save filter set to true, only filtered entities are saved
- **Individual entity save/load**:
  - `entity_save(Entity *ent, const char *n, Store *s)`: Saves a single entity
  - `entity_load(Entity *ent, const char *n, Entity d, Store *s)`: Loads a single entity with default value
- **Bulk operations**:
  - `entity_save_all(Store *s)`: Saves all entities to storage
  - `entity_load_all(Store *s)`: Loads all entities from storage
  - `entity_load_all_begin()` and `entity_load_all_end()`: Frame the bulk loading process
- **Internal utility**:
  - `_entity_resolve_saved_id(unsigned int id)`: Resolves saved IDs during loading (internal use)

## System Management Functions
- **entity_init()**: Initializes the entity system
- **entity_deinit()**: Cleans up the entity system
- **entity_update_all()**: Updates all entities (likely calls update logic for all active entities)

## Dependencies
- **saveload.h**: Provides the `Store` type used for persistence
- **script_export.h**: Provides the `SCRIPT()` and `EXPORT` macros for exposing functionality to scripting
- **stdbool.h**: Standard boolean type support

## Scripting Integration
The file uses a `SCRIPT()` macro to define the public interface that will be exposed to scripting languages, wrapping most of the entity management functionality for external use.


### Related
- [Parent](../)

---
Hash: 1a97ae80b05d05663597528bb2ded430330f8919063253429d7bd38410388bd8
Generated: 2025-07-07T20:21:19-04:00
