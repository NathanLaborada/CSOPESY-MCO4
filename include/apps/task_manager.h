// Task Manager window with a sample process table.

#ifndef DESKTOP_OS_APPS_TASK_MANAGER_H_
#define DESKTOP_OS_APPS_TASK_MANAGER_H_

#include "core/app_state.h"

namespace desktop_os {
namespace task_manager {

// Draw Task Manager when state.show_task_manager is true. The table shows sample process names, CPU values, and memory values. The X button closes the window. The taskbar can open it again.
void Render(AppState& state);

}  // namespace task_manager
}  // namespace desktop_os

#endif  // DESKTOP_OS_APPS_TASK_MANAGER_H_
