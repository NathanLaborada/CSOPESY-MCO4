#include "ui/theme.h"

#include "imgui.h"

namespace desktop_os {
namespace theme {
namespace {

// Use one blue accent in both themes.
ImVec4 AccentBlue() { return ImVec4(0.29f, 0.48f, 0.88f, 1.00f); }

void ApplySpacing(ImGuiStyle& style) {
  style.WindowRounding = 10.0f;
  style.ChildRounding = 8.0f;
  style.FrameRounding = 6.0f;
  style.PopupRounding = 8.0f;
  style.ScrollbarRounding = 8.0f;
  style.GrabRounding = 6.0f;
  style.TabRounding = 6.0f;
  style.WindowBorderSize = 1.0f;
  style.ChildBorderSize = 1.0f;
  style.FrameBorderSize = 0.0f;
  style.PopupBorderSize = 1.0f;
  style.WindowPadding = ImVec2(12.0f, 10.0f);
  style.FramePadding = ImVec2(10.0f, 6.0f);
  style.ItemSpacing = ImVec2(8.0f, 6.0f);
  style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
  style.ScrollbarSize = 14.0f;
  style.GrabMinSize = 12.0f;
}

void ApplyMidnight(ImGuiStyle& style) {
  ImVec4* colors = style.Colors;
  const ImVec4 accent = AccentBlue();
  colors[ImGuiCol_Text] = ImVec4(0.93f, 0.94f, 0.97f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.51f, 0.54f, 0.60f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.08f, 0.14f, 1.00f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.11f, 0.18f, 1.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.11f, 0.18f, 0.96f);
  colors[ImGuiCol_Border] = ImVec4(0.23f, 0.28f, 0.40f, 1.00f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.13f, 0.16f, 0.25f, 1.00f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.22f, 0.34f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.22f, 0.28f, 0.42f, 1.00f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.08f, 0.14f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.13f, 0.21f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.07f, 0.08f, 0.14f, 1.00f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.09f, 0.11f, 0.18f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.07f, 0.08f, 0.14f, 1.00f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.30f, 0.42f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = accent;
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.36f, 0.55f, 0.94f, 1.00f);
  colors[ImGuiCol_CheckMark] = accent;
  colors[ImGuiCol_SliderGrab] = accent;
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.36f, 0.55f, 0.94f, 1.00f);
  colors[ImGuiCol_Button] = ImVec4(0.16f, 0.20f, 0.32f, 1.00f);
  colors[ImGuiCol_ButtonHovered] = accent;
  colors[ImGuiCol_ButtonActive] = ImVec4(0.22f, 0.38f, 0.75f, 1.00f);
  colors[ImGuiCol_Header] = ImVec4(0.16f, 0.20f, 0.32f, 1.00f);
  colors[ImGuiCol_HeaderHovered] = accent;
  colors[ImGuiCol_HeaderActive] = ImVec4(0.22f, 0.38f, 0.75f, 1.00f);
  colors[ImGuiCol_Separator] = ImVec4(0.23f, 0.28f, 0.40f, 1.00f);
  colors[ImGuiCol_SeparatorHovered] = accent;
  colors[ImGuiCol_SeparatorActive] = accent;
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.29f, 0.48f, 0.88f, 0.40f);
  colors[ImGuiCol_ResizeGripHovered] = accent;
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.36f, 0.55f, 0.94f, 1.00f);
  colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.13f, 0.21f, 1.00f);
  colors[ImGuiCol_TabHovered] = accent;
  colors[ImGuiCol_TabActive] = ImVec4(0.16f, 0.20f, 0.32f, 1.00f);
  colors[ImGuiCol_TextSelectedBg] = ImVec4(0.29f, 0.48f, 0.88f, 0.45f);
}

void ApplySlate(ImGuiStyle& style) {
  ImVec4* colors = style.Colors;
  const ImVec4 accent = ImVec4(0.35f, 0.55f, 0.95f, 1.00f);
  colors[ImGuiCol_Text] = ImVec4(0.94f, 0.94f, 0.95f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.56f, 0.59f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.16f, 0.96f);
  colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.33f, 1.00f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.19f, 0.19f, 0.22f, 1.00f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.29f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.35f, 1.00f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.18f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.32f, 0.32f, 0.36f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = accent;
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.42f, 0.62f, 1.00f, 1.00f);
  colors[ImGuiCol_CheckMark] = accent;
  colors[ImGuiCol_SliderGrab] = accent;
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.42f, 0.62f, 1.00f, 1.00f);
  colors[ImGuiCol_Button] = ImVec4(0.21f, 0.21f, 0.25f, 1.00f);
  colors[ImGuiCol_ButtonHovered] = accent;
  colors[ImGuiCol_ButtonActive] = ImVec4(0.27f, 0.45f, 0.82f, 1.00f);
  colors[ImGuiCol_Header] = ImVec4(0.21f, 0.21f, 0.25f, 1.00f);
  colors[ImGuiCol_HeaderHovered] = accent;
  colors[ImGuiCol_HeaderActive] = ImVec4(0.27f, 0.45f, 0.82f, 1.00f);
  colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.30f, 0.33f, 1.00f);
  colors[ImGuiCol_SeparatorHovered] = accent;
  colors[ImGuiCol_SeparatorActive] = accent;
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.55f, 0.95f, 0.40f);
  colors[ImGuiCol_ResizeGripHovered] = accent;
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.42f, 0.62f, 1.00f, 1.00f);
  colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.18f, 1.00f);
  colors[ImGuiCol_TabHovered] = accent;
  colors[ImGuiCol_TabActive] = ImVec4(0.21f, 0.21f, 0.25f, 1.00f);
  colors[ImGuiCol_TextSelectedBg] = ImVec4(0.35f, 0.55f, 0.95f, 0.45f);
}

}  // namespace

void Apply(int theme_index) {
  const bool valid = theme_index >= 0 && theme_index < kThemeCount;
  const int theme = valid ? theme_index : 0;
  ImGui::StyleColorsDark();
  ImGuiStyle& style = ImGui::GetStyle();
  ApplySpacing(style);
  if (theme == 1) {
    ApplySlate(style);
  } else {
    ApplyMidnight(style);
  }
}

}  // namespace theme
}  // namespace desktop_os
