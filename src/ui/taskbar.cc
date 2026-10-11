#include "ui/taskbar.h"

#include "imgui.h"
#include "ui/system_controls.h"
#include "ui/widgets.h"

namespace desktop_os {
namespace taskbar {
namespace {

// Use 32 px buttons in the 48 px taskbar. Show an indicator below each open app.
constexpr float kLeftPadding = 10.0f;
constexpr float kTopPadding = 6.0f;
constexpr float kButtonSize = 32.0f;
constexpr float kButtonGap = 6.0f;
constexpr float kIndicatorWidth = 18.0f;
constexpr float kIndicatorHeight = 3.0f;
constexpr float kIndicatorGap = 4.0f;
constexpr float kIndicatorRadius = 1.5f;
constexpr float kTooltipGap = 4.0f;

// Use the same blue accent as the theme.
constexpr ImU32 kIndicatorColor = IM_COL32(74, 123, 224, 255);

// Use a semi-transparent dark background for the bar.
const ImVec4 kBarBackground(0.06f, 0.07f, 0.12f, 0.92f);

struct LaunchButton {
  const char* id;
  const char* label;  // Hover tooltip.
  widgets::IconKind kind;
  bool AppState::* show;
};

// Place all app buttons before the system controls.
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

  // Keep the bar fixed and behind the system controls. Render system controls before the taskbar so the taskbar does not cover them.
  constexpr ImGuiWindowFlags kFlags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
      ImGuiWindowFlags_NoBringToFrontOnFocus |
      ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNavFocus |
      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

  ImGui::Begin("Taskbar", nullptr, kFlags);

  // Keep app buttons clear of the clock and PWR area.
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
    if (ImGui::IsItemHovered()) {
      // Show tooltips above the taskbar.
      ImGui::SetNextWindowPos(ImVec2(button_pos.x, bar_pos.y - kTooltipGap),
                              ImGuiCond_Always, ImVec2(0.0f, 1.0f));
      ImGui::SetTooltip("%s", button.label);
    }
    if (open) {
      // Show an indicator below each open app button.
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
