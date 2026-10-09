#include "ui/Desktop.h"

#include <imgui.h>

#include <algorithm>
#include <cmath>

namespace Desktop {

const char* const kWallpaperNames[kWallpaperCount] = {"Midnight", "Aurora", "Slate"};

namespace {

void VerticalGradient(ImDrawList* dl, ImVec2 a, ImVec2 b, ImU32 top, ImU32 bottom)
{
    dl->AddRectFilledMultiColor(a, b, top, top, bottom, bottom);
}

// Soft radial glow: a triangle fan whose centre vertex is `inner` and whose rim
// is fully transparent; the GPU interpolates a smooth gradient between them.
void Glow(ImDrawList* dl, ImVec2 center, float radius, ImU32 inner)
{
    constexpr int kSegments = 64;
    const ImU32 outer = inner & ~IM_COL32_A_MASK;
    const ImVec2 uv = ImGui::GetFontTexUvWhitePixel();

    dl->PrimReserve(kSegments * 3, kSegments + 1);
    const unsigned int base = dl->_VtxCurrentIdx;
    dl->PrimWriteVtx(center, uv, inner);
    for (int i = 0; i < kSegments; ++i) {
        const float angle = 6.2831853f * float(i) / kSegments;
        dl->PrimWriteVtx(ImVec2(center.x + std::cos(angle) * radius, center.y + std::sin(angle) * radius),
                         uv, outer);
    }
    for (int i = 0; i < kSegments; ++i) {
        dl->PrimWriteIdx(static_cast<ImDrawIdx>(base));
        dl->PrimWriteIdx(static_cast<ImDrawIdx>(base + 1 + i));
        dl->PrimWriteIdx(static_cast<ImDrawIdx>(base + 1 + (i + 1) % kSegments));
    }
}

void DotGrid(ImDrawList* dl, ImVec2 a, ImVec2 b, float spacing, ImU32 col)
{
    for (float y = a.y + spacing * 0.5f; y < b.y; y += spacing)
        for (float x = a.x + spacing * 0.5f; x < b.x; x += spacing)
            dl->AddRectFilled(ImVec2(x, y), ImVec2(x + 1.5f, y + 1.5f), col);
}

void Waves(ImDrawList* dl, ImVec2 a, ImVec2 b, float startY, int count, ImU32 col)
{
    constexpr int kPoints = 97;
    const float w = b.x - a.x;
    const float h = b.y - a.y;
    ImVec2 pts[kPoints];
    for (int line = 0; line < count; ++line) {
        const float baseY = a.y + h * (startY + 0.035f * line);
        const float amp = h * (0.05f + 0.008f * line);
        const float phase = 0.45f * line;
        for (int i = 0; i < kPoints; ++i) {
            const float t = float(i) / (kPoints - 1);
            pts[i] = ImVec2(a.x + w * t, baseY + amp * std::sin(t * 7.5f + phase));
        }
        dl->AddPolyline(pts, kPoints, col, ImDrawFlags_None, 1.5f);
    }
}

void Stripes(ImDrawList* dl, ImVec2 a, ImVec2 b, float spacing, ImU32 col)
{
    const float h = b.y - a.y;
    for (float x = a.x - h; x < b.x; x += spacing)
        dl->AddLine(ImVec2(x, b.y), ImVec2(x + h, a.y), col, 1.0f);
}

} // namespace

void Render(const AppState& state)
{
    const ImGuiViewport* vp = ImGui::GetMainViewport();
    const ImVec2 a = vp->Pos;
    const ImVec2 b(vp->Pos.x + vp->Size.x, vp->Pos.y + vp->Size.y);
    const float w = vp->Size.x;
    const float h = vp->Size.y;
    const float r = std::max(w, h);
    ImDrawList* dl = ImGui::GetBackgroundDrawList();

    const int wallpaper =
        (state.wallpaperIndex >= 0 && state.wallpaperIndex < kWallpaperCount) ? state.wallpaperIndex : 0;

    switch (wallpaper) {
    case 0: // Midnight: deep navy, two soft glows, dot grid
        VerticalGradient(dl, a, b, IM_COL32(10, 14, 30, 255), IM_COL32(22, 30, 58, 255));
        Glow(dl, ImVec2(a.x + w * 0.78f, a.y + h * 0.25f), r * 0.45f, IM_COL32(70, 110, 220, 90));
        Glow(dl, ImVec2(a.x + w * 0.15f, a.y + h * 0.90f), r * 0.35f, IM_COL32(120, 80, 200, 64));
        DotGrid(dl, a, b, 28.0f, IM_COL32(255, 255, 255, 24));
        break;
    case 1: // Aurora: dark teal, flowing light lines
        VerticalGradient(dl, a, b, IM_COL32(8, 16, 30, 255), IM_COL32(10, 38, 50, 255));
        Glow(dl, ImVec2(a.x + w * 0.30f, a.y + h * 0.45f), r * 0.40f, IM_COL32(40, 200, 170, 56));
        Waves(dl, a, b, 0.30f, 7, IM_COL32(80, 220, 190, 38));
        Waves(dl, a, b, 0.52f, 5, IM_COL32(150, 120, 255, 30));
        break;
    default: // Slate: charcoal with pinstripes
        VerticalGradient(dl, a, b, IM_COL32(30, 32, 38, 255), IM_COL32(16, 17, 21, 255));
        Glow(dl, ImVec2(a.x + w * 0.10f, a.y + h * 0.10f), r * 0.40f, IM_COL32(90, 140, 255, 40));
        Stripes(dl, a, b, 22.0f, IM_COL32(255, 255, 255, 8));
        break;
    }

    // Small watermark above the taskbar, bottom-right.
    const char* mark = "CSOPESY Desktop OS  |  simulated environment";
    const ImVec2 size = ImGui::CalcTextSize(mark);
    dl->AddText(ImVec2(b.x - size.x - 16.0f, b.y - kTaskbarHeight - size.y - 12.0f),
                IM_COL32(255, 255, 255, 70), mark);
}

} // namespace Desktop
