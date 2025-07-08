# Summary: debugwin.m

## Table of Contents

- [Debug Window Initialization for macOS](#debug-window-initialization-for-macos)

This section implements platform-specific window configuration for a debug window on macOS using GLFW and Cocoa APIs.

## Function Definitions

**`debugwin_init()`** - Configures a debug window with specific display properties on macOS

## Dependencies and Includes

• **`glew_glfw.h`** - OpenGL Extension Wrangler and GLFW integration
• **`<GLFW/glfw3native.h>`** - GLFW native platform access functions
  - Enabled with `GLFW_EXPOSE_NATIVE_COCOA` and `GLFW_EXPOSE_NATIVE_NSGL` macros
• **`game.h`** - References `game_window` variable

## Implementation Details

The `debugwin_init()` function performs two main window configuration tasks:

• **Always-on-top behavior:**
  - Retrieves the native Cocoa window handle using `glfwGetCocoaWindow(game_window)`
  - Sets the window level to `NSFloatingWindowLevel` to keep it above other windows
  - Uses Objective-C syntax to call `setLevel:` method on the window object

• **Window positioning:**
  - Sets the window position to coordinates (9, 53) using `glfwSetWindowPos()`
  - This places the window in the top-left area of the screen

## Key References

• **`game_window`** - External window handle variable defined in `game.h`
• **`glfwGetCocoaWindow()`** - GLFW function to get native Cocoa window handle
• **`glfwSetWindowPos()`** - GLFW function for window positioning
• **`NSFloatingWindowLevel`** - macOS Cocoa constant for floating window level

This code is specifically designed for macOS development, using Cocoa's native window management features to create a debug window that stays visible above other application windows.

## Sections

### Debug Window Initialization for macOS
Source: [debugwin.m:1-13](../debugwin.m#L1-L13)

This section implements a debug window initialization function specifically for macOS using GLFW and Cocoa native APIs.

## Function Definition
- **`debugwin_init()`**: A void function that configures a debug window with specific macOS window properties

## Dependencies and Includes
- **`glew_glfw.h`**: Custom header file (assumed to contain GLEW/GLFW setup)
- **`GLFW/glfw3native.h`**: GLFW native API access, enabled by preprocessor defines:
  - `GLFW_EXPOSE_NATIVE_COCOA`: Exposes Cocoa window system integration
  - `GLFW_EXPOSE_NATIVE_NSGL`: Exposes NSOpenGL context integration
- **`game.h`**: External header providing access to `game_window` variable

## Window Configuration Logic
The function performs two main configuration steps:

1. **Always-on-top behavior**:
   - Retrieves the native Cocoa window handle using `glfwGetCocoaWindow(game_window)`
   - Stores the NSWindow object in variable `ns` (declared as `id` type)
   - Sets the window level to `NSFloatingWindowLevel` using Objective-C method call `[ns setLevel:NSFloatingWindowLevel]`

2. **Window positioning**:
   - Uses `glfwSetWindowPos(game_window, 9, 53)` to position the window at coordinates (9, 53) pixels from the top-left corner of the screen

## External References
- **`game_window`**: A GLFW window handle defined in `game.h` that represents the main game window being configured as a debug window

This function is platform-specific to macOS and creates a floating debug window that stays above other windows and is positioned in the top-left area of the screen.


### Related
- [Parent](../)

---
Hash: eb9a00ce1168ec33bb1702c02048568fcce5ec84633e8ccddf1c9e4dc0832f3b
Generated: 2025-07-07T20:21:19-04:00
