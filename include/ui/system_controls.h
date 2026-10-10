// Live clock and PWR button at the right end of the taskbar area.

#ifndef DESKTOP_OS_UI_SYSTEM_CONTROLS_H_
#define DESKTOP_OS_UI_SYSTEM_CONTROLS_H_

#include "core/app_state.h"

namespace desktop_os {
namespace system_controls {

// Horizontal space reserved at the right end of the taskbar for the clock
// and PWR button. Kim: keep taskbar launch buttons clear of this strip so
// the two modules never overlap or intercept each other's clicks.
inline constexpr float kSystemControlsWidth = 260.0f;

// Draws HH:MM:SS, the local date and the PWR button. Clicking PWR sets
// `state.shutdown_requested = true`; Application::Run() then exits and the
// destructor performs the orderly shutdown.
void Render(AppState& state);

}  // namespace system_controls
}  // namespace desktop_os

#endif  // DESKTOP_OS_UI_SYSTEM_CONTROLS_H_
