# Summary: texture.c

## Table of Contents

- [Texture Loading and Management System](#texture-loading-and-management-system)

This section implements a comprehensive texture management system for OpenGL applications, providing functionality to load, cache, and bind textures from image files with automatic hot-reloading capabilities.

## Core Data Structure

• **`Texture` struct**: Contains texture metadata and OpenGL state
  - `filename`: Path to the texture file
  - `gl_name`: OpenGL texture ID (0 if uninitialized or error)
  - `width`, `height`, `components`: Image dimensions and channel count
  - `last_modified`: File modification timestamp for hot-reloading

• **`textures`**: Global static `Array` storing all loaded textures

## Key Functions

### Public API Functions

• **`texture_init()`**: Initializes the texture system by creating the textures array
• **`texture_deinit()`**: Cleanup function that frees all texture filenames and the textures array
• **`texture_load(const char *filename)`**: 
  - Loads a texture from file or returns existing one if already loaded
  - Returns `bool` indicating success/failure
  - Creates new `Texture` entry if not found, then calls `_load()`
• **`texture_bind(const char *filename)`**: Binds the specified texture for OpenGL rendering
• **`texture_get_size(const char *filename)`**: Returns texture dimensions as `Vec2`
• **`texture_update()`**: Updates all textures by calling `_load()` on each (enables hot-reloading)

### Internal Helper Functions

• **`_find(const char *filename)`**: 
  - Searches the textures array for a texture with matching filename
  - Returns pointer to `Texture` or `NULL` if not found

• **`_load(Texture *tex)`**: Core texture loading logic
  - Checks file modification time using `stat()` to avoid unnecessary reloads
  - Uses `stbi_load()` from stb_image library to read image data
  - Handles OpenGL texture creation and configuration:
    - Generates new OpenGL texture ID with `glGenTextures()`
    - Sets texture parameters (GL_NEAREST filtering)
    - Flips image vertically before uploading to OpenGL
    - Uploads texture data with `glTexImage2D()` using RGBA format
  - Updates modification timestamp
  - Provides console logging for load attempts

• **`_flip_image_vertical(unsigned char *data, unsigned int width, unsigned int height)`**: 
  - Flips image data vertically to match OpenGL's coordinate system
  - Allocates temporary buffer and copies rows in reverse order
  - Assumes 4 components per pixel (RGBA)

## Dependencies

• **External libraries**: `GL/glew.h`, `stb_image.h`
• **System libraries**: `string.h`, `stdlib.h`, `sys/stat.h`, `time.h`
• **Internal modules**: `error.h`, `array.h`, `console.h`, `texture.h`

## Key Features

• **Hot-reloading**: Automatically detects file changes and reloads textures
• **Caching**: Prevents duplicate loading of the same texture file
• **Error handling**: Gracefully handles missing files and load failures
• **Memory management**: Properly cleans up OpenGL resources and allocated memory
• **Console logging**: Provides feedback on texture loading operations

## Sections

### Texture Loading and Management System
Source: [texture.c:1-167](../texture.c#L1-L167)

This section implements a comprehensive texture management system for OpenGL applications with automatic file modification detection and hot-reloading capabilities.

## Core Data Structure
- **`Texture`** struct containing:
  - `filename`: path to texture file
  - `gl_name`: OpenGL texture ID (0 if uninitialized/error)
  - `width`, `height`, `components`: image dimensions and format info
  - `last_modified`: timestamp for file change detection

## Global State
- **`textures`**: static Array pointer storing all loaded Texture instances

## Key Functions

### Image Processing
- **`_flip_image_vertical()`**: Utility function that vertically flips image data
  - Allocates temporary buffer of size `width * height * 4`
  - Copies rows in reverse order using `memcpy()`
  - Required for OpenGL coordinate system compatibility

### Core Loading Logic
- **`_load()`**: Main texture loading function with hot-reload support
  - Uses `stat()` to check file modification time
  - Compares against cached `last_modified` timestamp
  - Skips reload if file unchanged and GL texture exists
  - On reload:
    - Uses `stbi_load()` to read image data
    - Deletes existing GL texture with `glDeleteTextures()`
    - Generates new GL texture with `glGenTextures()`
    - Sets GL_NEAREST filtering for both min/mag filters
    - Flips image vertically before uploading
    - Uploads as RGBA format with `glTexImage2D()`
    - Frees image data with `stbi_image_free()`

### Texture Management
- **`_find()`**: Searches texture array by filename using string comparison
- **`texture_load()`**: Public interface for loading textures
  - Checks if texture already exists via `_find()`
  - Creates new Texture entry if not found
  - Allocates and copies filename string
  - Calls `_load()` to perform actual loading

### Runtime Operations
- **`texture_bind()`**: Binds texture for rendering
  - Finds texture by filename
  - Calls `glBindTexture()` with GL_TEXTURE_2D target
- **`texture_get_size()`**: Returns texture dimensions as Vec2
  - Uses `error_assert()` to validate texture exists

### System Lifecycle
- **`texture_init()`**: Initializes texture system by creating empty Array
- **`texture_deinit()`**: Cleanup function that:
  - Frees all filename strings
  - Destroys the textures array
- **`texture_update()`**: Updates all textures by calling `_load()` on each
  - Enables hot-reloading by checking all textures for file changes

## Dependencies
- **External libraries**: GL/glew.h, stb_image.h for OpenGL and image loading
- **Internal modules**: error.h, array.h, console.h for utilities
- **System libraries**: sys/stat.h, time.h for file monitoring

## Key Features
- Automatic file modification detection and hot-reloading
- Texture caching to avoid redundant loads
- Proper OpenGL resource management (texture deletion/creation)
- Console logging for load success/failure feedback
- Image coordinate system conversion for OpenGL compatibility


### Related
- [Parent](../)

---
Hash: 24998f4b1fdc99e894c8b799dd5afc44ae81b90f87ed58bc0e5b2049cc520acf
Generated: 2025-07-07T20:21:19-04:00
