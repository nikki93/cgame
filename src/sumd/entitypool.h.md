# Summary: entitypool.h

## Table of Contents

- [EntityPool Header - Memory Pool for Entity-Component System](#entitypool-header---memory-pool-for-entity-component-system)

This header file defines a generic memory pool system for managing entity components in what appears to be an entity-component-system (ECS) architecture.

## Core Data Structures

• **EntityPool** - Opaque struct representing a contiguous memory pool for storing entity components
• **EntityPoolElem** - Required base struct that must be embedded at the top of all pooled data structures
  - Contains an `Entity ent` field used as the key for lookups
  - Must be included in user-defined structs like: `struct Data { EntityPoolElem pool_elem; ... }`

## Key Dependencies

• **entity.h** - Provides `Entity` type and related functions like `entity_destroyed()` and `entity_get_save_filter()`
• **saveload.h** - Provides `Store` type for serialization functionality

## Memory Management Functions

• **entitypool_new_()** / **entitypool_new(type)** - Creates new pool with specified element size
• **entitypool_free()** - Destroys the pool
• **entitypool_clear()** - Removes all elements from pool

## Element Operations

• **entitypool_add()** - Adds element for given entity, returns pointer to new element
• **entitypool_remove()** - Removes element associated with entity
• **entitypool_get()** - Retrieves element by entity key (returns NULL if not found)

## Iteration and Access

• **Direct pointer iteration** - Elements are stored contiguously in memory:
  - `entitypool_begin()` - Returns pointer to first element
  - `entitypool_end()` - Returns one-past-end pointer
  - `entitypool_nth()` - Returns pointer to nth element (0-indexed)
• **entitypool_size()** - Returns number of elements in pool
• **entitypool_sort()** - Sorts elements using provided comparator function

## Utility Macros

• **entitypool_foreach(var, pool)** - Macro for iterating over all elements
  - Variable `var` must be pre-declared as pointer to element type
  - Elements visited in order of increasing index

• **entitypool_remove_destroyed(pool, func)** - Macro for cleanup operations
  - Iterates through pool and calls `func` on each destroyed entity
  - Typically used in `*_update_all()` functions

## Serialization Support

• **entitypool_elem_save()** / **entitypool_elem_load()** - Save/load individual elements
• **entitypool_save_foreach()** / **entitypool_load_foreach()** - Macros for bulk serialization
  - Respect entity save filtering
  - Create hierarchical store structure for organized data

## Important Design Considerations

• **Memory layout warning** - Elements are continuous in memory but may be relocated/shuffled
• **Iterator invalidation** - Adding/removing elements during iteration invalidates pointers
• **Required struct embedding** - All pooled data must include `EntityPoolElem` as first member
• **Usage examples** - References transform.c, sprite.c as implementation examples

This system provides efficient memory management for entity components while maintaining fast iteration and lookup capabilities essential for game engine performance.

## Sections

### EntityPool Header - Memory Pool for Entity-Component System
Source: [entitypool.h:1-127](../entitypool.h#L1-L127)

This header defines `EntityPool`, a memory-efficient container for managing collections of components in an entity-component system. The pool stores elements contiguously in memory and provides efficient iteration and lookup capabilities.

## Core Data Structures

• **`EntityPool`** - Opaque struct representing the memory pool container
• **`EntityPoolElem`** - Required header struct that must be placed at the beginning of all pooled elements
  - Contains `Entity ent` field as the key for the element
  - Serves as metadata managed by the EntityPool system

## Memory Management Functions

• **`entitypool_new_(size_t object_size)`** - Creates new pool with specified element size
• **`entitypool_new(type)`** - Macro wrapper that automatically calculates sizeof(type)
• **`entitypool_free(EntityPool *pool)`** - Deallocates the pool
• **`entitypool_add(EntityPool *pool, Entity ent)`** - Adds element, returns pointer to new element
• **`entitypool_remove(EntityPool *pool, Entity ent)`** - Removes element by entity key
• **`entitypool_get(EntityPool *pool, Entity ent)`** - Retrieves element pointer (NULL if not found)

## Iteration and Access Functions

• **`entitypool_begin(EntityPool *pool)`** - Returns pointer to first element
• **`entitypool_end(EntityPool *pool)`** - Returns one-past-end pointer
• **`entitypool_nth(EntityPool *pool, unsigned int n)`** - Returns pointer to nth element (0-indexed)
• **`entitypool_size(EntityPool *pool)`** - Returns number of elements
• **`entitypool_clear(EntityPool *pool)`** - Removes all elements

## Utility Functions

• **`entitypool_sort(EntityPool *pool, int (*compar)(const void *, const void *))`** - Sorts elements using qsort-style comparator

## Save/Load System

• **`entitypool_elem_save(EntityPool *pool, void *elem, Store *s)`** - Saves single element
• **`entitypool_elem_load(EntityPool *pool, void *elem, Store *s)`** - Loads single element
• Dependencies: `Store` type from `saveload.h`

## Utility Macros

• **`entitypool_remove_destroyed(pool, func)`** - Macro that iterates through pool and calls `func` on each destroyed entity
  - Uses `entity_destroyed()` to check entity status
  - Safely removes destroyed entities during iteration
  - Commonly used in `*_update_all()` functions

• **`entitypool_foreach(var, pool)`** - Iteration macro for clean syntax
  - `var` must be pre-declared pointer variable
  - Visits elements in order of increasing index
  - Warning: Do not add/remove elements during iteration

• **`entitypool_save_foreach(var, var_s, pool, n, s)`** - Save iteration macro
  - `var`: iteration variable
  - `var_s`: Store pointer for per-element stores
  - `pool`: EntityPool to save
  - `n`: name to save pool under
  - `s`: parent Store
  - Respects entity save filtering via `entity_get_save_filter()`

• **`entitypool_load_foreach(var, var_s, pool, n, s)`** - Load iteration macro with similar parameters

## Dependencies

• **`entity.h`** - Provides `Entity` type and `entity_destroyed()`, `entity_get_save_filter()` functions
• **`saveload.h`** - Provides `Store` type and store manipulation functions
• **`stddef.h`** - For `size_t` type

## Key Design Notes

• Elements are stored contiguously in memory for cache efficiency
• Elements may be relocated/shuffled, so pointers can become invalid
• All pooled structs must include `EntityPoolElem` as their first member
• The system is designed for entity-component architecture where components are stored in separate pools
• Examples referenced in `transform.c`, `sprite.c` for usage patterns


### Related
- [Parent](../)

---
Hash: 213c0d0c42623f982732ad6cc29ac17b82b6fad5bbbf5e61fe9c9dde88dbaabe
Generated: 2025-07-07T20:21:19-04:00
