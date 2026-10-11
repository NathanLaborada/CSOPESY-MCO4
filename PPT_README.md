# PPT_README — Presentation Content Guide

This guide lists **what to put in the technical presentation and where to get it**. It is not the presentation itself; no slides have been created.

> As of 2026-10-11 (session 9) every module is implemented, wired and verified on Windows/MSVC, and the code is final. The screenshots for section 4 are in `docs/screenshots/`, taken from the final Release build. The PDF requires these PPT parts: Cover, Video Walkthrough, Architectural Diagram, Code Snippets, and a written discussion of the design.

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
| 2 | Video Walkthrough | **Required by the PDF.** One uncut MP4 (480p-720p, max 1 GB) **embedded in the PPTX**, starting with pressing Run/Debug in Visual Studio | The recording itself | group | PDF "Submission Details" |
| 3 | Project Overview | What is simulated; list of required features; "simulated data" disclaimer | Full-desktop screenshot | Nathan | PROJECT_PLAN.md §2 |
| 4 | Technologies & Environment | C++17, VS Community 2026, CMake + presets, GLFW 3.4, OpenGL 3.3 core, Dear ImGui v1.91.9b, Git/GitHub | VS screenshot with CMake preset dropdown | Nathan | CMakeLists.txt, CMakePresets.json |
| 5 | System Architecture | Layer diagram (see §4) and who owns what | Diagram | Nathan | docs/ARCHITECTURE.md |
| 6 | GLFW / OpenGL / ImGui Initialization | 10-step init order, error handling, shutdown order | Snippet A | Nathan | src/core/application.cc `Init()` |
| 7 | Render Loop & Integration | Event wait → theme check → NewFrame → desktop, clock/PWR, taskbar, apps → Render → swap; idle throttling (107% → ~3–7% CPU); why clock/PWR renders before the taskbar | Snippet B | Nathan | application.cc `Run()`, `RenderFrame()` |
| 8 | Desktop & Wallpaper | Background draw list, gradients, radial glow triangle fan, 3 wallpapers, resize behavior | Screenshots of all 3 wallpapers; one resized | Nathan | src/ui/desktop.cc |
| 9 | Theme, Clock & PWR | Shared style, real-time clock source, PWR → graceful shutdown | Close-up of clock + PWR; Snippet G | Lorens | theme.cc, system_controls.cc |
| 10 | Taskbar & App Launching | Icon buttons → `show_*` flags; reopen; no duplicates | Taskbar close-up, hover state | Kim | taskbar.cc, app_state.h |
| 11 | File Explorer | Layout (nav, listing, content), drawn icons, selection | Screenshot with a selected item | Kim | file_explorer.cc |
| 12 | System Settings | Categories, placeholder info, working demo controls (wallpaper/theme) | Before/after of a setting change | Kim | settings.cc |
| 13 | Task Manager | Processes table, CPU/Memory columns, how dummy values are generated | Screenshot of table | Byron | task_manager.cc |
| 14 | Shared State & Lifecycle | `AppState` fields, who writes/reads, open/close/reopen flow | Snippet C | Nathan | app_state.h |
| 15 | Testing & Demo Results | Test checklist with PASS/FAIL | Table from SESSION_HANDOFF.md | all | SESSION_HANDOFF.md |
| 16 | Member Contributions | One row per member: modules, key decisions | — | each member | PROJECT_PLAN.md §5 |
| 17 | Challenges | Intel driver ignoring vsync → CPU spin (`glfwWaitEventsTimeout`); glow banding (vertex-color fan); integration: taskbar hid clock/PWR (ImGui `NoBringToFrontOnFocus` stacking → render order swap); `Selectable` width `-1` gave zero-width rows; em dash rendered as `?`; File Explorer null-path crash (Kim) | — | all | SESSION_HANDOFF.md sessions 1, 5, 8 |
| 18 | Conclusion | What works, what we learned | — | anyone | — |

Merge slides 6–7 or 11–12 if a shorter deck is needed.

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

| Screenshot | How to get it | Responsible | File in `docs/screenshots/` |
|---|---|---|---|
| Full desktop with taskbar, nothing open | Launch, don't click anything | Nathan | `01_desktop.png` |
| Each wallpaper (Midnight, Aurora, Slate) | Settings → Personalization → Wallpaper | Nathan | `02a_wallpaper_midnight.png`, `02b_wallpaper_aurora.png`, `02c_wallpaper_slate.png` |
| Theme Midnight vs Slate (same window) | Settings → Personalization → Theme, before/after | Lorens | `03a_theme_midnight.png`, `03b_theme_slate.png` |
| Taskbar close-up with hover + open indicators | Open 1–2 apps, hover a launcher | Kim | `04c_taskbar_closeup.png` (crop), full: `04a_taskbar_hover_midnight.png`, `04b_taskbar_hover_slate.png` |
| File Explorer with a selected item and a deep breadcrumb | Sidebar → Downloads, click a file | Kim | `05_file_explorer_downloads.png` |
| System Settings, all four categories (or one before/after) | Click each category | Kim | `06a_settings_system.png` … `06d_settings_personalization.png` |
| Task Manager, Processes table with totals + "Simulated" banner | Taskbar → Task Manager | Byron | `07_task_manager.png` |
| Clock + PWR close-up | Crop the taskbar's right end | Lorens | `08c_clock_pwr_closeup.png` (crop), PWR red hover: `08a_pwr_hover_midnight.png`, `08b_pwr_hover_slate.png` |
| All three apps open at once (no duplicates, taskbar still visible) | Click all three launchers | anyone | `09_all_three_open.png` |
| Resized small window (UI re-clamps above taskbar) | Drag the window to ~900x560 | Nathan | `10_resized_small.png` (874x500 client) |

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
| Nathan | Core architecture, graphics stack, loop, shared state, desktop | 4–8, 14 |
| Kim | Taskbar, File Explorer, System Settings | 10–12 |
| Byron | Task Manager, dummy data design, testing results | 13, 15 |
| Lorens | Theme, widgets, clock, PWR, UI consistency | 9, 17 |

Everyone contributes their row to slide 16. Since session 9 Nathan finished Kim's and Lorens's polish (formatting, tooltips), but the modules stay credited to Kim and Lorens.

## 7. Accuracy checklist (before finalizing the deck)

- [ ] Every screenshot comes from the actual final build.
- [ ] Every code snippet matches the submitted source. Re-copy it after the last merge.
- [ ] The architecture diagram matches `docs/ARCHITECTURE.md` and the code.
- [ ] Every functionality claim is backed by a PASS in the test checklist.
- [ ] Simulated data is labeled as simulated (Task Manager, Settings info, File Explorer).
- [ ] Member contributions match who actually wrote what (check git history).
- [ ] All required assignment items appear somewhere in the deck. Check against the PDF.

## 8. Group-Handled Deliverables

The PDF (`MO4 - Desktop_GUI.pdf`, checked in session 9) **requires a video**. The group records it; Claude did not and will not create any video. The rules:
- One seamless, uncut recording, at least 480p and at most 720p, max 1 GB. The app's default 1280x720 window fits.
- It must show pressing Run/Debug in the IDE, which starts the program. Don't touch the code while recording, and don't exit the program to change code.
- Make the IDE font large enough to read on video.
- Embed the MP4 directly in the PPTX (the file size should show it).
- The quiz is a timed black-box test: per question you submit an MP4 that demonstrates the given test case. Keep a build that runs from the IDE ready; the PDF says not to recompile during the quiz.
