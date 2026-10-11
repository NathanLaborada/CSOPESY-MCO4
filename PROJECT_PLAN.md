# PROJECT_PLAN — CSOPESY MO4 Desktop-Style OS Mockup

Last updated: 2026-10-11 (Nathan, session 9: finished Kim's and Lorens's polish, cross-checked the assignment PDF, captured the PPT screenshots. Statuses below reflect PASS results in `SESSION_HANDOFF.md`).
Status legend: **DONE** (implemented and verified) · **TODO** (not started) · **IN PROGRESS**.

## 1. Overview

This is a desktop-style operating system mockup written in C++17 with OpenGL, GLFW and Dear ImGui, built with CMake through Visual Studio Community 2026. It is a graphical simulation only, not a real OS or kernel. All process and system data shown is dummy data.

## 2. Assignment requirements

> **Cross-checked against the assignment PDF** (`MO4 - Desktop_GUI.pdf`, updated Sept 18 2026) in session 9. Its checklist has three requirements: **Desktop** (fills the window, wallpaper, real-time clock updated every frame in a fixed corner, PWR button as the way to close the app), **Taskbar** (fixed top/bottom panel, at least 3 clickable icon buttons: two open unique placeholder screens, the third opens the Task Manager) and **Task Manager** (resembles Windows', placeholder "Processes" table with CPU and memory, dummy values). All of them map to rows below and are DONE. A1-A5, D1, E1 and V1 are more detailed than the PDF; nothing in the PDF is missing.
>
> Non-code deliverables in the PDF: README.txt with names, run instructions and the entry file (`src/main.cc`, done); a PPT with Cover, **Video Walkthrough**, Architectural Diagram, Code Snippets and a written design discussion; and an uncut 480p-720p **MP4 embedded in the PPTX** that starts by pressing Run/Debug in the IDE. See PPT_README.md section 8.

| # | Requirement | Owner | Status |
|---|---|---|---|
| A1 | Desktop background fills the client area | Nathan | DONE |
| A2 | Wallpaper (gradient / drawn pattern) | Nathan | DONE (3 wallpapers) |
| A3 | Real-time clock showing system time | Lorens | DONE (wired + verified 2026-10-11) |
| A4 | Clearly identifiable PWR button that closes the app gracefully | Lorens (button) / Nathan (shutdown path) | DONE (PWR exits Debug + Release; needed render-order fix, session 8) |
| A5 | Resources cleaned up on shutdown | Nathan | DONE |
| B1 | Fixed taskbar, top or bottom | Kim | DONE (bottom, 48 px, verified) |
| B2 | ≥3 working icon buttons: File Explorer, System Settings, Task Manager | Kim | DONE (verified) |
| B3 | Taskbar remains visible while apps are open | Kim | DONE (verified with all three apps open and after resize) |
| C1 | Task Manager with Processes section and table | Byron | DONE (`fa5a6da`, verified) |
| C2 | Process name, CPU %, memory columns with dummy data | Byron | DONE (verified; labelled simulated) |
| D1 | File Explorer: title/nav area, placeholder listing, content area, icons, selection | Kim | DONE (verified after session-8 hit-box fix) |
| E1 | System Settings: title/nav, categories, placeholder info, demo controls | Kim | DONE (verified after session-8 hit-box fix) |
| V1 | Consistent dark theme, hover/active states, usable when resized | Lorens (theme) / all | DONE (theme applied at startup; resize verified) |

**Optional enhancements** (only after all of the above):
- Wallpaper picker in Settings: DONE (Kim, verified).
- Theme switcher (`theme_index`): DONE (Kim picker + Nathan re-apply, verified).
- Task Manager sorting: not implemented (not required by the PDF).
- Taskbar launcher tooltips: DONE (session 9, Nathan finishing Kim's part).

## 3. Architecture

See `docs/ARCHITECTURE.md` for the full description. In short: `main.cc` → `Application` (owns GLFW/OpenGL/ImGui and the loop) → a single `AppState` passed by reference to each module's `Render(AppState&)` function every frame.

## 4. Folder structure (actual)

```text
MCO4/
├── CMakeLists.txt          build + pinned deps (GLFW 3.4, ImGui v1.91.9b)
├── CMakePresets.json       x64-debug / x64-release (Ninja + MSVC)
├── .gitignore
├── README.txt  PROJECT_PLAN.md  PPT_README.md  SESSION_HANDOFF.md
├── docs/{ARCHITECTURE.md, NATHAN_NEXTPLAN.md, NEXT_SESSION_HANDOFF.md,
│        KIM_MCO4_PLAN_HANDOFF_REVIEW.md, screenshots/}
├── include/core/{application.h, app_state.h}
├── include/ui/{desktop,taskbar,theme,widgets,system_controls}.h
├── include/apps/{file_explorer,settings,task_manager}.h
├── src/main.cc
├── src/core/application.cc
├── src/ui/{desktop,taskbar,theme,widgets,system_controls}.cc
└── src/apps/{file_explorer,settings,task_manager}.cc
```

All planned modules exist. CMake picks up new `src/**/*.cc` automatically.

## 5. Member responsibilities

### Nathan: core, graphics setup, desktop (DONE for this phase)
- CMake configuration with pinned FetchContent dependencies and presets: **DONE**
- GLFW init, OpenGL 3.3 core context, Dear ImGui + GLFW/OpenGL3 backends: **DONE**
- Main loop, idle throttling, resize-safe viewport: **DONE**
- Init error handling with partial cleanup; ordered shutdown: **DONE**
- Desktop wallpaper (3 styles) and watermark: **DONE**
- Shared `AppState` interface and module contract: **DONE**
- Integration of other members' `Render` calls into `Application::RenderFrame`: **DONE** (session 8)
- Theme applied at startup and re-applied when `theme_index` changes: **DONE** (session 8)

### Kim: taskbar and app screens
- `Taskbar`: fixed bottom bar, exactly `kTaskbarHeight` tall, full width, ≥3 icon buttons that set `show_file_explorer` / `show_settings` / `show_task_manager` to true. Show an "open" indicator under each button whose app is visible.
- `FileExplorer`: sidebar (Quick access, This PC…), breadcrumb/path bar, placeholder file/folder table with drawn icons, click-to-select, double-click a folder to navigate.
- `Settings`: left category list (System, Display, Personalization, About), placeholder system info marked "simulated", wallpaper combo using `desktop::kWallpaperNames`, theme selector writing `theme_index`.

### Byron: Task Manager
- `TaskManager`: Windows-style window with a "Processes" tab/header, an `ImGui::BeginTable` with Name / Status / CPU % / Memory (MB) columns.
- Dummy data in a separate `struct Process` array (header or top of `.cc`) so it's easy to edit. Values are deterministic, e.g. a fixed base value plus a small sine-based fluctuation over `ImGui::GetTime()`.
- Totals row/header (overall CPU % and memory); optional column sorting.

### Lorens: shared UI framework, clock, PWR
- `Theme`: `Apply(int theme_index)` sets ImGui colors, rounding and spacing. Nathan swaps it in for `ImGui::StyleColorsDark()` in `Application::Init()`. Re-apply when `theme_index` changes.
- `Widgets`: `IconButton(...)` with drawn icons (folder, gear, chart, power), hover/active states, and a panel/label helper.
- `system_controls::Render(AppState&)`: real-time clock (`std::chrono` + `localtime_s`, HH:MM:SS and date) and the PWR button (power icon, red hover) that sets `state.shutdown_requested = true`. Place both in the taskbar's right end (coordinate positions with Kim).
- Cross-module visual consistency and resize checks.

## 6. Shared interfaces

Defined in `include/core/app_state.h`. Documented in `docs/ARCHITECTURE.md` (sections "Shared state" and "Module contract"). Every module's public interface is one function, `void <module>::Render(AppState& state)`.

## 7. UI design

- Dark, restrained palette: navy and charcoal backgrounds, a single blue accent (≈ `#4A7BE0`), and red only for PWR.
- Wallpaper drawn with ImGui primitives; no emoji or system fonts. Icons are drawn with `ImDrawList` shapes.
- App windows: rounded corners and consistent padding (from Theme), kept above the taskbar.
- Taskbar: 48 px tall, semi-opaque dark, with icon buttons on the left/center and the clock and PWR on the right.

## 8. Development sequence and milestones

| Milestone | Content | Status |
|---|---|---|
| M1 Foundation | CMake, deps, GLFW/GL/ImGui init, loop, shutdown | DONE 2026-10-09 |
| M2 Desktop | Wallpaper, shared state, docs | DONE 2026-10-09 |
| M3 Shared UI | Theme, Widgets, clock, PWR (Lorens) | DONE 2026-10-11 (wired + verified) |
| M4 Taskbar + apps | Taskbar, File Explorer, Settings (Kim) | DONE 2026-10-11 (wired + verified, small hit-box fixes) |
| M5 Task Manager | Process data + table (Byron) | DONE 2026-10-11 (`fa5a6da`, wired + verified) |
| M6 Integration | All `Render` calls wired; full test checklist | DONE 2026-10-11, merged into `main` (PR #2) |
| M7 Docs + PPT | Final README/handoff/PPT guide refresh, screenshots | Docs + screenshots DONE 2026-10-11 (session 9, `docs/screenshots/`). Remaining for the group: build the slides, record and embed the video, confirm VS IDE F5 |

M3–M5 can run in parallel; each depends only on M1/M2. Kim's taskbar buttons should use Lorens's `widgets::IconButton` once it exists. Until then a plain `ImGui::Button` is fine and can be swapped later.

## 9. Integration strategy

1. Each member works on their own branch and their own files only.
2. When a module compiles and works on its own (temporarily calling it locally is fine), open a PR to `development`.
3. Nathan adds the single `<module>::Render(state_);` line in `Application::RenderFrame()` during review. Current order: desktop, system_controls, taskbar, file_explorer, settings, task_manager (system_controls before taskbar is deliberate, see docs/ARCHITECTURE.md).
4. After each merge, everyone pulls `development`, rebuilds, and runs the test checklist below.
5. `development` → `main` only when the full checklist passes.

## 10. Testing plan

Use the checklist in `SESSION_HANDOFF.md` ("Test checklist"). Mark each test PASS, FAIL or NOT RUN. Never mark a test PASS without running it.

## 11. Risks and mitigations

| Risk | Mitigation |
|---|---|
| Members edit `application.cc` or `app_state.h` simultaneously → merge conflicts | Only Nathan edits them; changes are requested in chat first |
| FetchContent needs internet + git on first configure | Documented in README; after the first configure, the deps are cached in `out/` |
| GPU driver ignores vsync → 100% CPU | Loop uses `glfwWaitEventsTimeout` (verified ~0–5% CPU idle) |
| Apps overlap taskbar | `kTaskbarHeight` constant; apps clamp position/size above it |
| Video must start from the IDE's Run/Debug, but VS IDE F5 is not yet confirmed | Confirm F5 (README.txt "Build and run in Visual Studio") before recording |

## 12. Definition of done

- [x] Every requirement in section 2 is marked DONE with a test result of PASS (2026-10-11).
- [x] Clean configure + build of `x64-debug` and `x64-release` with no errors (0 warnings).
- [x] PWR and the window close button both exit with code 0 (Release verified; Debug PWR exit verified).
- [x] Documentation matches the code. `SESSION_HANDOFF.md` is current.
- [ ] Integration branch reviewed and merged into `development`, then `main`.
- [x] Requirements cross-checked against the assignment PDF (session 9).
- [ ] VS IDE "Open Folder" + F5 confirmed by a member.
