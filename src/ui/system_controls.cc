#include "ui/system_controls.h"

#include <chrono>
#include <cstdio>
#include <ctime>

#include "imgui.h"

#include "core/app_state.h"
#include "ui/widgets.h"

namespace desktop_os {
namespace system_controls {
namespace {

constexpr float kButtonSize = 34.0f;
constexpr float kRightMargin = 8.0f;
constexpr float kGap = 10.0f;
constexpr float kNarrowWidth = 600.0f;

// Format the local time and date as text.
void CurrentTimeStrings(char* time_out, std::size_t time_size, char* date_out,
                        std::size_t date_size) {
  const std::time_t now_seconds =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::tm local_time = {};
#if defined(_WIN32)
  localtime_s(&local_time, &now_seconds);
#else
  localtime_r(&now_seconds, &local_time);
#endif
  std::strftime(time_out, time_size, "%H:%M:%S", &local_time);
  std::strftime(date_out, date_size, "%Y-%m-%d", &local_time);
}

}  // namespace

// Draw the clock and PWR button in the taskbar area.
void Render(AppState& state) {
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  float width = kSystemControlsWidth;
  const float half_view = viewport->Size.x * 0.5f;
  if (width > half_view) width = half_view;
  const ImVec2 window_pos(viewport->Pos.x + viewport->Size.x - width,
                          viewport->Pos.y + viewport->Size.y - kTaskbarHeight);
  const ImVec2 window_size(width, kTaskbarHeight);

  ImGui::SetNextWindowPos(window_pos);
  ImGui::SetNextWindowSize(window_size);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 7.0f));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

  constexpr ImGuiWindowFlags kFlags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;
  const bool open = ImGui::Begin("##system_controls", nullptr, kFlags);
  if (open) {
    char time_text[16] = "--:--:--";
    char date_text[32] = "----/--/--";
    CurrentTimeStrings(time_text, sizeof(time_text), date_text,
                       sizeof(date_text));

    const bool narrow = viewport->Size.x < kNarrowWidth;
    const float text_width =
        ImGui::CalcTextSize(narrow ? time_text : date_text).x;
    const float cursor_y =
        (kTaskbarHeight -
         ImGui::GetTextLineHeightWithSpacing() * (narrow ? 1.0f : 2.0f)) *
            0.5f -
        2.0f;
    ImGui::SetCursorPos(ImVec2(
        window_size.x - text_width - kButtonSize - kRightMargin - kGap * 2.0f,
        cursor_y > 4.0f ? cursor_y : 4.0f));
    ImGui::BeginGroup();
    ImGui::TextUnformatted(time_text);
    if (!narrow) {
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.62f, 0.65f, 0.72f, 1.0f));
      ImGui::TextUnformatted(date_text);
      ImGui::PopStyleColor();
    }
    ImGui::EndGroup();

    ImGui::SameLine();
    const float button_y = (kTaskbarHeight - kButtonSize) * 0.5f;
    ImGui::SetCursorPos(
        ImVec2(window_size.x - kButtonSize - kRightMargin, button_y));
    if (widgets::IconButton("pwr", widgets::IconKind::kPower,
                            ImVec2(kButtonSize, kButtonSize),
                            /*active=*/false)) {
      state.shutdown_requested = true;
    }
    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip("Shut down (PWR)");
    }
  }
  ImGui::End();
  ImGui::PopStyleColor();
  ImGui::PopStyleVar(3);
}

}  // namespace system_controls
}  // namespace desktop_os
