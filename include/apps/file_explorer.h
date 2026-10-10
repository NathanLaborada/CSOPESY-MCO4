// File Explorer window: sidebar, breadcrumb bar and a placeholder listing.

#ifndef DESKTOP_OS_APPS_FILE_EXPLORER_H_
#define DESKTOP_OS_APPS_FILE_EXPLORER_H_

#include "core/app_state.h"

namespace desktop_os {
namespace file_explorer {

// Draws the File Explorer window while state.show_file_explorer is true.
// The folder tree is simulated in-memory placeholder data; the module never
// touches the real filesystem. The window clamps itself above the taskbar.
void Render(AppState& state);

}  // namespace file_explorer
}  // namespace desktop_os

#endif  // DESKTOP_OS_APPS_FILE_EXPLORER_H_
