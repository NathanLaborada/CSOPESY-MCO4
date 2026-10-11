CSOPESY MCO4 - DESKTOP-STYLE OS MOCK-UP
========================================

PURPOSE
-------
This program shows a simulated desktop environment. It includes a desktop,
a taskbar, File Explorer, Settings, and Task Manager.

The program does not read real files, change system settings, or read real
process data. The file list, settings, and process values are sample data.

REQUIREMENTS
------------
- Windows 10 or Windows 11, 64-bit
- Visual Studio Community with the "Desktop development with C++" workload
- CMake, Ninja, and Git
- Internet access for the first configuration
- A graphics driver that supports OpenGL 3.3

BUILD AND RUN
-------------
1. Open Visual Studio.
2. Select File > Open > Folder.
3. Select the folder that contains CMakeLists.txt.
4. Wait for CMake to finish the first configuration. CMake downloads GLFW
   and Dear ImGui during this step.
5. Select x64 Debug or x64 Release in the configuration menu.
6. Select Build > Build All.
7. Select DesktopOS.exe as the startup item.
8. Press F5 to start the program.

To close the program, click the PWR button in the taskbar. You can also use
the window close button.

PROGRAM ENTRY
-------------
The main() function is in src/main.cc.

PROJECT FILES
-------------
CMakeLists.txt       Build settings and dependencies
CMakePresets.json    Debug and Release build settings
src/                 C++ source files
include/             C++ header files
README.txt           Build and run instructions

DEPENDENCIES
------------
CMake downloads these libraries during the first configuration:
- GLFW 3.4
- Dear ImGui v1.91.9b

OpenGL is provided by Windows.

FEATURES
--------
- Desktop wallpaper with three styles
- Clock and date
- PWR button to close the program
- Taskbar buttons for File Explorer, Settings, and Task Manager
- Simulated file and folder list
- Simulated system and display settings
- Wallpaper and color theme choices
- Process table with sample CPU and memory values

All system data in this program is simulated.
