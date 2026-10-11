// Shared desktop theme: dark colors, spacing and rounding.

#ifndef DESKTOP_OS_UI_THEME_H_
#define DESKTOP_OS_UI_THEME_H_

namespace desktop_os {
namespace theme {

// Number of built-in themes. Settings (Kim) uses this plus kThemeNames for
// the theme picker and writes the choice to AppState::theme_index.
inline constexpr int kThemeCount = 2;

// Display names indexed by theme. Keep in sync with Apply().
inline constexpr const char* kThemeNames[kThemeCount] = {"Midnight", "Slate"};

// Applies the theme selected by `theme_index`. Any out-of-range value falls
// back to theme 0. Call once during init AND again whenever theme_index
// changes (e.g. after Settings writes it); otherwise a picker change updates
// the stored index without changing the appearance. Safe to call before the
// first frame and cheap enough to call every frame if the host prefers.
void Apply(int theme_index);

}  // namespace theme
}  // namespace desktop_os

#endif  // DESKTOP_OS_UI_THEME_H_
