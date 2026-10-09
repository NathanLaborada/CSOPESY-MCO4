// Shared state passed to every UI module each frame.

#ifndef DESKTOP_OS_CORE_APP_STATE_H_
#define DESKTOP_OS_CORE_APP_STATE_H_

namespace desktop_os {

// Height in pixels of the fixed bottom taskbar. The desktop and the app
// windows use it to keep clear of the taskbar; the taskbar draws itself
// exactly this tall.
inline constexpr float kTaskbarHeight = 48.0f;

// State shared between modules. Exactly one instance exists, owned by
// Application and passed by reference to each module's Render() every frame.
//
// Ownership (who WRITES each field; anyone may read any field):
//   show_*              taskbar (Kim) opens; each app window closes itself
//   shutdown_requested  PWR button (Lorens); Application::Run() exits on it
//   wallpaper_index     settings (Kim); read by desktop (Nathan)
//   theme_index         settings (Kim); read by theme (Lorens)
//
// Changing this struct is a shared-interface change: tell the group first and
// record it in SESSION_HANDOFF.md.
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
