#pragma once

#include "core/AppState.h"

namespace Desktop {

inline constexpr int kWallpaperCount = 3;

// Display names, indexed by AppState::wallpaperIndex (for the Settings picker).
extern const char* const kWallpaperNames[kWallpaperCount];

// Draws the wallpaper behind every window. Call once per frame, first.
void Render(const AppState& state);

} // namespace Desktop
