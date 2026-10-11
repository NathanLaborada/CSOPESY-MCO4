// File Explorer window with a sidebar and a sample file list.

#ifndef DESKTOP_OS_APPS_FILE_EXPLORER_H_
#define DESKTOP_OS_APPS_FILE_EXPLORER_H_

#include "core/app_state.h"

namespace desktop_os {
namespace file_explorer {

// Draw the window when state.show_file_explorer is true. The folder tree is sample data. The window stays above the taskbar.
void Render(AppState& state);

}  // namespace file_explorer
}  // namespace desktop_os

#endif  // DESKTOP_OS_APPS_FILE_EXPLORER_H_
