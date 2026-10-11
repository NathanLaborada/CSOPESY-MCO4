// Settings window with categories and sample system information.

#ifndef DESKTOP_OS_APPS_SETTINGS_H_
#define DESKTOP_OS_APPS_SETTINGS_H_

#include "core/app_state.h"

namespace desktop_os {
namespace settings {

// Draw Settings when state.show_settings is true. All information is sample data. The wallpaper and theme choices update AppState. The window stays above the taskbar.
void Render(AppState& state);

}  // namespace settings
}  // namespace desktop_os

#endif  // DESKTOP_OS_APPS_SETTINGS_H_
