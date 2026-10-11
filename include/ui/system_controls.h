// Clock and PWR button on the right side of the taskbar.

#ifndef DESKTOP_OS_UI_SYSTEM_CONTROLS_H_
#define DESKTOP_OS_UI_SYSTEM_CONTROLS_H_

#include "core/app_state.h"

namespace desktop_os {
namespace system_controls {

// Space reserved for the clock and PWR button. Keep app buttons out of this area.
inline constexpr float kSystemControlsWidth = 260.0f;

// Draw the current time, date, and PWR button. PWR sets shutdown_requested. The main loop then exits.
void Render(AppState& state);

}  // namespace system_controls
}  // namespace desktop_os

#endif  // DESKTOP_OS_UI_SYSTEM_CONTROLS_H_
