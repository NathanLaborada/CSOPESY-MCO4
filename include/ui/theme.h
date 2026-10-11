// Colors and spacing for the desktop UI.

#ifndef DESKTOP_OS_UI_THEME_H_
#define DESKTOP_OS_UI_THEME_H_

namespace desktop_os {
namespace theme {

// Number of themes. Settings uses this value and kThemeNames to show choices.
inline constexpr int kThemeCount = 2;

// Theme names shown in Settings. Keep the names in the same order as Apply().
inline constexpr const char* kThemeNames[kThemeCount] = {"Midnight", "Slate"};

// Apply the selected theme. Use theme 0 for an invalid index. Call this at startup and when the selected theme changes.
void Apply(int theme_index);

}  // namespace theme
}  // namespace desktop_os

#endif  // DESKTOP_OS_UI_THEME_H_
