// Bottom taskbar with three app buttons.

#ifndef DESKTOP_OS_UI_TASKBAR_H_
#define DESKTOP_OS_UI_TASKBAR_H_

#include "core/app_state.h"

namespace desktop_os {
namespace taskbar {

// Draw the taskbar and app buttons. Buttons open File Explorer, Settings, or Task Manager. Show an indicator for each open window. Leave space on the right for the clock and PWR button.
void Render(AppState& state);

}  // namespace taskbar
}  // namespace desktop_os

#endif  // DESKTOP_OS_UI_TASKBAR_H_
