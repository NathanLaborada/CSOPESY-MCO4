# PPT_README — Presentation Content Guide

This guide lists **what to put in the technical presentation and where to get it**. It is not the presentation itself; no slides have been created.

> As of 2026-10-09 only Nathan's part (core + desktop) is implemented. Slides about the taskbar, apps, clock and PWR depend on modules not yet written. Refresh this guide once they land, and take every screenshot from the final integrated build.

## 1. What the presentation must communicate

- What we simulate: a desktop OS shell (desktop, taskbar, File Explorer, Settings, Task Manager). It is not a real OS. All data is dummy data.
- How the desktop is structured: Application → shared `AppState` → modules.
- How GLFW (window/input), OpenGL (rendering) and Dear ImGui (UI) work together.
- How each required feature was implemented.
- What each of the four members built.

## 2. Slide-by-slide outline

| # | Title | Purpose / content | Evidence to capture | Supplied by | Source files |
|---|---|---|---|---|---|
| 1 | Cover | Course (CSOPESY), MO4 title, group members | — | anyone | — |
| 2 | Project Overview | What is simulated; list of required features; "simulated data" disclaimer | Full-desktop screenshot | Nathan | PROJECT_PLAN.md §2 |
| 3 | Technologies & Environment | C++17, VS Community 2026, CMake + presets, GLFW 3.4, OpenGL 3.3 core, Dear ImGui v1.91.9b, Git/GitHub | VS screenshot with CMake preset dropdown | Nathan | CMakeLists.txt, CMakePresets.json |
| 4 | System Architecture | Layer diagram (see §4) and who owns what | Diagram | Nathan | docs/ARCHITECTURE.md |
| 5 | GLFW / OpenGL / ImGui Initialization | 10-step init order, error handling, shutdown order | Snippet A | Nathan | src/core/Application.cpp `init()` |
| 6 | Render Loop | Event wait → NewFrame → modules → Render → swap; idle throttling (107% → ~0–5% CPU) | Snippet B | Nathan | Application.cpp `run()`, `renderFrame()` |
| 7 | Desktop & Wallpaper | Background draw list, gradients, radial glow triangle fan, 3 wallpapers, resize behavior | Screenshots of all 3 wallpapers; one resized | Nathan | src/ui/Desktop.cpp |
| 8 | Theme, Clock & PWR | Shared style, real-time clock source, PWR → graceful shutdown | Close-up of clock + PWR; Snippet G | Lorens | Theme.cpp, SystemControls.cpp |
| 9 | Taskbar & App Launching | Icon buttons → `show*` flags; reopen; no duplicates | Taskbar close-up, hover state | Kim | Taskbar.cpp, AppState.h |
| 10 | File Explorer | Layout (nav, listing, content), drawn icons, selection | Screenshot with a selected item | Kim | FileExplorer.cpp |
| 11 | System Settings | Categories, placeholder info, working demo controls (wallpaper/theme) | Before/after of a setting change | Kim | Settings.cpp |
| 12 | Task Manager | Processes table, CPU/Memory columns, how dummy values are generated | Screenshot of table | Byron | TaskManager.cpp |
| 13 | Shared State & Lifecycle | `AppState` fields, who writes/reads, open/close/reopen flow | Snippet C | Nathan | AppState.h |
| 14 | Testing & Demo Results | Test checklist with PASS/FAIL | Table from SESSION_HANDOFF.md | all | SESSION_HANDOFF.md |
| 15 | Member Contributions | One row per member: modules, key decisions | — | each member | PROJECT_PLAN.md §5 |
| 16 | Challenges | e.g. Intel driver ignoring vsync → CPU spin, fixed with `glfwWaitEventsTimeout`; glow banding fixed with vertex-color fan; others from members | — | all | SESSION_HANDOFF.md log |
| 17 | Conclusion | What works, what we learned | — | anyone | — |

Merge slides 5–6 or 10–11 if a shorter deck is needed.

## 3. Code snippets to include

Keep each snippet to ≤15 lines and cut irrelevant lines.

| ID | What | File / location | Talking points |
|---|---|---|---|
| A | Init sequence | `src/core/Application.cpp`, `Application::init()` | GL 3.3 core hints, context current, ImGui context, two backends, failure → partial cleanup |
| B | Main loop + frame | `Application::run()` and `renderFrame()` | Wait-with-timeout instead of busy loop; frame order; viewport set from framebuffer size each frame (resize) |
| C | Shared state | `include/core/AppState.h` | Single instance, ownership comments, why no globals |
| D | Shutdown | `Application::shutdown()` | Reverse order, idempotent flags |
| E | Wallpaper glow | `src/ui/Desktop.cpp`, `Glow()` | Center vertex opaque, rim transparent → GPU gradient |
| F | Taskbar button → flag | `src/ui/Taskbar.cpp` (Kim) | Button sets `state.showX = true`; window X sets it false |
| G | PWR + clock | `src/ui/SystemControls.cpp` (Lorens) | `shutdownRequested = true`; time from `std::chrono` |
| H | Process table | `src/apps/TaskManager.cpp` (Byron) | `BeginTable`, dummy data struct, deterministic values |

## 4. Screenshots and diagrams

| Screenshot | Responsible |
|---|---|
| Full desktop with taskbar, nothing open | Nathan |
| Each wallpaper (Midnight, Aurora, Slate) | Nathan (switch via Settings once Kim adds the picker) |
| Taskbar close-up with hover/active state | Kim |
| File Explorer with a selected item | Kim |
| System Settings, before/after one control | Kim |
| Task Manager, Processes table | Byron |
| Clock + PWR close-up | Lorens |
| All three apps open at once (shows no duplicates, taskbar still visible) | anyone |

**Architecture diagram:** draw the "Layers" tree from `docs/ARCHITECTURE.md` as boxes: `main.cpp` → `Application` → shared `AppState` → six module boxes labeled with owner names. Add arrows: Taskbar → `show*` flags; PWR → `shutdownRequested` → `Application::run` exit; Settings → `wallpaperIndex` → Desktop.

## 5. Technical discussion points

- **Why GLFW:** cross-platform window creation, OpenGL context creation and input events. ImGui has an official backend for it.
- **What OpenGL does:** it doesn't draw widgets itself. It rasterizes the triangle lists that ImGui produces (`ImGui_ImplOpenGL3_RenderDrawData`). We request a 3.3 core context with GLSL 330.
- **How ImGui draws:** immediate mode. Every frame each module calls `Begin/Button/...`, ImGui builds vertex/index buffers, and the OpenGL3 backend draws them. The wallpaper uses the background draw list directly.
- **Render loop:** wait for events (≤1/30 s) → new frame → desktop → taskbar → controls → apps → render → swap.
- **Shared state controls visibility:** taskbar sets `show*` true, the window's close button sets it false, and windows have fixed titles so they never duplicate.
- **Task Manager values:** simulated, generated by Byron's code from fixed base values (explain the formula he uses). Not measured from Windows.
- **PWR shutdown:** sets `shutdownRequested` → loop exits → backends shut down → ImGui context destroyed → window destroyed → `glfwTerminate` → exit code 0.

## 6. Group contributions (presentation responsibilities)

| Member | Presents | Slides |
|---|---|---|
| Nathan | Core architecture, graphics stack, loop, shared state, desktop | 3–7, 13 |
| Kim | Taskbar, File Explorer, System Settings | 9–11 |
| Byron | Task Manager, dummy data design, testing results | 12, 14 |
| Lorens | Theme, widgets, clock, PWR, UI consistency | 8, 16 |

Everyone contributes their row to slide 15.

## 7. Accuracy checklist (before finalizing the deck)

- [ ] Every screenshot comes from the actual final build.
- [ ] Every code snippet matches the submitted source. Re-copy it after the last merge.
- [ ] The architecture diagram matches `docs/ARCHITECTURE.md` and the code.
- [ ] Every functionality claim is backed by a PASS in the test checklist.
- [ ] Simulated data is labeled as simulated (Task Manager, Settings info, File Explorer).
- [ ] Member contributions match who actually wrote what (check git history).
- [ ] All required assignment items appear somewhere in the deck. Check against the PDF.

## 8. Group-Handled Deliverables

The assignment PDF was not available to Claude, so it is unknown whether a video walkthrough or embedded MP4 is required. **Whoever has the PDF must check this.** If a video is required, the group records, edits and embeds it themselves. Claude did not and will not create any video.
