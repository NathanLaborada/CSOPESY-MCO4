#pragma once

// Shared application state. ONE instance lives inside Application and is
// passed by reference to every module's render function each frame.
//
// Ownership (who WRITES each field). Anyone may READ any field.
//   show*             : Taskbar (Kim) opens; each app window closes itself
//   shutdownRequested : PWR button (Lorens); Application::run exits on it
//   wallpaperIndex    : Settings (Kim); read by Desktop (Nathan)
//   themeIndex        : Settings (Kim); read by Theme (Lorens)
//
// Changing this struct is a shared-interface change: tell the group first and
// record it in SESSION_HANDOFF.md.
struct AppState {
    bool showFileExplorer = false;
    bool showSettings = false;
    bool showTaskManager = false;

    bool shutdownRequested = false;

    int wallpaperIndex = 0; // 0..Desktop::kWallpaperCount-1
    int themeIndex = 0;     // meaning defined by Theme (Lorens)
};

// Height of the fixed bottom taskbar. Desktop and app windows use it to keep
// clear of the taskbar; Taskbar draws itself exactly this tall.
inline constexpr float kTaskbarHeight = 48.0f;
