# Summary: fs.h

## Table of Contents

- [File System Directory Operations Header](#file-system-directory-operations-header)

This header file (`fs.h`) defines a C API for directory traversal operations that are exported to a scripting system. The file establishes a simple interface for opening directories, iterating through their contents, and properly cleaning up resources.

## Key Components

### Header Structure
- Uses standard header guard pattern with `#ifndef FS_H` / `#define FS_H` / `#endif`
- Includes dependency on `"script_export.h"` for scripting integration macros

### Script Export Block
- All API definitions are wrapped in a `SCRIPT(fs, ...)` macro block
- This suggests the filesystem operations are being exposed to a scripting language or runtime system

### Data Types
- **`Dir`**: An opaque struct typedef representing a directory handle
  - Forward declared only (implementation details hidden)
  - Used as a handle for directory operations

### API Functions
The interface provides three core directory operations:

- **`fs_dir_open(const char *path)`**
  - Opens a directory at the specified path
  - Returns a `Dir*` pointer (directory handle)
  - Marked with `EXPORT` for script system visibility

- **`fs_dir_next_file(Dir *dir)`**
  - Iterates through files in an opened directory
  - Returns `const char*` pointing to the next filename
  - Returns `NULL` when no more files remain
  - Requires valid `Dir*` handle from `fs_dir_open`

- **`fs_dir_close(Dir *dir)`**
  - Closes directory handle and frees associated resources
  - Takes the `Dir*` handle to be cleaned up
  - Essential for proper resource management

### Resource Management
- Includes explicit comment reminder: "remember to *_close(...) when done to free resources!"
- Follows typical C pattern of open/iterate/close lifecycle
- Manual resource cleanup required (no automatic cleanup)

### Dependencies
- **`script_export.h`**: Provides `SCRIPT()` and `EXPORT` macros for binding C functions to scripting system
- Standard C string handling (uses `const char*` for paths and filenames)

## Sections

### File System Directory Operations Header
Source: [fs.h:1-11](../fs.h#L1-L11)

This header file defines a C API for directory traversal operations that are exported to a scripting system. The file establishes a complete interface for opening, reading, and closing directories.

## Header Guard and Dependencies
• Uses standard header guard pattern with `FS_H` macro to prevent multiple inclusions
• Depends on `script_export.h` for the `SCRIPT` and `EXPORT` macros

## Core API Structure
The entire API is wrapped in a `SCRIPT(fs, ...)` macro call, indicating this functionality is being exported to a scripting environment under the namespace "fs".

## Defined Entities

### Data Types
• **`Dir`** - An opaque struct type representing a directory handle
  - Declared as `typedef struct Dir Dir;`
  - Implementation details are hidden from users

### Exported Functions

• **`fs_dir_open`**
  - Signature: `Dir *fs_dir_open(const char *path)`
  - Opens a directory at the specified path
  - Returns a `Dir` pointer handle for subsequent operations

• **`fs_dir_next_file`**
  - Signature: `const char *fs_dir_next_file(Dir *dir)`
  - Iterates through files in an opened directory
  - Returns the name of the next file, or `NULL` when no more files remain
  - Takes a `Dir` pointer obtained from `fs_dir_open`

• **`fs_dir_close`**
  - Signature: `void fs_dir_close(Dir *dir)`
  - Releases resources associated with a directory handle
  - Must be called when finished with a `Dir` to prevent resource leaks

## Resource Management
The API includes an explicit warning comment emphasizing the importance of calling the `*_close` function to free allocated resources, indicating this follows a manual memory management pattern typical in C APIs.


### Related
- [Parent](../)

---
Hash: c6b4eb1c7f7a4e3345854e543bc4fe78863eb6424a36548895febbe44086b8a3
Generated: 2025-07-07T20:21:19-04:00
