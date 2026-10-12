CSOPESY MCO4 - Desktop-Style OS Mock-Up
========================================

Members
-------
  Ang, Byron Scott
  Laborada, Nathan
  Sotingco, Kimbery Wynelle
  Tee, John Lorens

Entry file
----------
  src/main.cc
      Contains function main().
      It creates the Application object, starts the application, and runs
      the main loop.

Build requirements
------------------
  Windows 10 or Windows 11, 64-bit
  Visual Studio Community 2026 with the "Desktop development with C++" workload
  CMake 3.24 or later
  Ninja and Git
  Internet access for the first setup
  A graphics driver that supports OpenGL 3.3

Build
-----
  In Visual Studio:
  1. Select File > Open > Folder.
  2. Select the folder that contains CMakeLists.txt.
  3. Wait for CMake to finish the first setup. CMake downloads GLFW and
     Dear ImGui during this step.
  4. Select x64 Debug or x64 Release.
  5. Select Build > Build All.

  Or use Developer PowerShell for Visual Studio 2026. Change to the project
  folder and run:

      cmake --preset x64-debug
      cmake --build --preset x64-debug

  Use x64-release instead of x64-debug for a Release build.

Run
---
  In Visual Studio, select DesktopOS.exe as the startup item and press F5.

  Or run this file from Developer PowerShell:

      .\out\build\x64-debug\DesktopOS.exe

  To close the program, click the PWR button on the taskbar. You can also
  close the window with its X button.

Dependencies
------------
  CMake downloads these libraries during the first setup:
  GLFW 3.4
  Dear ImGui v1.91.9b

  Windows provides OpenGL.

Features
--------
  Desktop wallpaper with three styles
  Live clock and date
  PWR button to close the program
  Taskbar buttons for File Explorer, Settings, and Task Manager
  Sample file and folder list
  Sample system and display settings
  Wallpaper and color theme choices
  Process table with sample CPU and memory values

The program uses sample data for system and process information.
