// Settings window: category list, simulated system info, pickers.

#ifndef DESKTOP_OS_APPS_SETTINGS_H_
#define DESKTOP_OS_APPS_SETTINGS_H_

#include "core/app_state.h"

namespace desktop_os {
namespace settings {

// Draws the Settings window while state.show_settings is true. Categories:
// System, Display, Personalization, About. All information is simulated
// placeholder data. The wallpaper picker writes state.wallpaper_index; the
// theme picker writes state.theme_index (visible only after Nathan wires
// theme::Apply into Application). The window clamps above the taskbar.
void Render(AppState& state);

}  // namespace settings
}  // namespace desktop_os

#endif  // DESKTOP_OS_APPS_SETTINGS_H_
