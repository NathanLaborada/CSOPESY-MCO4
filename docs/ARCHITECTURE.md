# Architecture — CSOPESY MO4 Desktop OS Mockup

Status legend: **[DONE]** implemented and verified · **[PLANNED]** interface agreed, not yet implemented.

## Layers

```text
main.cpp
  └── Application (core)                 owns window, GL context, ImGui, loop, shutdown
        ├── AppState state_              the ONE shared state instance
        └── renderFrame() each frame, in this order:
              1. Desktop::Render(state)        [DONE]    Nathan  wallpaper (background draw list)
              2. Taskbar::Render(state)        [PLANNED] Kim     bottom bar, launches apps
              3. SystemControls::Render(state) [PLANNED] Lorens  clock + PWR button
              4. FileExplorer::Render(state)   [PLANNED] Kim
              5. Settings::Render(state)       [PLANNED] Kim
              6. TaskManager::Render(state)    [PLANNED] Byron
```

Supporting shared UI (no per-frame call from Application unless Lorens needs one):

- `Theme` [PLANNED] (Lorens): colors and style. Applied once in `Application::init()` and re-applied when `themeIndex` changes.
- `Widgets` [PLANNED] (Lorens): reusable icon buttons, panels and labels used by every module.

## Graphics stack

| Piece | Version | Role |
|---|---|---|
| GLFW | 3.4 (git tag `3.4`) | Window, OpenGL context, input events |
| OpenGL | 3.3 core profile, GLSL `#version 330` | Rasterizes ImGui's vertex buffers |
| Dear ImGui | v1.91.9b (git tag) | Immediate-mode UI and drawing |
| `imgui_impl_glfw` | same as ImGui | Feeds GLFW input to ImGui |
| `imgui_impl_opengl3` | same as ImGui | Renders ImGui draw data with OpenGL (built-in GL loader, no GLAD needed) |

Dependencies are downloaded by CMake `FetchContent` on first configure. ImGui core and backend `.cpp` files are compiled once, into the static library target `imgui` (see `CMakeLists.txt`).

## Lifecycle ([DONE], `src/core/Application.cpp`)

**init()**
1. `glfwSetErrorCallback` → `glfwInit`
2. Window hints: GL 3.3 core → `glfwCreateWindow(1280x720)` → `glfwMakeContextCurrent` → `glfwSwapInterval(1)` → min size 800x500
3. `ImGui::CreateContext`, `IniFilename = nullptr` (same layout every launch), style
4. `ImGui_ImplGlfw_InitForOpenGL` → `ImGui_ImplOpenGL3_Init("#version 330")`

Any failure prints `[init] ...` to stderr, undoes whatever already succeeded, and returns `false`. `main` then exits with code 1.

**run()** loops while `!state.shutdownRequested && !glfwWindowShouldClose(window)`:
- `glfwWaitEventsTimeout(1/30 s)`: input wakes it immediately; otherwise it redraws about 30 times per second. This replaced plain vsync polling, which used about 107% of a CPU core on the Intel Arc test machine.
- When minimized: `glfwWaitEvents()` (sleeps until restored).
- `renderFrame()`: backend NewFrames → `ImGui::NewFrame` → module renders → `ImGui::Render` → `glViewport(framebuffer size)` → clear → `ImGui_ImplOpenGL3_RenderDrawData` → `glfwSwapBuffers`.

**shutdown()** runs in reverse order: OpenGL3 backend → GLFW backend → ImGui context → window → `glfwTerminate`. Each step is guarded by a flag, so it's safe after a partial init and safe to call twice. The destructor calls it.

**Graceful exit paths**
- PWR button (Lorens) sets `state.shutdownRequested = true`. The loop ends and `main` returns, and the destructor cleans up.
- The native window close button sets the GLFW close flag and follows the same path.

## Shared state (`include/core/AppState.h`)

```cpp
struct AppState {
    bool showFileExplorer = false;   // Taskbar opens, FileExplorer closes itself
    bool showSettings = false;       // Taskbar opens, Settings closes itself
    bool showTaskManager = false;    // Taskbar opens, TaskManager closes itself
    bool shutdownRequested = false;  // PWR button writes; Application::run reads
    int wallpaperIndex = 0;          // Settings writes; Desktop reads (0..2)
    int themeIndex = 0;              // Settings writes; Theme reads
};
inline constexpr float kTaskbarHeight = 48.0f;
```

Rules:
- Exactly one `AppState` exists: `Application::state_`. Modules receive it by reference and never keep their own copies or globals for these fields.
- Module-private UI state stays inside the module's `.cpp` in an anonymous namespace. Examples: selected file, Task Manager sort column.
- Adding or changing a field is a shared-interface change. Agree it in the group chat first and log it in `SESSION_HANDOFF.md`.

## Module contract

Each module exposes one free function in its own namespace:

```cpp
// include/apps/TaskManager.h
#pragma once
#include "core/AppState.h"
namespace TaskManager { void Render(AppState& state); }
```

- The function returns immediately if its `show*` flag is false.
- To close, use `ImGui::Begin(title, &state.showX)`. The window's X button sets the flag back to false, and the taskbar can set it true again (reopen).
- One fixed window title per app means ImGui never creates a duplicate window.
- Keep app windows clear of the bottom `kTaskbarHeight` pixels so the taskbar stays usable.

## Desktop ([DONE], `src/ui/Desktop.cpp`)

The wallpaper is drawn into `ImGui::GetBackgroundDrawList()`, which sits behind every ImGui window. It covers the full main viewport and is recomputed every frame, so resizing works automatically. There are no textures.

| index | name | look |
|---|---|---|
| 0 | Midnight | navy vertical gradient, blue/violet radial glows, dot grid |
| 1 | Aurora | dark teal gradient, teal glow, two families of sine-wave lines |
| 2 | Slate | charcoal gradient, faint blue glow, diagonal pinstripes |

Radial glows are a triangle fan with an opaque center vertex and transparent rim vertices, so the GPU interpolates a smooth gradient. `Desktop::kWallpaperNames` gives Settings the picker labels. An out-of-range `wallpaperIndex` falls back to 0.

## Recommended presentation diagram

Use the "Layers" tree above as a box diagram: `main` → `Application` → (`AppState` shared box) → six module boxes, each labeled with its owner. Draw arrows from Taskbar → `show*` flags, PWR → `shutdownRequested`, Settings → `wallpaperIndex`/`themeIndex`, and `wallpaperIndex` → Desktop.
