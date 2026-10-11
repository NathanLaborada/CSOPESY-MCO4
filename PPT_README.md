# PPT_README — Presentation Content Guide

This guide lists **what to put in the technical presentation and where to get it**. It is not the presentation itself; no slides have been created.

> As of 2026-10-11 every module is implemented, wired into `Application::RenderFrame()` and verified on Windows/MSVC (see `SESSION_HANDOFF.md`, session 8). All slides can now be filled. Take every screenshot from the final integrated build (after `feature/nathan-integration` is merged), not from older branches.

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
| 5 | GLFW / OpenGL / ImGui Initialization | 10-step init order, error handling, shutdown order | Snippet A | Nathan | src/core/application.cc `Init()` |
| 6 | Render Loop & Integration | Event wait → theme check → NewFrame → desktop, clock/PWR, taskbar, apps → Render → swap; idle throttling (107% → ~3–7% CPU); why clock/PWR renders before the taskbar | Snippet B | Nathan | application.cc `Run()`, `RenderFrame()` |
| 7 | Desktop & Wallpaper | Background draw list, gradients, radial glow triangle fan, 3 wallpapers, resize behavior | Screenshots of all 3 wallpapers; one resized | Nathan | src/ui/desktop.cc |
| 8 | Theme, Clock & PWR | Shared style, real-time clock source, PWR → graceful shutdown | Close-up of clock + PWR; Snippet G | Lorens | theme.cc, system_controls.cc |
| 9 | Taskbar & App Launching | Icon buttons → `show_*` flags; reopen; no duplicates | Taskbar close-up, hover state | Kim | taskbar.cc, app_state.h |
| 10 | File Explorer | Layout (nav, listing, content), drawn icons, selection | Screenshot with a selected item | Kim | file_explorer.cc |
| 11 | System Settings | Categories, placeholder info, working demo controls (wallpaper/theme) | Before/after of a setting change | Kim | settings.cc |
| 12 | Task Manager | Processes table, CPU/Memory columns, how dummy values are generated | Screenshot of table | Byron | task_manager.cc |
| 13 | Shared State & Lifecycle | `AppState` fields, who writes/reads, open/close/reopen flow | Snippet C | Nathan | app_state.h |
| 14 | Testing & Demo Results | Test checklist with PASS/FAIL | Table from SESSION_HANDOFF.md | all | SESSION_HANDOFF.md |
| 15 | Member Contributions | One row per member: modules, key decisions | — | each member | PROJECT_PLAN.md §5 |
| 16 | Challenges | Intel driver ignoring vsync → CPU spin (`glfwWaitEventsTimeout`); glow banding (vertex-color fan); integration: taskbar hid clock/PWR (ImGui `NoBringToFrontOnFocus` stacking → render order swap); `Selectable` width `-1` gave zero-width rows; em dash rendered as `?`; File Explorer null-path crash (Kim) | — | all | SESSION_HANDOFF.md sessions 1, 5, 8 |
| 17 | Conclusion | What works, what we learned | — | anyone | — |

Merge slides 5–6 or 10–11 if a shorter deck is needed.

## 3. Code snippets to include

Keep each snippet to ≤15 lines and cut irrelevant lines.

| ID | What | File / location | Talking points |
|---|---|---|---|
| A | Init sequence | `src/core/application.cc`, `Application::Init()` | GL 3.3 core hints, context current, ImGui context, two backends, failure → partial cleanup |
| B | Main loop + frame | `Application::Run()` and `RenderFrame()` | Wait-with-timeout instead of busy loop; theme re-applied only when `theme_index` changes (`applied_theme_index_`); module order with the system_controls-before-taskbar comment; viewport set from framebuffer size each frame (resize) |
| C | Shared state | `include/core/app_state.h` | Single instance, ownership comments, why no globals |
| D | Shutdown | `Application::Shutdown()` | Reverse order, idempotent flags |
| E | Wallpaper glow | `src/ui/desktop.cc`, `DrawGlow()` | Center vertex opaque, rim transparent → GPU gradient |
| F | Taskbar button → flag | `src/ui/taskbar.cc` (Kim) | Button sets `state.show_x = true`; window X sets it false |
| G | PWR + clock | `src/ui/system_controls.cc` (Lorens) | `shutdown_requested = true`; time from `std::chrono` |
| H | Process table | `src/apps/task_manager.cc` (Byron) | `BeginTable`, `struct Process` with `cpu_base`, CPU = base + shared `sin(ImGui::GetTime() ...)` wobble (clamped), "Simulated dummy data" banner |
| I | Theme switch | `Application::RenderFrame()` top + `src/ui/theme.cc` `Apply()` | Settings writes `theme_index`; Application detects the change outside the ImGui frame and calls `theme::Apply` |

## 4. Screenshots and diagrams

Capture all of these from the final Release build at the default 1280x720 window size.

| Screenshot | How to get it | Responsible |
|---|---|---|
| Full desktop with taskbar, nothing open | Launch, don't click anything | Nathan |
| Each wallpaper (Midnight, Aurora, Slate) | Settings → Personalization → Wallpaper | Nathan |
| Theme Midnight vs Slate (same window) | Settings → Personalization → Theme, before/after | Lorens |
| Taskbar close-up with hover + open indicators | Open 1–2 apps, hover a launcher | Kim |
| File Explorer with a selected item and a deep breadcrumb | Sidebar → Downloads, click a file | Kim |
| System Settings, all four categories (or one before/after) | Click each category | Kim |
| Task Manager, Processes table with totals + "Simulated" banner | Taskbar → Task Manager | Byron |
| Clock + PWR close-up | Crop the taskbar's right end | Lorens |
| All three apps open at once (no duplicates, taskbar still visible) | Click all three launchers | anyone |
| Resized small window (UI re-clamps above taskbar) | Drag the window to ~900x560 | Nathan |

**Architecture diagram:** draw the "Layers" tree from `docs/ARCHITECTURE.md` as boxes: `main.cc` → `Application` → shared `AppState` → six module boxes labeled with owner names, in the real render order (desktop, clock/PWR, taskbar, apps). Add arrows: Taskbar → `show_*` flags; app X → `show_*` false; PWR → `shutdown_requested` → `Application::Run` exit; Settings → `wallpaper_index` → Desktop; Settings → `theme_index` → Application → `theme::Apply`.

## 5. Technical discussion points

- **Why GLFW:** cross-platform window creation, OpenGL context creation and input events. ImGui has an official backend for it.
- **What OpenGL does:** it doesn't draw widgets itself. It rasterizes the triangle lists that ImGui produces (`ImGui_ImplOpenGL3_RenderDrawData`). We request a 3.3 core context with GLSL 330.
- **How ImGui draws:** immediate mode. Every frame each module calls `Begin/Button/...`, ImGui builds vertex/index buffers, and the OpenGL3 backend draws them. The wallpaper uses the background draw list directly.
- **Render loop:** wait for events (≤1/30 s) → re-apply theme if changed → new frame → desktop → clock/PWR → taskbar → apps → render → swap.
- **Window stacking:** the taskbar and clock/PWR never take focus (`NoBringToFrontOnFocus`), and ImGui puts such windows at the back when they are created. That's why clock/PWR is rendered before the taskbar: otherwise the bar would cover it.
- **Shared state controls visibility:** taskbar sets `show_*` true, the window's close button sets it false, and windows have fixed titles so they never duplicate.
- **Task Manager values:** simulated. Each process has a fixed `cpu_base` and memory value; CPU adds a small shared sine wobble over `ImGui::GetTime()` and is clamped. Nothing is measured from Windows.
- **PWR shutdown:** sets `shutdown_requested` → loop exits → backends shut down → ImGui context destroyed → window destroyed → `glfwTerminate` → exit code 0.

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
