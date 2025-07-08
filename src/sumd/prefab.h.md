# Summary: prefab.h

## Table of Contents

- [Prefab System Header - Entity Serialization Interface](#prefab-system-header---entity-serialization-interface)

This header file defines the interface for a prefab system that handles saving and loading collections of entities to/from files. The prefab system allows for serializing entity hierarchies with a designated root entity.

## Key Components Defined:

• **Script-exported functions** (exposed to scripting system):
  - `prefab_save(const char *filename, Entity root)` - Saves all filtered entities as a prefab file, using the specified `root` entity as the hierarchy root
  - `prefab_load(const char *filename)` - Loads a previously saved prefab file and returns the root `Entity` from the saved data

• **Internal C functions** (not script-exposed):
  - `prefab_save_all(Store *s)` - Saves all prefabs using the provided `Store` object
  - `prefab_load_all(Store *s)` - Loads all prefabs using the provided `Store` object

## Dependencies Referenced:

• **entity.h** - Provides the `Entity` type used throughout the prefab system
• **saveload.h** - Provides the `Store` type used for bulk save/load operations
• **script_export.h** - Provides the `SCRIPT()` macro and `EXPORT` keyword for making functions available to the scripting system

## System Architecture:

• The prefab system operates on two levels:
  - Individual prefab operations (`prefab_save`/`prefab_load`) for single prefab files
  - Bulk operations (`prefab_save_all`/`prefab_load_all`) for managing multiple prefabs through a Store system

• The save operation filters entities before serialization, suggesting selective saving based on some criteria
• The load operation reconstructs the entity hierarchy and returns the root entity for further manipulation

## Sections

### Prefab System Header - Entity Serialization Interface
Source: [prefab.h:1-21](../prefab.h#L1-L21)

This header file defines the prefab system interface for saving and loading collections of entities to/from files. The prefab system allows for serializing entity hierarchies with a designated root entity.

## Key Definitions

**Exported Script Functions (via SCRIPT macro):**
- `prefab_save(const char *filename, Entity root)` - Saves all filtered entities as a prefab file with the specified root entity
- `prefab_load(const char *filename)` - Loads a previously saved prefab file and returns the root entity

**Internal Functions:**
- `prefab_save_all(Store *s)` - Saves all prefabs to a data store
- `prefab_load_all(Store *s)` - Loads all prefabs from a data store

## Dependencies

The header includes and depends on:
- `entity.h` - Provides the `Entity` type used in function signatures
- `saveload.h` - Provides the `Store` type for bulk save/load operations
- `script_export.h` - Provides the `SCRIPT` macro for exposing functions to scripting and the `EXPORT` keyword

## System Architecture

• **Two-tier approach**: The system provides both individual prefab file operations and bulk store operations
• **Root-based hierarchy**: Prefabs are organized around a root entity, suggesting a hierarchical entity structure
• **Filtering mechanism**: The save function mentions "filtered entities," indicating selective serialization based on some criteria
• **Script integration**: Core functions are exposed to scripting systems via the SCRIPT macro wrapper

The prefab system appears to be designed for scene management, allowing developers to save and restore complex entity arrangements for reuse across different contexts.


### Related
- [Parent](../)

---
Hash: 00375d378865e157f56b78e1752008fc6208d9aee6848f12f4c16fceab4c07a5
Generated: 2025-07-07T20:21:19-04:00
