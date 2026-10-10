#include "ui/widgets.h"

#include <cmath>

#include "imgui.h"

namespace desktop_os {
namespace widgets {
namespace {

constexpr float kTwoPi = 6.2831853f;
constexpr float kIconPadding = 8.0f;

ImU32 IconColor(bool hovered, bool held, bool active) {
  if (held) return IM_COL32(255, 255, 255, 255);
  if (hovered || active) return IM_COL32(255, 255, 255, 255);
  return IM_COL32(200, 206, 220, 255);
}

void DrawFolder(ImDrawList* draw_list, ImVec2 center, float radius,
                ImU32 color) {
  const float half = radius * 0.85f;
  const ImVec2 body_min(center.x - half, center.y - half * 0.45f);
  const ImVec2 body_max(center.x + half, center.y + half * 0.75f);
  const ImVec2 tab_max(center.x - half * 0.05f, body_min.y);
  const ImVec2 tab_min(center.x - half, tab_max.y - radius * 0.35f);
  draw_list->AddRectFilled(tab_min, tab_max, color, radius * 0.15f);
  draw_list->AddRectFilled(body_min, body_max, color, radius * 0.15f);
  const ImVec2 inner_min(body_min.x + 2.0f, body_min.y + 2.0f);
  const ImVec2 inner_max(body_max.x - 2.0f, body_max.y - 2.0f);
  draw_list->AddRect(inner_min, inner_max, IM_COL32(20, 26, 44, 200),
                     radius * 0.1f, 0, 1.5f);
}

void DrawSettings(ImDrawList* draw_list, ImVec2 center, float radius,
                  ImU32 color) {
  constexpr int kTeeth = 8;
  for (int i = 0; i < kTeeth; ++i) {
    const float angle = kTwoPi * static_cast<float>(i) / kTeeth;
    const ImVec2 inner(center.x + std::cos(angle) * radius * 0.62f,
                       center.y + std::sin(angle) * radius * 0.62f);
    const ImVec2 outer(center.x + std::cos(angle) * radius * 0.95f,
                       center.y + std::sin(angle) * radius * 0.95f);
    draw_list->AddLine(inner, outer, color, 2.5f);
  }
  draw_list->AddCircle(center, radius * 0.55f, color, 24, 2.5f);
  draw_list->AddCircleFilled(center, radius * 0.18f, color);
}

void DrawTaskManager(ImDrawList* draw_list, ImVec2 center, float radius,
                     ImU32 color) {
  const float half = radius * 0.85f;
  draw_list->AddLine(ImVec2(center.x - half, center.y + half),
                     ImVec2(center.x + half, center.y + half), color, 2.0f);
  draw_list->AddLine(ImVec2(center.x - half, center.y + half),
                     ImVec2(center.x - half, center.y - half), color, 2.0f);
  const float widths[3] = {0.28f, 0.52f, 0.78f};
  for (int i = 0; i < 3; ++i) {
    const float x = center.x - half + half * 2.0f * (0.25f + 0.25f * i);
    const float top = center.y + half - half * 2.0f * widths[i];
    draw_list->AddRectFilled(ImVec2(x - radius * 0.16f, top),
                             ImVec2(x + radius * 0.16f, center.y + half - 2.0f),
                             color, 1.0f);
  }
}

void DrawPower(ImDrawList* draw_list, ImVec2 center, float radius,
               ImU32 color) {
  // Open ring with a gap centered at the top. ImGui angles start at +x and
  // grow clockwise (y points down), so the top is at -pi/2; the arc runs
  // from one side of the gap around to the other, spanning under 360
  // degrees so it never overlaps itself.
  constexpr float kHalfPi = 1.5707963f;
  constexpr float kGapHalf = 0.45f;
  const float start = -kHalfPi + kGapHalf;
  const float end = -kHalfPi - kGapHalf + kTwoPi;
  draw_list->PathArcTo(center, radius * 0.75f, start, end, 28);
  draw_list->PathStroke(color, 0, 2.5f);
  draw_list->AddLine(ImVec2(center.x, center.y - radius * 0.95f),
                     ImVec2(center.x, center.y - radius * 0.05f), color, 2.5f);
}

void DrawIconShape(ImDrawList* draw_list, IconKind kind, ImVec2 center,
                   float radius, ImU32 color) {
  switch (kind) {
    case IconKind::kFolder:
      DrawFolder(draw_list, center, radius, color);
      break;
    case IconKind::kSettings:
      DrawSettings(draw_list, center, radius, color);
      break;
    case IconKind::kTaskManager:
      DrawTaskManager(draw_list, center, radius, color);
      break;
    case IconKind::kPower:
      DrawPower(draw_list, center, radius, color);
      break;
  }
}

}  // namespace

bool IconButton(const char* id, IconKind kind, ImVec2 size, bool active) {
  ImGui::PushID(id);
  const bool clicked = ImGui::InvisibleButton("##icon", size);
  const bool hovered = ImGui::IsItemHovered();
  const bool held = ImGui::IsItemActive();
  const ImVec2 rect_min = ImGui::GetItemRectMin();
  const ImVec2 rect_max = ImGui::GetItemRectMax();
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  ImU32 background = IM_COL32(0, 0, 0, 0);
  if (held) {
    background = kind == IconKind::kPower
                     ? IM_COL32(150, 40, 45, 255)
                     : IM_COL32(60, 100, 190, 255);
  } else if (hovered) {
    background = kind == IconKind::kPower
                     ? IM_COL32(190, 60, 65, 255)
                     : IM_COL32(74, 123, 224, 255);
  } else if (active) {
    background = IM_COL32(52, 78, 150, 255);
  } else {
    background = IM_COL32(30, 36, 58, 255);
  }
  const float rounding = ImGui::GetStyle().FrameRounding;
  draw_list->AddRectFilled(rect_min, rect_max, background, rounding);
  if (active && !hovered && !held) {
    draw_list->AddRect(rect_min, rect_max, IM_COL32(120, 160, 255, 255),
                       rounding, 0, 1.5f);
  }

  const ImVec2 center((rect_min.x + rect_max.x) * 0.5f,
                      (rect_min.y + rect_max.y) * 0.5f);
  const float radius =
      (size.x < size.y ? size.x : size.y) * 0.5f - kIconPadding * 0.5f;
  DrawIconShape(draw_list, kind, center,
                radius > 4.0f ? radius : 4.0f,
                IconColor(hovered, held, active));
  ImGui::PopID();
  return clicked;
}

void Icon(IconKind kind, float size, ImU32 color) {
  const ImVec2 cursor = ImGui::GetCursorScreenPos();
  const ImVec2 center(cursor.x + size * 0.5f, cursor.y + size * 0.5f);
  ImGui::Dummy(ImVec2(size, size));
  DrawIconShape(ImGui::GetWindowDrawList(), kind, center, size * 0.5f - 1.0f,
                color);
}

void MutedText(const char* text) {
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.62f, 0.65f, 0.72f, 1.00f));
  ImGui::TextUnformatted(text);
  ImGui::PopStyleColor();
}

}  // namespace widgets
}  // namespace desktop_os
