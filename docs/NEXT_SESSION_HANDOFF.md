# Next Session Handoff: Nathan Finishes the Project

Point the next session at this file. Written 2026-10-11, right after session 8.
Full team history is in `SESSION_HANDOFF.md`; this file is the starter.

> **Status: carried out in session 9** on `feature/nathan-finish` (see `SESSION_HANDOFF.md`, session 9). Still open: the PR/merge, VS IDE F5, the slides and the video.

**Scope change (decided by Nathan, 2026-10-11):** Nathan takes over the remaining
work of **Kim** (taskbar, File Explorer, Settings) and **Lorens** (theme, widgets,
clock/PWR) to finish the project. This overrides the earlier rule "don't edit
other members' files" for those modules. Record it in `SESSION_HANDOFF.md` and
in commit messages ("finishing Kim's/Lorens's part"). Credit stays with the
original authors in docs and the PPT.

## Where things stand

- **All modules are implemented, wired and verified.** Kim's and Lorens's *required* features already PASS the acceptance checklist, so what's left for them is polish, cleanup, docs and presentation material, not missing features.
- **Builds:** `x64-debug` and `x64-release` are clean, 0 warnings at `/W4` (VS Community 2026, MSVC).
- **Tests:** every executed row PASSES. NOT RUN: Visual Studio IDE F5, and display scaling above 100 %.
- **Branches (all pushed to origin):**
  - `main` = `fa5a6da`
  - `development` = `fa5a6da`
  - `feature/nathan-integration` = `development` + `4fda46d` (code) + `d958723` (docs)
- **No PR is open yet** (no `gh` CLI). Open it in the browser: https://github.com/NathanLaborada/CSOPESY-MCO4/compare/development...feature/nathan-integration

## Decisions a new session must NOT undo

1. **Render order is deliberate:** `desktop, system_controls, taskbar, file_explorer, settings, task_manager`. ImGui inserts `NoBringToFrontOnFocus` windows at the *back* of the stack when they are created, so the taskbar must be created after the clock/PWR window or it hides the clock and swallows PWR clicks.
2. **Theme:** applied in `Init()`, and re-applied in `RenderFrame()` *before* `ImGui::NewFrame()` only when `state_.theme_index != applied_theme_index_` (a private member, not a global).
3. **`ImGui::Selectable` full-row width must be `0.0f`, not `-1.0f`** (v1.91.9b gives a zero-width hit box for negative widths).
4. **UI strings stay plain ASCII/Latin-1.** The default font shows `—`, `→` and emoji as `?`.
5. **`AppState` is unchanged.** Keep it that way unless a feature truly needs it.
6. **The project stays a labelled simulation.** No real filesystem access, no real process enumeration.

## Work plan for the new session

Work on a new branch from `feature/nathan-integration` (or from `development` once the PR is merged), e.g. `feature/nathan-finish`. Build both presets and re-test after each step.

### 1. Kim's part: cleanup and polish

| # | Task | Files | Notes |
|---|---|---|---|
| K1 | Apply clang-format (73 pre-existing violations) | `src/apps/file_explorer.cc`, `src/apps/settings.cc` | Run `clang-format -i` (VS-bundled: `C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\bin\clang-format.exe`). Formatting only: commit it **separately** from any behaviour change |
| K2 | Taskbar launcher tooltips ("File Explorer", "Settings", "Task Manager") | `src/ui/taskbar.cc` | Optional polish: `if (ImGui::IsItemHovered()) ImGui::SetTooltip(...)` after each `IconButton`, like the PWR tooltip |
| K3 | Re-verify after K1/K2 | | File Explorer sidebar, breadcrumb, select, double-click; Settings 4 categories, wallpaper, theme |
| K4 | Session-log entry + PROJECT_PLAN note | `SESSION_HANDOFF.md`, `PROJECT_PLAN.md` | Say Nathan finished Kim's part and what changed |

### 2. Lorens's part: cleanup and polish

| # | Task | Files | Notes |
|---|---|---|---|
| L1 | Format check of Lorens's files | `src/ui/theme.cc`, `widgets.cc`, `system_controls.cc` + headers | `clang-format --dry-run -Werror`; fix only if it fails |
| L2 | Hover/active-state check | `widgets.cc` | Verify visually in both themes: launcher hover, active fill, PWR red hover. Already looked right in session 8; just confirm and screenshot |
| L3 | (Optional) Nicer bundled font | `application.cc` `Init()` + `theme` | Only if wanted. Must use a font **compiled into the binary** (no external asset files) and be loaded before the first frame. Skip if time is short: ProggyClean works |
| L4 | High-DPI | | Known limitation; document it rather than implement unless required by the PDF |
| L5 | Session-log entry | `SESSION_HANDOFF.md` | |

### 3. Byron's leftovers (small)

- B1: Byron's Task Manager (`fa5a6da`) has **no session-log entry**. Write one from the code: dummy `struct Process` with `cpu_base`, a shared sine wobble over `ImGui::GetTime()`, clamping, totals header, "Simulated dummy data" banner.
- B2 (optional): Task Manager column sorting is "not implemented" in `PROJECT_PLAN.md`. Do it only if the PDF requires it.

### 4. Finish line

1. Open the PR `feature/nathan-integration` → `development`, then merge. Then PR/merge the finishing branch → `development` → `main`. No force-push, no direct commits to `main`.
2. Confirm VS IDE: File > Open > Folder, x64 Debug, Build All, DesktopOS.exe, F5. Update the checklist row (currently NOT RUN).
3. Cross-check `PROJECT_PLAN.md` §2 against the assignment PDF (still unverified).
4. Capture all screenshots in `PPT_README.md` §4 from the final **Release** build at 1280x720, including Kim's (taskbar hover, File Explorer, Settings) and Lorens's (theme before/after, clock + PWR close-up).
5. Update `SESSION_HANDOFF.md`, `PROJECT_PLAN.md` (milestone M7), `README.txt` and `PPT_README.md` so docs match the final code. Never mark anything PASS that wasn't run.

## Build commands

```bat
"C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=x64
cd /d D:\MCO4\CSOPESY-MCO4
cmake --preset x64-debug   && cmake --build --preset x64-debug
cmake --preset x64-release && cmake --build --preset x64-release
.\out\build\x64-release\DesktopOS.exe
```

## How the GUI was tested (to repeat it)

There are no native computer-use tools in this environment. Session 8 used a throwaway PowerShell script (not in the repo; recreate it) that:

- launched the exe and got `MainWindowHandle`;
- mapped coordinates with `GetClientRect` + `ClientToScreen`;
- clicked with `SetCursorPos` + `mouse_event`;
- captured with `Graphics.CopyFromScreen`;
- closed the app with `PostMessage(WM_CLOSE)`.

Client-area coordinates at the default 1280x720 window (W/H = client size):

| Target | Client coordinate (x, y) |
|---|---|
| Taskbar launchers | File Explorer (26, H-26), Settings (64, H-26), Task Manager (102, H-26) |
| PWR | (W-25, H-24) |
| File Explorer close X | (832, 68); first listing row (340, 169) |
| Settings categories | x=160; y = 120 / 158 / 196 / 234 (System, Display, Personalization, About) |
| Wallpaper combo | (460, 166); items at y = 195 / 214 / 233 |
| Theme combo | (460, 222); items at y = 251 / 270 |

## Suggested opening prompt

> Read `docs/NEXT_SESSION_HANDOFF.md` and `SESSION_HANDOFF.md` in D:\MCO4\CSOPESY-MCO4. I'm Nathan and I'm finishing Kim's and Lorens's remaining parts myself. Check `git status`, `git fetch`, and whether the `feature/nathan-integration` PR exists or was merged. Then work through the work plan in order on a new branch, building and re-testing after each step, without undoing the listed decisions.
