# Summary: fs.c

## Table of Contents

- [Directory Operations Implementation](#directory-operations-implementation)

This section implements a cross-platform directory traversal API using a wrapper around the standard POSIX directory functions. The implementation provides a simplified interface for opening, reading, and closing directories.

## Key Components

### Data Structure
- **`struct Dir`**: A wrapper structure containing a single member `DIR *d` that holds the underlying system directory handle

### Function Definitions

- **`fs_dir_open(const char *path)`**: Opens a directory for reading
  - Allocates memory for a `Dir` structure
  - Calls `opendir(path)` to get the system directory handle
  - Returns `NULL` on failure (with proper cleanup), or the `Dir` pointer on success
  - Handles error cases by freeing allocated memory before returning `NULL`

- **`fs_dir_next_file(Dir *dir)`**: Retrieves the next file/directory entry
  - Calls `readdir(dir->d)` to get the next directory entry
  - Returns the filename (`de->d_name`) if an entry exists
  - Returns `NULL` when no more entries are available

- **`fs_dir_close(Dir *dir)`**: Closes the directory and cleans up resources
  - Calls `closedir(dir->d)` to close the system directory handle
  - Frees the allocated `Dir` structure memory

## Platform Compatibility

The code handles cross-platform compilation differences:
- **Windows (MSVC)**: Uses `#include <dirent_win.h>` 
- **Unix/Linux systems**: Uses standard `#include <dirent.h>`

## Dependencies

- **Internal**: `fs.h` header file
- **External**: Standard library functions (`malloc`, `free`) and directory operations (`opendir`, `readdir`, `closedir`)
- **Platform-specific**: `dirent.h` or `dirent_win.h` for directory entry structures and functions

## Usage Pattern

The API follows a typical open-read-close pattern:
1. Open directory with `fs_dir_open()`
2. Iterate through entries with repeated calls to `fs_dir_next_file()`
3. Close and cleanup with `fs_dir_close()`

## Sections

### Directory Operations Implementation
Source: [fs.c:1-46](../fs.c#L1-L46)

This section implements a cross-platform directory traversal API using standard POSIX directory functions. The code provides a simplified interface for opening, reading, and closing directories.

## Dependencies and Platform Support
• **Headers**: Includes `fs.h`, `stdlib.h`, `sys/types.h`
• **Platform-specific**: Uses conditional compilation for Windows (`_MSC_VER`) vs Unix-like systems
  - Windows: `dirent_win.h` 
  - Unix/Linux: `dirent.h`

## Data Structure
• **`Dir` struct**: Wrapper structure containing:
  - `DIR *d`: Pointer to the underlying system directory handle

## Function Definitions

### `fs_dir_open(const char *path)`
• **Purpose**: Opens a directory for reading
• **Parameters**: `path` - string path to the directory
• **Return**: Pointer to `Dir` structure, or `NULL` on failure
• **Logic**:
  1. Allocates memory for a new `Dir` structure using `malloc(sizeof(Dir))`
  2. Calls `opendir(path)` to open the directory
  3. If `opendir` fails (returns NULL):
     - Frees the allocated `Dir` structure
     - Returns `NULL`
  4. Otherwise returns the populated `Dir` structure

### `fs_dir_next_file(Dir *dir)`
• **Purpose**: Reads the next directory entry
• **Parameters**: `dir` - pointer to opened `Dir` structure
• **Return**: String name of next file/directory, or `NULL` when no more entries
• **Logic**:
  1. Calls `readdir(dir->d)` to get next directory entry
  2. If entry exists, returns `de->d_name` (the filename)
  3. If no more entries, returns `NULL`

### `fs_dir_close(Dir *dir)`
• **Purpose**: Closes directory and frees resources
• **Parameters**: `dir` - pointer to `Dir` structure to close
• **Logic**:
  1. Calls `closedir(dir->d)` to close the system directory handle
  2. Calls `free(dir)` to deallocate the `Dir` structure

## Key External Dependencies
• **System calls**: `opendir`, `readdir`, `closedir` from dirent library
• **Memory management**: `malloc`, `free` from stdlib
• **Data types**: `struct dirent` for directory entries


### Related
- [Parent](../)

---
Hash: 2cd7d348c882e714a715418966cfd67d7ccfada01c58ef89aeeeaefe832df3e6
Generated: 2025-07-07T20:21:19-04:00
