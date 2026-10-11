# Architecture — CSOPESY MO4 Desktop OS Mockup

Status legend: **[DONE]** implemented and verified · **[IMPLEMENTED]** code merged, not yet wired or verified · **[PLANNED]** interface agreed, not yet implemented. As of 2026-10-11 every module is [DONE].

## Layers

```text
main.cc
  └── Application (core)                 owns window, GL context, ImGui, loop, shutdown
        ├── AppState state_              the ONE shared state instance
        └── RenderFrame() each frame:
              0. theme::Apply(theme_index) if it changed   (before NewFrame)
              1. desktop::Render(state)          [DONE] Nathan  wallpaper (background draw list)
              2. system_controls::Render(state)  [DONE] Lorens  clock + PWR button
              3. taskbar::Render(state)          [DONE] Kim     bottom bar, launches apps
              4. file_explorer::Render(state)    [DONE] Kim
              5. settings::Render(state)         [DONE] Kim
              6. task_manager::Render(state)     [DONE] Byron
```

All modules were wired and verified on Windows/MSVC in session 8 (2026-10-11).

**Why system_controls renders before taskbar.** Both windows use
`ImGuiWindowFlags_NoBringToFrontOnFocus`. Dear ImGui inserts a newly created
window with that flag at the *back* of its window stack, so of the two, the one
created *last* ends up *lowest*. With taskbar first, the 92 %-opaque bar covered
the clock and received the PWR clicks. Rendering system_controls first puts its
transparent overlay above the bar. App windows are normal windows created later,
so they stay above both; they clamp themselves above `kTaskbarHeight` and never
overlap the bar.

Supporting shared UI (no per-frame call from Application):

- `Theme` [DONE] (Lorens): colors and style. `Application::Init()` calls `theme::Apply(state_.theme_index)` (Midnight by default) right after creating the ImGui context. `RenderFrame()` compares `state_.theme_index` with the private cache `applied_theme_index_` before `ImGui::NewFrame()` and re-applies only when Settings changed it. Invalid indices fall back to Midnight inside `theme::Apply`.
- `Widgets` [DONE] (Lorens): reusable icon buttons, icons and muted labels used by the taskbar, clock/PWR and apps.

## Graphics stack

| Piece | Version | Role |
|---|---|---|
| GLFW | 3.4 (git tag `3.4`) | Window, OpenGL context, input events |
| OpenGL | 3.3 core profile, GLSL `#version 330` | Rasterizes ImGui's vertex buffers |
| Dear ImGui | v1.91.9b (git tag) | Immediate-mode UI and drawing |
| `imgui_impl_glfw` | same as ImGui | Feeds GLFW input to ImGui |
| `imgui_impl_opengl3` | same as ImGui | Renders ImGui draw data with OpenGL (built-in GL loader, no GLAD needed) |

Dependencies are downloaded by CMake `FetchContent` on first configure. ImGui core and backend `.cpp` files are compiled once, into the static library target `imgui` (see `CMakeLists.txt`).

## Lifecycle ([DONE], `src/core/application.cc`)

**Init()**
1. `glfwSetErrorCallback` → `glfwInit`
2. Window hints: GL 3.3 core → `glfwCreateWindow(1280x720)` → `glfwMakeContextCurrent` → `glfwSwapInterval(1)` → min size 800x500
3. `ImGui::CreateContext`, `IniFilename = nullptr` (same layout every launch), `theme::Apply(state_.theme_index)`
4. `ImGui_ImplGlfw_InitForOpenGL` → `ImGui_ImplOpenGL3_Init("#version 330")`

Any failure prints `[init] ...` to stderr, undoes whatever already succeeded, and returns `false`. `main` then exits with code 1.

**Run()** loops while `!state.shutdown_requested && !glfwWindowShouldClose(window)`:
- `glfwWaitEventsTimeout(1/30 s)`: input wakes it immediately; otherwise it redraws about 30 times per second. This replaced plain vsync polling, which used about 107% of a CPU core on the Intel Arc test machine.
- When minimized: `glfwWaitEvents()` (sleeps until restored).
- `RenderFrame()`: re-apply theme if `theme_index` changed → backend NewFrames → `ImGui::NewFrame` → module renders (order above) → `ImGui::Render` → `glViewport(framebuffer size)` → clear → `ImGui_ImplOpenGL3_RenderDrawData` → `glfwSwapBuffers`.

**Shutdown()** runs in reverse order: OpenGL3 backend → GLFW backend → ImGui context → window → `glfwTerminate`. Each step is guarded by a flag, so it's safe after a partial init and safe to call twice. The destructor calls it.

**Graceful exit paths**
- PWR button (Lorens) sets `state.shutdown_requested = true`. The loop ends and `main` returns, and the destructor cleans up. Verified 2026-10-11 (Release exit code 0).
- The native window close button sets the GLFW close flag and follows the same path. Verified (exit code 0).

## Shared state (`include/core/app_state.h`)

```cpp
struct AppState {
    bool show_file_explorer = false;   // Taskbar opens, FileExplorer closes itself
    bool show_settings = false;       // Taskbar opens, Settings closes itself
    bool show_task_manager = false;    // Taskbar opens, TaskManager closes itself
    bool shutdown_requested = false;  // PWR button writes; Application::Run reads
    int wallpaper_index = 0;          // Settings writes; Desktop reads (0..2)
    int theme_index = 0;              // Settings writes; Theme reads
};
inline constexpr float kTaskbarHeight = 48.0f;
```

Rules:
- Exactly one `AppState` exists: `Application::state_`. Modules receive it by reference and never keep their own copies or globals for these fields.
- Module-private UI state stays inside the module's `.cc` in an anonymous namespace. Examples: selected file, Task Manager sort column.
- Adding or changing a field is a shared-interface change. Agree it in the group chat first and log it in `SESSION_HANDOFF.md`.

## Module contract

Each module exposes one free function in its own namespace, nested in the project namespace `desktop_os`:

```cpp
// include/apps/task_manager.h
#ifndef DESKTOP_OS_APPS_TASK_MANAGER_H_
#define DESKTOP_OS_APPS_TASK_MANAGER_H_

#include "core/app_state.h"

namespace desktop_os {
namespace task_manager {

// Draws the Task Manager window while `state.show_task_manager` is true.
void Render(AppState& state);

}  // namespace task_manager
}  // namespace desktop_os

#endif  // DESKTOP_OS_APPS_TASK_MANAGER_H_
```

- The function returns immediately if its `show_*` flag is false.
- To close, use `ImGui::Begin(title, &state.show_x)`. The window's X button sets the flag back to false, and the taskbar can set it true again (reopen).
- One fixed window title per app means ImGui never creates a duplicate window.
- Keep app windows clear of the bottom `kTaskbarHeight` pixels so the taskbar stays usable.
- For full-row `ImGui::Selectable`s pass width `0.0f` (span available width), not `-1.0f`: in ImGui v1.91.9b a negative width gives a zero-width hit box.
- UI strings must stay within the default font's Latin-1 glyphs (no `—`, `→`, emoji); missing glyphs render as `?`.

## Coding style

All project C++ follows the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html). The repo's `.clang-format` (`BasedOnStyle: Google`) applies the formatting automatically in Visual Studio (Ctrl+K, Ctrl+D). Rules clang-format can't enforce:

| Rule | Example |
|---|---|
| File names `lower_snake_case`, `.cc` / `.h` | `src/apps/task_manager.cc` |
| `#define` guards `DESKTOP_OS_<PATH>_<FILE>_H_` (path below `include/`) | `DESKTOP_OS_UI_DESKTOP_H_` |
| All code in `namespace desktop_os`; one nested snake_case namespace per module | `desktop_os::task_manager::Render` |
| Types and functions PascalCase | `AppState`, `DrawGlow()`, `Init()` |
| Variables, parameters, struct fields snake_case | `show_task_manager`, `draw_list` |
| Class data members get a trailing `_` | `window_`, `state_` |
| Constants `k` + PascalCase | `kTaskbarHeight` |
| Include order: related header, C, C++ std, other libraries, project; blank line between groups; quotes for non-system headers | see `src/core/application.cc` |
| Casts with `static_cast`, not `(float)x` or `float(x)` | `static_cast<float>(i)` |
| Include the header instead of forward-declaring | `application.h` includes `"GLFW/glfw3.h"` |

Third-party APIs (ImGui, GLFW) keep their own naming. That's expected.

## Desktop ([DONE], `src/ui/desktop.cc`)

The wallpaper is drawn into `ImGui::GetBackgroundDrawList()`, which sits behind every ImGui window. It covers the full main viewport and is recomputed every frame, so resizing works automatically. There are no textures.

| index | name | look |
|---|---|---|
| 0 | Midnight | navy vertical gradient, blue/violet radial glows, dot grid |
| 1 | Aurora | dark teal gradient, teal glow, two families of sine-wave lines |
| 2 | Slate | charcoal gradient, faint blue glow, diagonal pinstripes |

Radial glows are a triangle fan with an opaque center vertex and transparent rim vertices, so the GPU interpolates a smooth gradient. `desktop::kWallpaperNames` gives Settings the picker labels. An out-of-range `wallpaper_index` falls back to 0.

## Recommended presentation diagram

Use the "Layers" tree above as a box diagram: `main` → `Application` → (`AppState` shared box) → six module boxes, each labeled with its owner. Draw arrows from Taskbar → `show_*` flags, each app's X → its `show_*` flag, PWR → `shutdown_requested`, Settings → `wallpaper_index`/`theme_index`, `wallpaper_index` → Desktop, and `theme_index` → Application → `theme::Apply`.
