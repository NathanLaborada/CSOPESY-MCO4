#include "ui/taskbar.h"

#include "imgui.h"
#include "ui/system_controls.h"
#include "ui/widgets.h"

namespace desktop_os {
namespace taskbar {
namespace {

// Layout inside the 48 px bar: 32 px buttons starting at the left edge,
// with a small open-indicator bar centered underneath each button.
constexpr float kLeftPadding = 10.0f;
constexpr float kTopPadding = 6.0f;
constexpr float kButtonSize = 32.0f;
constexpr float kButtonGap = 6.0f;
constexpr float kIndicatorWidth = 18.0f;
constexpr float kIndicatorHeight = 3.0f;
constexpr float kIndicatorGap = 4.0f;
constexpr float kIndicatorRadius = 1.5f;

// Single restrained blue accent, matching theme::Apply() (#4A7BE0).
constexpr ImU32 kIndicatorColor = IM_COL32(74, 123, 224, 255);

// Semi-opaque dark bar (PROJECT_PLAN.md section 7).
const ImVec4 kBarBackground(0.06f, 0.07f, 0.12f, 0.92f);

struct LaunchButton {
  const char* id;
  const char* label;  // Hover tooltip.
  widgets::IconKind kind;
  bool AppState::* show;
};

// Left-to-right order; all buttons sit left of the system_controls strip.
constexpr LaunchButton kLaunchButtons[] = {
    {"file_explorer", "File Explorer", widgets::IconKind::kFolder,
     &AppState::show_file_explorer},
    {"settings", "Settings", widgets::IconKind::kSettings,
     &AppState::show_settings},
    {"task_manager", "Task Manager", widgets::IconKind::kTaskManager,
     &AppState::show_task_manager},
};

}  // namespace

void Render(AppState& state) {
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  const ImVec2 bar_size(viewport->WorkSize.x, kTaskbarHeight);
  const ImVec2 bar_pos(
      viewport->WorkPos.x,
      viewport->WorkPos.y + viewport->WorkSize.y - kTaskbarHeight);

  ImGui::SetNextWindowPos(bar_pos);
  ImGui::SetNextWindowSize(bar_size);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, kBarBackground);

  // Not movable/resizable, never takes focus, never jumps in front of the
  // system_controls overlay. Application renders that overlay *before* this
  // bar on purpose: ImGui puts a new NoBringToFrontOnFocus window at the
  // back, so the window created last is drawn lowest.
  constexpr ImGuiWindowFlags kFlags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
      ImGuiWindowFlags_NoBringToFrontOnFocus |
      ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNavFocus |
      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

  ImGui::Begin("Taskbar", nullptr, kFlags);

  // Launch buttons must stay clear of the clock/PWR strip on the right.
  const float limit_x = bar_size.x - system_controls::kSystemControlsWidth;
  float x = kLeftPadding;
  for (const LaunchButton& button : kLaunchButtons) {
    if (x + kButtonSize > limit_x) break;
    ImGui::SetCursorPos(ImVec2(x, kTopPadding));
    const bool open = state.*(button.show);
    const ImVec2 button_pos = ImGui::GetCursorScreenPos();
    if (widgets::IconButton(button.id, button.kind,
                            ImVec2(kButtonSize, kButtonSize), open)) {
      state.*(button.show) = true;  // Clicking opens; it never toggles closed.
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s", button.label);
    if (open) {
      // Open indicator centered under the button (requirements B1-B3).
      const float indicator_x =
          button_pos.x + (kButtonSize - kIndicatorWidth) * 0.5f;
      const float indicator_y = button_pos.y + kButtonSize + kIndicatorGap;
      ImGui::GetWindowDrawList()->AddRectFilled(
          ImVec2(indicator_x, indicator_y),
          ImVec2(indicator_x + kIndicatorWidth, indicator_y + kIndicatorHeight),
          kIndicatorColor, kIndicatorRadius);
    }
    x += kButtonSize + kButtonGap;
  }

  ImGui::End();
  ImGui::PopStyleColor();
  ImGui::PopStyleVar(3);
}

}  // namespace taskbar
}  // namespace desktop_os
