// Reusable drawn widgets shared by the taskbar and application screens.

#ifndef DESKTOP_OS_UI_WIDGETS_H_
#define DESKTOP_OS_UI_WIDGETS_H_

#include "imgui.h"

namespace desktop_os {
namespace widgets {

// Geometric icon kinds drawn with ImDrawList. No emoji or custom fonts.
enum class IconKind {
  kFolder,
  kSettings,
  kTaskManager,
  kPower,
};

// Clickable icon button with hover, pressed (held) and active/selected
// states. `id` must be stable while the button is visible. `size` is the
// full clickable area including padding. Returns true when clicked.
// The icon is drawn geometrically, so it works without extra fonts.
bool IconButton(const char* id, IconKind kind, ImVec2 size, bool active);

// Draws a single geometric icon at the current cursor position. Advances the
// cursor by `size`. Useful for inline listing icons (e.g. File Explorer).
void Icon(IconKind kind, float size, ImU32 color);

// Small dimmed label for section headers and helper text.
void MutedText(const char* text);

}  // namespace widgets
}  // namespace desktop_os

#endif  // DESKTOP_OS_UI_WIDGETS_H_
