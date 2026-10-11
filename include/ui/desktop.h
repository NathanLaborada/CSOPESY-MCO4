// Desktop wallpaper drawn behind all windows.

#ifndef DESKTOP_OS_UI_DESKTOP_H_
#define DESKTOP_OS_UI_DESKTOP_H_

#include "core/app_state.h"

namespace desktop_os {
namespace desktop {

inline constexpr int kWallpaperCount = 3;

// Names for the wallpapers shown in Settings.
inline constexpr const char* kWallpaperNames[kWallpaperCount] = {
    "Midnight", "Aurora", "Slate"};

// Draw the selected wallpaper. Call this before the other UI modules.
void Render(const AppState& state);

}  // namespace desktop
}  // namespace desktop_os

#endif  // DESKTOP_OS_UI_DESKTOP_H_
