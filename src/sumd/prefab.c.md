# Summary: prefab.c

## Table of Contents

- [Prefab System Implementation](#prefab-system-implementation)

This section implements a prefab system that allows saving and loading entity hierarchies to/from files. The system provides functionality to serialize entity data and restore it later.

## Key Components

### Static Variables
- `saved_root`: A static Entity variable that temporarily stores the root entity during save/load operations

### Public Functions

- **`prefab_save(const char *filename, Entity root)`**
  - Saves an entity hierarchy to a file
  - Steps:
    1. Sets `saved_root` to the provided root entity
    2. Opens a new store using `store_open()`
    3. Calls `system_save_all()` to serialize all system data
    4. Writes the store data to the specified filename using `store_write_file()`
    5. Closes the store
    6. Resets `saved_root` to `entity_nil`

- **`prefab_load(const char *filename)`**
  - Loads an entity hierarchy from a file
  - Steps:
    1. Opens the specified file as a store using `store_open_file()`
    2. Calls `system_load_all()` to deserialize all system data
    3. Closes the store
    4. Retrieves the root entity from `saved_root`
    5. Resets `saved_root` to `entity_nil`
    6. Returns the loaded root entity

- **`prefab_save_all(Store *s)`**
  - Serializes prefab-specific data to a store
  - Creates a child store section named "prefab"
  - Saves the `saved_root` entity with the key "saved_root"

- **`prefab_load_all(Store *s)`**
  - Deserializes prefab-specific data from a store
  - Loads from a child store section named "prefab"
  - Restores the `saved_root` entity using the key "saved_root"

## Dependencies

### External Headers
- `prefab.h`: Header file for this module
- `system.h`: Provides system-wide save/load functionality

### External Functions Referenced
- `store_open()`, `store_open_file()`, `store_write_file()`, `store_close()`: Store management functions
- `store_child_save()`, `store_child_load()`: Hierarchical store operations
- `system_save_all()`, `system_load_all()`: System-wide serialization functions
- `entity_save()`, `entity_load()`: Entity serialization functions

### External Constants
- `entity_nil`: Represents a null/empty entity value

The prefab system acts as a high-level interface for entity serialization, coordinating between the store system and the entity system to provide complete save/load functionality for entity hierarchies.

## Sections

### Prefab System Implementation
Source: [prefab.c:1-45](../prefab.c#L1-L45)

This section implements a prefab system for saving and loading entity hierarchies to/from files. The implementation uses a global state variable to track the root entity during serialization operations.

## Global State
- **`saved_root`**: A static Entity variable that temporarily stores the root entity during save/load operations

## Core Functions

### **`prefab_save(const char *filename, Entity root)`**
Saves an entity hierarchy to a file:
- Sets `saved_root` to the provided `root` entity
- Opens a new store using `store_open()`
- Calls `system_save_all(s)` to serialize all systems
- Writes the store to the specified filename using `store_write_file(s, filename)`
- Closes the store and resets `saved_root` to `entity_nil`

### **`prefab_load(const char *filename)`**
Loads an entity hierarchy from a file:
- Opens the specified file using `store_open_file(filename)`
- Calls `system_load_all(s)` to deserialize all systems
- Closes the store
- Retrieves the root entity from `saved_root`
- Resets `saved_root` to `entity_nil`
- Returns the loaded root entity

### **`prefab_save_all(Store *s)`**
Handles prefab-specific data during system-wide saves:
- Creates a child store section named "prefab"
- If successful, saves the `saved_root` entity with the key "saved_root"

### **`prefab_load_all(Store *s)`**
Handles prefab-specific data during system-wide loads:
- Attempts to load a child store section named "prefab"
- If successful, loads the "saved_root" entity with `entity_nil` as default value

## Key Dependencies
- **`Store`**: Used for serialization operations
- **`Entity`**: The entity type being saved/loaded
- **`system_save_all()`** and **`system_load_all()`**: System-wide serialization functions
- **`store_*()` functions**: File I/O and hierarchical storage operations
- **`entity_save()`** and **`entity_load()`**: Entity-specific serialization functions
- **`entity_nil`**: Null/empty entity constant

## Design Notes
- The system uses a global variable approach to communicate the root entity between the main prefab functions and the system-wide save/load callbacks
- The `prefab_save_all()` and `prefab_load_all()` functions appear to be callbacks used by the broader system serialization framework
- Error handling relies on the return values of `store_child_save()` and `store_child_load()` functions


### Related
- [Parent](../)

---
Hash: 666a18c37071a63f4b80259b3ddcf320a406dbf20fc906834dd1d2b1b9d6f9df
Generated: 2025-07-07T20:21:19-04:00
