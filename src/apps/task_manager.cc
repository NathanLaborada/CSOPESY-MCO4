// Draw sample processes in an ImGui table.

#include "apps/task_manager.h"

#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstring>

#include "imgui.h"
#include "ui/widgets.h"

namespace desktop_os {
namespace task_manager {
namespace {

// Keep one window title to prevent duplicate windows.
constexpr const char* kWindowTitle = "Task Manager";

// Layout constants use the project naming style.
constexpr float kRowHeight = 24.0f;
constexpr float kRowTextPadY = 4.0f;  // same pad File Explorer uses
constexpr float kStatusColumnWidth = 92.0f;
constexpr float kCpuColumnWidth = 72.0f;
constexpr float kMemColumnWidth = 110.0f;

// These values set the window's first position and size.
constexpr float kDefaultPosX = 120.0f;
constexpr float kDefaultPosY = 72.0f;
constexpr float kDefaultWidth = 660.0f;
constexpr float kDefaultHeight = 440.0f;
constexpr float kMinWidth = 460.0f;
constexpr float kMinHeight = 280.0f;

// Vary each CPU value slightly by time and row. These values are sample data.
constexpr float kCpuWobbleAmplitude = 1.0f;
constexpr float kCpuWobbleSpeed = 0.9f;
constexpr float kCpuPhaseStep = 1.3f;

// Each Process has a name, status, CPU value, and memory value. Edit the array to change the sample data.
struct Process {
  const char* name;
  const char* status;  // "Running" or "Suspended".
  float cpu_base;
  float mem_mb;
};

// Add or edit sample processes in this table.
const Process kProcesses[] = {
    {"System", "Running", 0.8f, 48.0f},
    {"Desktop Window Manager", "Running", 2.4f, 96.5f},
    {"csopesy_shell.exe", "Running", 1.6f, 74.2f},
    {"File Explorer", "Running", 0.9f, 58.3f},
    {"Settings", "Suspended", 0.0f, 32.7f},
    {"Task Manager", "Running", 1.2f, 41.6f},
    {"web_browser.exe", "Running", 12.5f, 420.8f},
    {"media_player.exe", "Suspended", 0.0f, 66.4f},
    {"notepad.exe", "Running", 0.3f, 18.9f},
    {"antivirus_service.exe", "Running", 3.1f, 152.0f},
    {"update_helper.exe", "Running", 0.5f, 24.6f},
    {"print_spooler.exe", "Running", 0.2f, 12.1f},
};
constexpr int kProcessCount =
    static_cast<int>(sizeof(kProcesses) / sizeof(kProcesses[0]));

// Calculate each CPU value from its base value and a small sine wave. Suspended processes use 0%.
bool IsSuspended(const Process& process) {
  return std::strcmp(process.status, "Suspended") == 0;
}

float ProcessCpu(const Process& process, int index) {
  if (IsSuspended(process)) return 0.0f;
  const float time = static_cast<float>(ImGui::GetTime());
  const float wobble =
      kCpuWobbleAmplitude * std::sin(time * kCpuWobbleSpeed +
                                     static_cast<float>(index) * kCpuPhaseStep);
  float cpu = process.cpu_base + wobble;
  if (cpu < 0.0f) cpu = 0.0f;
  if (cpu > 100.0f) cpu = 100.0f;
  return cpu;
}

// Show the process count and total CPU and memory above the table.
void DrawSummary() {
  float total_cpu = 0.0f;
  float total_mem = 0.0f;
  for (int i = 0; i < kProcessCount; ++i) {
    total_cpu += ProcessCpu(kProcesses[i], i);
    total_mem += kProcesses[i].mem_mb;
  }
  ImGui::Text("Processes: %d", kProcessCount);
  ImGui::SameLine();
  ImGui::Text("CPU: %.1f%%", total_cpu);
  ImGui::SameLine();
  ImGui::Text("Memory: %.1f MB", total_mem);
  widgets::MutedText("Simulated dummy data, not actual system usage.");
}

// Draw one process row. Align numbers to the right in fixed-width columns.
void DrawValueCell(float value) {
  char label[32];
  std::snprintf(label, sizeof(label), "%.1f", value);
  const float text_width = ImGui::CalcTextSize(label).x;
  const float spare = ImGui::GetContentRegionAvail().x - text_width;
  if (spare > 0.0f) {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + spare);
  }
  ImGui::TextUnformatted(label);
}

void DrawProcessRow(const Process& process, int index) {
  ImGui::TableNextRow(ImGuiTableRowFlags_None, kRowHeight);

  ImGui::TableSetColumnIndex(0);
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + kRowTextPadY);
  ImGui::TextUnformatted(process.name);

  ImGui::TableSetColumnIndex(1);
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + kRowTextPadY);
  const bool suspended = IsSuspended(process);
  if (suspended) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.51f, 0.54f, 0.60f, 1.00f));
  }
  ImGui::TextUnformatted(process.status);
  if (suspended) ImGui::PopStyleColor();

  ImGui::TableSetColumnIndex(2);
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + kRowTextPadY);
  DrawValueCell(ProcessCpu(process, index));

  ImGui::TableSetColumnIndex(3);
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + kRowTextPadY);
  DrawValueCell(process.mem_mb);
}

// Draw the Processes table with four columns.
void DrawProcessTable() {
  widgets::MutedText("Processes");
  const float table_height = ImGui::GetContentRegionAvail().y;
  if (ImGui::BeginTable("##processes", 4,
                        ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_RowBg |
                            ImGuiTableFlags_Resizable |
                            ImGuiTableFlags_ScrollY |
                            ImGuiTableFlags_SizingStretchProp,
                        ImVec2(0.0f, table_height))) {
    ImGui::TableSetupScrollFreeze(0, 1);
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed,
                            kStatusColumnWidth);
    ImGui::TableSetupColumn("CPU %", ImGuiTableColumnFlags_WidthFixed,
                            kCpuColumnWidth);
    ImGui::TableSetupColumn("Memory (MB)", ImGuiTableColumnFlags_WidthFixed,
                            kMemColumnWidth);
    ImGui::TableHeadersRow();

    for (int i = 0; i < kProcessCount; ++i) {
      DrawProcessRow(kProcesses[i], i);
    }
    ImGui::EndTable();
  }
}

// Keep the window above the taskbar.
void ClampAboveTaskbar() {
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  const float limit_y = viewport->Pos.y + viewport->Size.y - kTaskbarHeight;

  ImVec2 pos = ImGui::GetWindowPos();
  ImVec2 size = ImGui::GetWindowSize();
  if (pos.y + size.y <= limit_y) return;

  pos.y = limit_y - size.y;
  if (pos.y < viewport->Pos.y) {
    pos.y = viewport->Pos.y;
    size.y = limit_y - pos.y;
    ImGui::SetWindowSize(size);
  }
  ImGui::SetWindowPos(pos);
}

}  // namespace

void Render(AppState& state) {
  if (!state.show_task_manager) return;

  ImGui::SetNextWindowPos(ImVec2(kDefaultPosX, kDefaultPosY),
                          ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(kDefaultWidth, kDefaultHeight),
                           ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSizeConstraints(ImVec2(kMinWidth, kMinHeight),
                                      ImVec2(FLT_MAX, FLT_MAX));

  const bool visible = ImGui::Begin(kWindowTitle, &state.show_task_manager);
  ClampAboveTaskbar();
  if (!visible) {
    ImGui::End();
    return;
  }

  DrawSummary();
  ImGui::Spacing();
  DrawProcessTable();

  ImGui::End();
}

}  // namespace task_manager
}  // namespace desktop_os
