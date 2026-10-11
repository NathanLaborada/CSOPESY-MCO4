#include "ui/desktop.h"

#include <algorithm>
#include <cmath>

#include "imgui.h"

namespace desktop_os {
namespace desktop {
namespace {

constexpr float kTwoPi = 6.2831853f;

void DrawVerticalGradient(ImDrawList* draw_list, ImVec2 top_left,
                          ImVec2 bottom_right, ImU32 top_color,
                          ImU32 bottom_color) {
  draw_list->AddRectFilledMultiColor(top_left, bottom_right, top_color,
                                     top_color, bottom_color, bottom_color);
}

// Draw a soft glow with a solid center and transparent edge.
void DrawGlow(ImDrawList* draw_list, ImVec2 center, float radius,
              ImU32 inner_color) {
  constexpr int kSegments = 64;
  const ImU32 outer_color = inner_color & ~IM_COL32_A_MASK;
  const ImVec2 white_pixel_uv = ImGui::GetFontTexUvWhitePixel();

  draw_list->PrimReserve(kSegments * 3, kSegments + 1);
  const unsigned int center_index = draw_list->_VtxCurrentIdx;
  draw_list->PrimWriteVtx(center, white_pixel_uv, inner_color);
  for (int i = 0; i < kSegments; ++i) {
    const float angle = kTwoPi * static_cast<float>(i) / kSegments;
    const ImVec2 rim_point(center.x + std::cos(angle) * radius,
                           center.y + std::sin(angle) * radius);
    draw_list->PrimWriteVtx(rim_point, white_pixel_uv, outer_color);
  }
  for (int i = 0; i < kSegments; ++i) {
    draw_list->PrimWriteIdx(static_cast<ImDrawIdx>(center_index));
    draw_list->PrimWriteIdx(static_cast<ImDrawIdx>(center_index + 1 + i));
    draw_list->PrimWriteIdx(
        static_cast<ImDrawIdx>(center_index + 1 + (i + 1) % kSegments));
  }
}

void DrawDotGrid(ImDrawList* draw_list, ImVec2 top_left, ImVec2 bottom_right,
                 float spacing, ImU32 color) {
  constexpr float kDotSize = 1.5f;
  for (float y = top_left.y + spacing * 0.5f; y < bottom_right.y;
       y += spacing) {
    for (float x = top_left.x + spacing * 0.5f; x < bottom_right.x;
         x += spacing) {
      draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + kDotSize, y + kDotSize),
                               color);
    }
  }
}

// Draw sine-wave lines at increasing vertical positions.
void DrawWaves(ImDrawList* draw_list, ImVec2 top_left, ImVec2 bottom_right,
               float start_fraction, int line_count, ImU32 color) {
  constexpr int kPointCount = 97;
  const float width = bottom_right.x - top_left.x;
  const float height = bottom_right.y - top_left.y;
  ImVec2 points[kPointCount];
  for (int line = 0; line < line_count; ++line) {
    const float base_y = top_left.y + height * (start_fraction + 0.035f * line);
    const float amplitude = height * (0.05f + 0.008f * line);
    const float phase = 0.45f * line;
    for (int i = 0; i < kPointCount; ++i) {
      const float t = static_cast<float>(i) / (kPointCount - 1);
      points[i] = ImVec2(top_left.x + width * t,
                         base_y + amplitude * std::sin(t * 7.5f + phase));
    }
    draw_list->AddPolyline(points, kPointCount, color, ImDrawFlags_None, 1.5f);
  }
}

void DrawStripes(ImDrawList* draw_list, ImVec2 top_left, ImVec2 bottom_right,
                 float spacing, ImU32 color) {
  const float height = bottom_right.y - top_left.y;
  for (float x = top_left.x - height; x < bottom_right.x; x += spacing) {
    draw_list->AddLine(ImVec2(x, bottom_right.y),
                       ImVec2(x + height, top_left.y), color, 1.0f);
  }
}

}  // namespace

void Render(const AppState& state) {
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  const ImVec2 top_left = viewport->Pos;
  const ImVec2 bottom_right(viewport->Pos.x + viewport->Size.x,
                            viewport->Pos.y + viewport->Size.y);
  const float width = viewport->Size.x;
  const float height = viewport->Size.y;
  const float glow_scale = std::max(width, height);
  ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

  const bool valid_index =
      state.wallpaper_index >= 0 && state.wallpaper_index < kWallpaperCount;
  const int wallpaper = valid_index ? state.wallpaper_index : 0;

  switch (wallpaper) {
    case 0:  // Midnight: deep navy, two soft glows, dot grid.
      DrawVerticalGradient(draw_list, top_left, bottom_right,
                           IM_COL32(10, 14, 30, 255),
                           IM_COL32(22, 30, 58, 255));
      DrawGlow(draw_list,
               ImVec2(top_left.x + width * 0.78f, top_left.y + height * 0.25f),
               glow_scale * 0.45f, IM_COL32(70, 110, 220, 90));
      DrawGlow(draw_list,
               ImVec2(top_left.x + width * 0.15f, top_left.y + height * 0.90f),
               glow_scale * 0.35f, IM_COL32(120, 80, 200, 64));
      DrawDotGrid(draw_list, top_left, bottom_right, 28.0f,
                  IM_COL32(255, 255, 255, 24));
      break;
    case 1:  // Aurora: dark teal, flowing light lines.
      DrawVerticalGradient(draw_list, top_left, bottom_right,
                           IM_COL32(8, 16, 30, 255), IM_COL32(10, 38, 50, 255));
      DrawGlow(draw_list,
               ImVec2(top_left.x + width * 0.30f, top_left.y + height * 0.45f),
               glow_scale * 0.40f, IM_COL32(40, 200, 170, 56));
      DrawWaves(draw_list, top_left, bottom_right, 0.30f, 7,
                IM_COL32(80, 220, 190, 38));
      DrawWaves(draw_list, top_left, bottom_right, 0.52f, 5,
                IM_COL32(150, 120, 255, 30));
      break;
    default:  // Slate: charcoal with pinstripes.
      DrawVerticalGradient(draw_list, top_left, bottom_right,
                           IM_COL32(30, 32, 38, 255),
                           IM_COL32(16, 17, 21, 255));
      DrawGlow(draw_list,
               ImVec2(top_left.x + width * 0.10f, top_left.y + height * 0.10f),
               glow_scale * 0.40f, IM_COL32(90, 140, 255, 40));
      DrawStripes(draw_list, top_left, bottom_right, 22.0f,
                  IM_COL32(255, 255, 255, 8));
      break;
  }

  // Draw a small label above the taskbar.
  const char* watermark = "CSOPESY Desktop OS  |  simulated environment";
  const ImVec2 text_size = ImGui::CalcTextSize(watermark);
  draw_list->AddText(
      ImVec2(bottom_right.x - text_size.x - 16.0f,
             bottom_right.y - kTaskbarHeight - text_size.y - 12.0f),
      IM_COL32(255, 255, 255, 70), watermark);
}

}  // namespace desktop
}  // namespace desktop_os
