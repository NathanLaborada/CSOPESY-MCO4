CSOPESY MCO4 - DESKTOP-STYLE OS MOCK-UP
========================================

GROUP MEMBERS
-------------
Ang, Byron Scott
Laborada, Nathan
Sotingco, Kimbery Wynelle
Tee, John Lorens

PROGRAM PURPOSE
---------------
This program shows a simulated desktop environment. It includes a desktop,
a taskbar, File Explorer, Settings, and Task Manager.

The program does not read real files, change system settings, or read real
process data. These screens use sample data.

PROGRAM ENTRY
-------------
File: src/main.cc
Function: main()

The main() function creates the Application object. It calls Init() to start
the program and Run() to show the desktop.

REQUIREMENTS
------------
- Windows 10 or Windows 11, 64-bit
- Visual Studio Community 2026 with the "Desktop development with C++" workload
- CMake and Ninja
- Git
- Internet access for the first build
- A graphics driver that supports OpenGL 3.3

BUILD AND RUN IN VISUAL STUDIO
------------------------------
1. Open Visual Studio.
2. Select File > Open > Folder.
3. Select the project folder. It contains CMakeLists.txt.
4. Wait for CMake to finish. The first setup downloads GLFW and Dear ImGui.
5. Select Windows x64 Debug or Windows x64 Release.
6. Select Build > Build All.
7. Select DesktopOS.exe as the startup item.
8. Press F5 to start the program.

To close the program, click the PWR button on the taskbar. You can also
close the window with its X button.

BUILD FROM A COMMAND LINE
-------------------------
Open Developer PowerShell for Visual Studio 2026. Change to the project
folder and run these commands:

    cmake --preset x64-debug
    cmake --build --preset x64-debug
    .\out\build\x64-debug\DesktopOS.exe

Use x64-release instead of x64-debug for a Release build. Use Developer
PowerShell so that the Visual Studio compiler is available.

DEPENDENCIES
------------
CMake downloads these libraries during the first setup:
- GLFW 3.4
- Dear ImGui v1.91.9b

Windows provides OpenGL.

FEATURES
--------
- Desktop wallpaper with three styles
- Live clock and date
- PWR button to close the program
- Taskbar buttons for File Explorer, Settings, and Task Manager
- Sample file and folder list
- Sample system and display settings
- Wallpaper and color theme choices
- Process table with sample CPU and memory values

All system and process data in this program is sample data.
