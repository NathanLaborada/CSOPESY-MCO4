CSOPESY MO4 - DESKTOP-STYLE OS MOCKUP
=====================================

A graphical desktop operating-system mockup (desktop, taskbar, File Explorer,
System Settings, Task Manager) written in C++17 with OpenGL, GLFW and
Dear ImGui. This is a simulation: it does not manage real files, change real
settings, or measure real processes. All such data is placeholder/dummy data.

GROUP MEMBERS AND MODULE OWNERSHIP
----------------------------------
  Nathan  - Core application, CMake, GLFW/OpenGL/ImGui setup, main loop,
            shutdown, desktop wallpaper, shared state (AppState)
            Files: src/main.cpp, src/core/Application.cpp,
                   include/core/Application.h, include/core/AppState.h,
                   src/ui/Desktop.cpp, include/ui/Desktop.h, CMakeLists.txt
  Kim     - Taskbar, File Explorer, System Settings
            Files: src/ui/Taskbar.*, src/apps/FileExplorer.*, src/apps/Settings.*
  Byron   - Task Manager and dummy process data
            Files: src/apps/TaskManager.*
  Lorens  - Theme, reusable widgets, real-time clock, PWR button
            Files: src/ui/Theme.*, src/ui/Widgets.*, src/ui/SystemControls.*
  (.h files live under include/ with the same sub-folder.)

Current status per module: see SESSION_HANDOFF.md (top section).

REQUIRED TECHNOLOGIES
---------------------
  - Windows 10/11, x64
  - Visual Studio Community 2026 with the "Desktop development with C++"
    workload (includes MSVC, CMake and Ninja)
  - Git for Windows (CMake uses it to download dependencies)
  - Internet access for the FIRST configure only
  - A GPU/driver supporting OpenGL 3.3 (any Windows GPU from the last ~10 years)

DEPENDENCIES (downloaded automatically by CMake FetchContent, pinned)
---------------------------------------------------------------------
  GLFW        3.4       https://github.com/glfw/glfw        (tag 3.4)
  Dear ImGui  v1.91.9b  https://github.com/ocornut/imgui    (tag v1.91.9b)
  OpenGL      system opengl32 (part of Windows)
Nothing has to be installed by hand. ImGui's core files and its two backends
(imgui_impl_glfw, imgui_impl_opengl3) are compiled into a static library target
named "imgui" defined in CMakeLists.txt.

VISUAL STUDIO COMMUNITY 2026 SETUP
----------------------------------
  1. Install Visual Studio Community 2026:
     https://visualstudio.microsoft.com/downloads/
  2. In the Visual Studio Installer, tick the workload
     "Desktop development with C++". Keep the default optional component
     "C++ CMake tools for Windows" ticked.
  3. Install Git for Windows if you do not have it: https://git-scm.com/

BUILD AND RUN IN VISUAL STUDIO
------------------------------
  1. File > Open > Folder... and pick the project folder (the one that
     contains CMakeLists.txt). Do NOT use "Open Project/Solution".
  2. Visual Studio reads CMakePresets.json and configures automatically.
     The first configure downloads GLFW and ImGui and takes about a minute.
     Watch the Output window ("CMake" pane) for "CMake generation finished".
  3. In the toolbar configuration dropdown choose "x64 Debug"
     (or "x64 Release").
  4. Build > Build All (Ctrl+Shift+B).
  5. In the "Select Startup Item" dropdown (green play button) choose
     DesktopOS.exe, then press F5 (debug) or Ctrl+F5 (run without debugging).
  6. Exit with the PWR button, or the window's X button.

  Debug builds show a console window that prints [init]/[GLFW] errors.
  Release builds have no console window.

BUILD FROM THE COMMAND LINE (optional)
--------------------------------------
Open "Developer PowerShell for VS 2026" (Start menu), cd to the project, then:
  cmake --preset x64-debug
  cmake --build --preset x64-debug
  .\out\build\x64-debug\DesktopOS.exe
Use x64-release instead of x64-debug for an optimized build. A normal
PowerShell window will NOT work, because it doesn't have cl.exe and ninja on PATH.

Build output goes to out\build\<preset>\ (ignored by git).

ADDING A NEW SOURCE FILE
------------------------
Put .cpp files anywhere under src\ and headers under include\. CMake collects
src\**\*.cpp automatically. In Visual Studio use
Project > Configure Cache (or save CMakeLists.txt) so the new file is seen.
Include headers by path from include\, e.g.  #include "apps/TaskManager.h".

TROUBLESHOOTING
---------------
  "CMake Error ... git" / FetchContent fails
      Git is missing or there is no internet. Install Git, check the
      connection, then Project > Delete Cache and Reconfigure.
  "'cl.exe' is not recognized" or "CMAKE_CXX_COMPILER not set"
      You built from a normal terminal. Use Developer PowerShell for VS 2026,
      or build inside Visual Studio.
  DesktopOS.exe is missing from the startup item list
      The configure step failed or never ran; check the Output > CMake pane,
      then Project > Delete Cache and Reconfigure.
  LNK2019 unresolved external symbol "SomeModule::Render"
      A Render call was added to Application.cpp but that module's .cpp
      does not exist yet or was not picked up. Reconfigure CMake.
  LNK2005 "already defined" for an ImGui symbol
      Somebody added imgui .cpp files to src\. Delete them; ImGui is built
      only by the "imgui" target in CMakeLists.txt.
  Window does not open, console says "could not create window with OpenGL 3.3"
      Update the graphics driver. Remote Desktop / VMs without GPU
      acceleration may not support OpenGL 3.3.
  High CPU usage
      Should be ~0-5% when idle. If a module adds a busy loop or
      sleeps, remove it; the main loop already throttles itself.

PROJECT DIRECTORY OVERVIEW
--------------------------
  CMakeLists.txt        build definition, pinned dependencies
  CMakePresets.json     x64-debug / x64-release configurations
  src\main.cpp          ENTRY POINT: creates Application, init(), run()
  src\core\             Application: window, OpenGL, ImGui, loop, shutdown
  src\ui\               desktop, taskbar, theme, widgets, clock/PWR
  src\apps\             File Explorer, Settings, Task Manager
  include\              headers mirroring src\ (AppState.h = shared state)
  docs\ARCHITECTURE.md  design, lifecycle, shared interfaces
  PROJECT_PLAN.md       roadmap, ownership, milestones
  SESSION_HANDOFF.md    current status + session log (READ THIS FIRST)
  PPT_README.md         what to put in the presentation

GIT / GITHUB
------------
The folder is not yet a git repository (as of 2026-10-09). To set it up:
  git init -b main
  git add .
  git commit -m "core: project skeleton, graphics init, desktop"
  (create an empty repo on GitHub, then)
  git remote add origin https://github.com/<user>/<repo>.git
  git push -u origin main
  git switch -c development && git push -u origin development

Branches: main (stable, demo-ready), development (integration), and one
feature branch per member:
  feature/nathan-core-desktop   feature/kim-taskbar-apps
  feature/byron-taskmanager     feature/lorens-ui-framework
Workflow:
  1. git switch development && git pull
  2. git switch -c feature/<name>-<topic>   (or reuse your branch and
     merge development into it)
  3. Commit small steps:  "<area>: <what>"  e.g. "taskmgr: add process table"
  4. Push, open a Pull Request into development, and ask one other
     member to review it (does it build? does the app still start/exit?).
  5. After merging, everyone pulls development, rebuilds, and re-tests.
  Do not edit another member's files. Changes to AppState.h or
  Application.cpp go through Nathan. Agree them in chat first.
  On merge conflicts: keep both sides' intent, rebuild, run the app, and
  ask the file's owner if unsure.
