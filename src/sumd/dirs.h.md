# Summary: dirs.h

## Table of Contents

- [Directory Path Configuration Header](#directory-path-configuration-header)

This header file defines default directory paths and utility macros for constructing file paths within the application.

**Defined Entities:**

• **DATA_DIR** - Preprocessor macro defining the default data directory path
  - Set to `"./data/"` if not already defined elsewhere
  - Uses conditional compilation (`#ifndef`) to allow override at compile time

• **USR_DIR** - Preprocessor macro defining the default user directory path
  - Set to `"./usr/"` if not already defined elsewhere
  - Uses conditional compilation (`#ifndef`) to allow override at compile time

• **data_path(path)** - Function-like macro for constructing data file paths
  - Takes a `path` parameter and concatenates it with `DATA_DIR`
  - Example usage: `data_path("config.txt")` would expand to `"./data/config.txt"`

• **usr_path(path)** - Function-like macro for constructing user file paths
  - Takes a `path` parameter and concatenates it with `USR_DIR`
  - Example usage: `usr_path("settings.ini")` would expand to `"./usr/settings.ini"`

**Key Features:**

• **Include Guard** - Uses `#ifndef DIRS_H` / `#define DIRS_H` / `#endif` pattern to prevent multiple inclusions

• **Configurable Defaults** - Both directory macros can be overridden at compile time by defining them before including this header

• **Path Construction Utilities** - Provides convenient macros to build full file paths by combining base directories with relative paths

This header serves as a centralized location for managing application directory structure and provides a consistent interface for file path construction throughout the codebase.

## Sections

### Directory Path Configuration Header
Source: [dirs.h:1-11](../dirs.h#L1-L11)

This header file defines directory path constants and utility macros for file system organization. It provides a centralized way to configure and access data and user directories within the application.

**Defined Entities:**

• **DATA_DIR** - Directory path constant for data files
  - Default value: `"./data/"`
  - Can be overridden by defining it before including this header
  - Protected by `#ifndef` to allow external definition

• **USR_DIR** - Directory path constant for user files  
  - Default value: `"./usr/"`
  - Can be overridden by defining it before including this header
  - Protected by `#ifndef` to allow external definition

• **data_path(path)** - Macro function for constructing data file paths
  - Takes a path string as parameter
  - Returns concatenated string: `DATA_DIR` + `path`
  - Example usage: `data_path("config.txt")` would expand to `"./data/config.txt"`

• **usr_path(path)** - Macro function for constructing user file paths
  - Takes a path string as parameter  
  - Returns concatenated string: `USR_DIR` + `path`
  - Example usage: `usr_path("settings.ini")` would expand to `"./usr/settings.ini"`

**Header Guard:**
• Uses `DIRS_H` as the include guard to prevent multiple inclusions

**Key Features:**
• Configurable directory paths through conditional compilation
• Convenient path construction macros for consistent file access
• Relative path defaults that can be customized for different deployment environments


### Related
- [Parent](../)

---
Hash: 927f4549a6865f44e59b76b7b4e4cb84a6850898e05d34225cf4c7b1c19464fa
Generated: 2025-07-07T20:21:19-04:00
