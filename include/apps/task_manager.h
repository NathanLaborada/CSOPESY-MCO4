// Task Manager window: simulated process table with CPU and memory columns.

#ifndef DESKTOP_OS_APPS_TASK_MANAGER_H_
#define DESKTOP_OS_APPS_TASK_MANAGER_H_

#include "core/app_state.h"

namespace desktop_os {
namespace task_manager {

// Draws the Task Manager window while state.show_task_manager is true.
// A "Processes" section lists dummy processes (name, status, CPU %,
// memory in MB) in an ImGui table. All values are simulated placeholder
// data — nothing here queries the real system. The window's X button sets
// state.show_task_manager to false; the taskbar can set it true again.
// The window clamps itself above the taskbar.
void Render(AppState& state);

}  // namespace task_manager
}  // namespace desktop_os

#endif  // DESKTOP_OS_APPS_TASK_MANAGER_H_
