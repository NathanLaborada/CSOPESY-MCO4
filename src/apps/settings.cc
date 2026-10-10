#include "apps/settings.h"

#include <cfloat>

#include "imgui.h"
#include "ui/desktop.h"
#include "ui/theme.h"
#include "ui/widgets.h"

namespace desktop_os {
namespace settings {
namespace {

// Window title must stay fixed so ImGui never creates a duplicate window.
constexpr const char* kWindowTitle = "Settings";

// Layout constants (Google style: k + PascalCase).
constexpr float kCategoryWidth = 180.0f;
constexpr float kCategoryHeight = 26.0f;
constexpr float kContentIndent = 4.0f;

// Placeholder window placement, used until the user moves/resizes it.
constexpr float kDefaultPosX = 96.0f;
constexpr float kDefaultPosY = 68.0f;
constexpr float kDefaultWidth = 720.0f;
constexpr float kDefaultHeight = 470.0f;
constexpr float kMinWidth = 480.0f;
constexpr float kMinHeight = 300.0f;

constexpr const char* kCategories[] = {"System", "Display",
                                       "Personalization", "About"};
constexpr int kCategoryCount = 4;

// Module-private UI state: which category is shown.
int g_category = 0;

// A muted "label: value" line of simulated system information.
void InfoRow(const char* label, const char* value) {
  ImGui::PushStyleColor(ImGuiCol_Text,
                        ImVec4(0.62f, 0.65f, 0.72f, 1.00f));
  ImGui::Text("%s:", label);
  ImGui::PopStyleColor();
  ImGui::SameLine();
  ImGui::TextUnformatted(value);
}

// Clearly-marked placeholder content; nothing here queries the OS.
void DrawSystemCategory() {
  widgets::MutedText("All values below are simulated dummy data.");
  ImGui::Spacing();
  InfoRow("Device name", "CSOPESY-DESKTOP");
  InfoRow("Processor", "QuantumCore i9 @ 3.60 GHz (simulated)");
  InfoRow("Installed RAM", "16.0 GB (simulated)");
  InfoRow("System type", "64-bit operating system, x64 processor");
  InfoRow("Graphics", "OpenGL 3.3 renderer (simulated)");
  InfoRow("OS version", "CSOPESY Desktop OS 1.0 (simulated)");
}

void DrawDisplayCategory() {
  widgets::MutedText("Placeholder display controls (simulated).");
  ImGui::Spacing();
  // Disabled placeholders: locals are fine because nothing persists.
  int resolution = 0;
  float scale = 1.0f;
  ImGui::BeginDisabled(true);
  ImGui::Combo("Resolution", &resolution, "1280 x 720 (simulated)", -1);
  ImGui::SliderFloat("Scale", &scale, 0.5f, 2.0f, "%.2fx");
  ImGui::EndDisabled();
  ImGui::Spacing();
  ImGui::TextUnformatted("The real desktop is 1:1 pixels; see the handoff");
  ImGui::TextUnformatted("notes on high-DPI for details.");
}

// Wallpaper and theme pickers write straight into the shared AppState.
// The wallpaper change is visible immediately (desktop reads it every
// frame); the theme change becomes visible once theme::Apply is wired.
void DrawPersonalizationCategory(AppState& state) {
  if (state.wallpaper_index < 0 ||
      state.wallpaper_index >= desktop::kWallpaperCount) {
    state.wallpaper_index = 0;
  }
  if (state.theme_index < 0 || state.theme_index >= theme::kThemeCount) {
    state.theme_index = 0;
  }

  ImGui::TextUnformatted("Desktop background");
  ImGui::Combo("Wallpaper", &state.wallpaper_index, desktop::kWallpaperNames,
               desktop::kWallpaperCount);
  ImGui::Spacing();
  ImGui::TextUnformatted("Color theme");
  ImGui::Combo("Theme", &state.theme_index, theme::kThemeNames,
               theme::kThemeCount);
  ImGui::Spacing();
  widgets::MutedText("Both options are demo controls (simulated).");
}

void DrawAboutCategory() {
  ImGui::TextUnformatted("CSOPESY Desktop OS Mockup");
  ImGui::Spacing();
  InfoRow("Course project", "CSOPESY MCO4 — desktop-style GUI");
  InfoRow("Built with", "C++17, OpenGL 3.3, GLFW, Dear ImGui");
  InfoRow("Nature", "Graphical simulation only, not a real OS");
  InfoRow("Data", "All process and system data is dummy data");
  ImGui::Spacing();
  widgets::MutedText("Nathan · Lorens · Kim · Byron");
}

void DrawContent(AppState& state) {
  ImGui::TextUnformatted(kCategories[g_category]);
  ImGui::Separator();
  ImGui::Spacing();
  switch (g_category) {
    case 0:
      DrawSystemCategory();
      break;
    case 1:
      DrawDisplayCategory();
      break;
    case 2:
      DrawPersonalizationCategory(state);
      break;
    default:
      DrawAboutCategory();
      break;
  }
}

// Left category list (System, Display, Personalization, About).
void DrawCategoryList() {
  ImGui::BeginChild("##categories", ImVec2(kCategoryWidth, 0.0f), true);
  for (int i = 0; i < kCategoryCount; ++i) {
    if (i > 0) ImGui::Spacing();
    if (ImGui::Selectable(kCategories[i], g_category == i, 0,
                          ImVec2(-1.0f, kCategoryHeight))) {
      g_category = i;
    }
  }
  ImGui::EndChild();
}

// Snap the window so its bottom edge never enters the taskbar strip
// (same approach as File Explorer; public API only, user moves stick).
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
  if (!state.show_settings) return;

  ImGui::SetNextWindowPos(ImVec2(kDefaultPosX, kDefaultPosY),
                          ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(kDefaultWidth, kDefaultHeight),
                           ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSizeConstraints(
      ImVec2(kMinWidth, kMinHeight), ImVec2(FLT_MAX, FLT_MAX));

  const bool visible = ImGui::Begin(kWindowTitle, &state.show_settings);
  ClampAboveTaskbar();
  if (!visible) {
    ImGui::End();
    return;
  }

  DrawCategoryList();
  ImGui::SameLine();
  ImGui::BeginChild("##content", ImVec2(0.0f, 0.0f), false);
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + kContentIndent);
  DrawContent(state);
  ImGui::EndChild();

  ImGui::End();
}

}  // namespace settings
}  // namespace desktop_os
