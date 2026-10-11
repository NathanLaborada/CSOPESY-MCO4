// Reusable icons and labels for the UI.

#ifndef DESKTOP_OS_UI_WIDGETS_H_
#define DESKTOP_OS_UI_WIDGETS_H_

#include "imgui.h"

namespace desktop_os {
namespace widgets {

// Icon types drawn with ImDrawList.
enum class IconKind {
  kFolder,
  kSettings,
  kTaskManager,
  kPower,
};

// Draw a clickable icon button. Keep id the same while the button is visible. size includes the clickable area and padding. Return true when the user clicks the button.
bool IconButton(const char* id, IconKind kind, ImVec2 size, bool active);

// Draw an icon at the current cursor. Move the cursor by size.
void Icon(IconKind kind, float size, ImU32 color);

// Draw a small, dim label.
void MutedText(const char* text);

}  // namespace widgets
}  // namespace desktop_os

#endif  // DESKTOP_OS_UI_WIDGETS_H_
