// Fixed bottom taskbar with the three app launch buttons.

#ifndef DESKTOP_OS_UI_TASKBAR_H_
#define DESKTOP_OS_UI_TASKBAR_H_

#include "core/app_state.h"

namespace desktop_os {
namespace taskbar {

// Draws the fixed, full-width bottom taskbar (exactly kTaskbarHeight tall).
// Its three icon buttons set state.show_file_explorer / show_settings /
// show_task_manager to true; each button shows an open-indicator while the
// matching app window is visible. The rightmost
// system_controls::kSystemControlsWidth pixels stay free for the clock
// and PWR button.
void Render(AppState& state);

}  // namespace taskbar
}  // namespace desktop_os

#endif  // DESKTOP_OS_UI_TASKBAR_H_
