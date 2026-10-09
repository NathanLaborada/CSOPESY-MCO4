// Desktop background layer: the wallpaper drawn behind every window.

#ifndef DESKTOP_OS_UI_DESKTOP_H_
#define DESKTOP_OS_UI_DESKTOP_H_

#include "core/app_state.h"

namespace desktop_os {
namespace desktop {

inline constexpr int kWallpaperCount = 3;

// Display names indexed by AppState::wallpaper_index, for the Settings picker.
inline constexpr const char* kWallpaperNames[kWallpaperCount] = {
    "Midnight", "Aurora", "Slate"};

// Draws the wallpaper selected by `state.wallpaper_index` behind every
// window. Call once per frame, before any other module.
void Render(const AppState& state);

}  // namespace desktop
}  // namespace desktop_os

#endif  // DESKTOP_OS_UI_DESKTOP_H_
