// Shared state for the UI modules.

#ifndef DESKTOP_OS_CORE_APP_STATE_H_
#define DESKTOP_OS_CORE_APP_STATE_H_

namespace desktop_os {

// Taskbar height in pixels. Other windows stay above this area.
inline constexpr float kTaskbarHeight = 48.0f;

// Application owns one AppState and passes it to each UI module. The taskbar opens app windows. Each app closes its own window. PWR sets shutdown_requested. Settings updates wallpaper_index and theme_index. Tell the group before you change this shared structure.
struct AppState {
  bool show_file_explorer = false;
  bool show_settings = false;
  bool show_task_manager = false;

  bool shutdown_requested = false;

  int wallpaper_index = 0;  // 0 to desktop::kWallpaperCount - 1.
  int theme_index = 0;      // Meaning defined by theme (Lorens).
};

}  // namespace desktop_os

#endif  // DESKTOP_OS_CORE_APP_STATE_H_
