#include "apps/file_explorer.h"

#include <algorithm>
#include <cfloat>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "imgui.h"
#include "ui/widgets.h"

namespace desktop_os {
namespace file_explorer {
namespace {

// Window title must stay fixed so ImGui never creates a duplicate window.
constexpr const char* kWindowTitle = "File Explorer";

// Layout constants (Google style: k + PascalCase).
constexpr float kSidebarWidth = 175.0f;
constexpr float kPathBarHeight = 36.0f;
constexpr float kRowHeight = 24.0f;
constexpr float kIconSize = 16.0f;
constexpr float kIconGap = 8.0f;
constexpr float kIconPadY = 4.0f;

// Placeholder window placement, used until the user moves/resizes it.
constexpr float kDefaultPosX = 70.0f;
constexpr float kDefaultPosY = 56.0f;
constexpr float kDefaultWidth = 780.0f;
constexpr float kDefaultHeight = 500.0f;
constexpr float kMinWidth = 460.0f;
constexpr float kMinHeight = 260.0f;

// Icon colors: accent-blue folders, muted grey files (no emoji, no fonts).
constexpr ImU32 kFolderColor = IM_COL32(122, 156, 236, 255);
constexpr ImU32 kFileColor = IM_COL32(170, 178, 196, 255);
constexpr ImU32 kChevronColor = IM_COL32(158, 165, 183, 255);

// One node of the simulated filesystem. Folders own their children; files
// only carry display strings. All data is static placeholder content.
struct Entry {
  std::string name;
  bool is_folder = false;
  std::string modified;
  std::string type;
  std::string size;
  std::vector<Entry> children;
};

Entry Folder(std::string name, std::vector<Entry> children,
             std::string modified = "10/09/2026 09:41 AM") {
  Entry entry;
  entry.name = std::move(name);
  entry.is_folder = true;
  entry.type = "File folder";
  entry.modified = std::move(modified);
  entry.children = std::move(children);
  return entry;
}

Entry File(std::string name, std::string type, std::string size,
           std::string modified = "10/08/2026 02:15 PM") {
  Entry entry;
  entry.name = std::move(name);
  entry.is_folder = false;
  entry.type = std::move(type);
  entry.size = std::move(size);
  entry.modified = std::move(modified);
  return entry;
}

// The simulated tree: "This PC" is the root, so the default breadcrumb is
// one segment and every listing below it is reachable from the sidebar.
Entry BuildTree() {
  return Folder(
      "This PC",
      {Folder(
          "Local Disk (C:)",
          {Folder("Program Files",
                  {Folder("Common Files",
                          {File("license.rtf", "Rich Text Document", "18 KB")}),
                   File("opengl32.dll", "Application Extension", "1.2 MB")},
                  "10/07/2026 11:02 AM"),
           Folder(
               "Users",
               {Folder("kim",
                       {Folder("Desktop",
                               {File("report.txt", "Text Document", "12 KB",
                                     "10/10/2026 09:41 AM"),
                                File("wallpaper.png", "PNG File", "2.4 MB",
                                     "10/09/2026 06:30 PM"),
                                Folder("Projects",
                                       {Folder("CSOPESY-MCO4",
                                               {File("README.md",
                                                     "Markdown File", "1 KB"),
                                                File("notes.txt",
                                                     "Text Document", "3 KB")},
                                               "10/10/2026 08:15 AM")},
                                       "10/10/2026 08:15 AM")},
                               "10/10/2026 09:41 AM"),
                        Folder("Documents",
                               {File("timetable.xlsx",
                                     "Microsoft Excel Worksheet", "34 KB"),
                                File("agenda.txt", "Text Document", "2 KB",
                                     "10/06/2026 04:20 PM"),
                                Folder("School", {})},
                               "10/08/2026 10:05 AM"),
                        Folder("Downloads",
                               {File("imgui_v1.91.9b.zip",
                                     "Compressed (zipped) Folder", "4.8 MB"),
                                File("setup.exe", "Application", "64 MB",
                                     "10/05/2026 07:55 PM")},
                               "10/09/2026 07:55 PM")},
                       "10/10/2026 09:41 AM")},
               "10/10/2026 09:41 AM"),
           Folder(
               "Windows",
               {File("notepad.exe", "Application", "194 KB"),
                Folder("System32", {File("kernel32.dll",
                                         "Application Extension", "741 KB")})},
               "10/04/2026 01:12 PM")},
          "10/10/2026 09:41 AM")},
      "10/10/2026 09:41 AM");
}

const Entry& Root() {
  static const Entry root = BuildTree();
  return root;
}

// Module-private UI state: current folder path and selected entry name.
std::vector<std::string> g_path{"This PC"};
std::string g_selected;

const Entry* FindPath(const std::vector<std::string>& path) {
  // path[0] must be the root's own name ("This PC"); segments after that
  // walk down through children.
  if (path.empty() || path.front() != Root().name) return nullptr;
  const Entry* node = &Root();
  for (size_t i = 1; i < path.size(); ++i) {
    const std::string& segment = path[i];
    const auto it =
        std::find_if(node->children.begin(), node->children.end(),
                     [&segment](const Entry& e) { return e.name == segment; });
    if (it == node->children.end()) return nullptr;
    node = &*it;
  }
  return node;
}

void NavigateTo(const std::vector<std::string>& path) {
  g_path = path;
  g_selected.clear();
}

// Paper-document icon with a folded corner, drawn with ImDrawList shapes.
void DrawFileIcon(ImDrawList* draw_list, ImVec2 top_left, float size) {
  const float fold = size * 0.30f;
  draw_list->PathLineTo(top_left);
  draw_list->PathLineTo(ImVec2(top_left.x + size - fold, top_left.y));
  draw_list->PathLineTo(ImVec2(top_left.x + size, top_left.y + fold));
  draw_list->PathLineTo(ImVec2(top_left.x + size, top_left.y + size));
  draw_list->PathLineTo(ImVec2(top_left.x, top_left.y + size));
  draw_list->PathFillConvex(kFileColor);
  const ImU32 edge = IM_COL32(40, 46, 66, 255);
  draw_list->AddLine(ImVec2(top_left.x + size - fold, top_left.y),
                     ImVec2(top_left.x + size - fold, top_left.y + fold), edge,
                     1.0f);
  draw_list->AddLine(ImVec2(top_left.x + size - fold, top_left.y + fold),
                     ImVec2(top_left.x + size, top_left.y + fold), edge, 1.0f);
}

void DrawRowIcon(ImDrawList* draw_list, const Entry& entry, ImVec2 top_left) {
  if (entry.is_folder) {
    // Folder silhouette: tab over a rounded body (same idea as widgets).
    const float s = kIconSize;
    const ImVec2 tab_min(top_left.x, top_left.y + s * 0.15f);
    const ImVec2 tab_max(top_left.x + s * 0.55f, top_left.y + s * 0.35f);
    const ImVec2 body_min(top_left.x, top_left.y + s * 0.28f);
    const ImVec2 body_max(top_left.x + s, top_left.y + s);
    draw_list->AddRectFilled(tab_min, tab_max, kFolderColor, 2.0f);
    draw_list->AddRectFilled(body_min, body_max, kFolderColor, 2.5f);
  } else {
    DrawFileIcon(draw_list, top_left, kIconSize);
  }
}

// Path of `name` below `base`, for sidebar jumps.
std::vector<std::string> ChildPath(const std::vector<std::string>& base,
                                   const char* name) {
  std::vector<std::string> path = base;
  path.emplace_back(name);
  return path;
}

// A sidebar row: folder icon + full-width clickable label.
bool SidebarItem(const char* label, const std::vector<std::string>& target) {
  const ImVec2 pos = ImGui::GetCursorScreenPos();
  ImGui::SetCursorScreenPos(ImVec2(pos.x + 4.0f, pos.y + 3.0f));
  widgets::Icon(widgets::IconKind::kFolder, kIconSize, kFolderColor);
  ImGui::SetCursorScreenPos(ImVec2(pos.x + 26.0f, pos.y));
  const bool active = g_path == target;
  const bool clicked =
      ImGui::Selectable(label, active, 0, ImVec2(0.0f, kRowHeight));
  if (clicked) NavigateTo(target);
  return clicked;
}

// Breadcrumb: "This PC > Local Disk (C:) > ..." where each segment jumps
// to that prefix of the current path.
void DrawPathBar() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6.0f, 4.0f));
  ImGui::BeginChild("##pathbar", ImVec2(0.0f, kPathBarHeight), true);
  for (size_t i = 0; i < g_path.size(); ++i) {
    if (i > 0) {
      ImGui::SameLine();
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.62f, 0.65f, 0.72f, 1.00f));
      ImGui::TextUnformatted(">");
      ImGui::PopStyleColor();
      ImGui::SameLine();
    }
    if (ImGui::SmallButton(g_path[i].c_str())) {
      NavigateTo(std::vector<std::string>(
          g_path.begin(), g_path.begin() + static_cast<std::ptrdiff_t>(i) + 1));
    }
  }
  ImGui::EndChild();
  ImGui::PopStyleVar();
}

// Left navigation pane: Quick access jumps plus the This PC drive.
void DrawSidebar() {
  ImGui::BeginChild("##sidebar", ImVec2(kSidebarWidth, 0.0f), true);
  widgets::MutedText("Quick access");
  const std::vector<std::string> base{"This PC", "Local Disk (C:)", "Users",
                                      "kim"};
  SidebarItem("Desktop", ChildPath(base, "Desktop"));
  SidebarItem("Downloads", ChildPath(base, "Downloads"));
  SidebarItem("Documents", ChildPath(base, "Documents"));
  ImGui::Spacing();
  widgets::MutedText("This PC");
  SidebarItem("Local Disk (C:)", {"This PC", "Local Disk (C:)"});
  ImGui::EndChild();
}

// Main pane: header row plus the file/folder table. Click selects;
// double-clicking a folder navigates into it.
void DrawListing(const Entry& node) {
  if (ImGui::BeginTable("##files", 4,
                        ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_RowBg |
                            ImGuiTableFlags_Resizable |
                            ImGuiTableFlags_ScrollY |
                            ImGuiTableFlags_SizingStretchProp,
                        ImVec2(0.0f, ImGui::GetContentRegionAvail().y))) {
    ImGui::TableSetupScrollFreeze(0, 1);
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Date modified", ImGuiTableColumnFlags_WidthFixed,
                            130.0f);
    ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 150.0f);
    ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 70.0f);
    ImGui::TableHeadersRow();

    if (node.children.empty()) {
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.0f);
      widgets::MutedText("This folder is empty.");
    }

    for (const Entry& entry : node.children) {
      ImGui::TableNextRow(ImGuiTableRowFlags_None, kRowHeight);
      ImGui::TableSetColumnIndex(0);
      const ImVec2 cell = ImGui::GetCursorScreenPos();
      const bool selected = g_selected == entry.name;

      // Invisible full-row target first; icon and label are drawn on top.
      ImGui::SetCursorScreenPos(cell);
      const bool clicked =
          ImGui::Selectable(("##sel_" + entry.name).c_str(), selected,
                            ImGuiSelectableFlags_SpanAllColumns |
                                ImGuiSelectableFlags_AllowDoubleClick,
                            ImVec2(0.0f, kRowHeight));

      ImGui::SetCursorScreenPos(ImVec2(cell.x + 3.0f, cell.y + kIconPadY));
      DrawRowIcon(ImGui::GetWindowDrawList(), entry,
                  ImGui::GetCursorScreenPos());

      ImGui::SetCursorScreenPos(
          ImVec2(cell.x + kIconSize + kIconGap, cell.y + 4.0f));
      ImGui::TextUnformatted(entry.name.c_str());

      if (clicked) {
        g_selected = entry.name;
        if (entry.is_folder && ImGui::IsMouseDoubleClicked(0)) {
          std::vector<std::string> next = g_path;
          next.push_back(entry.name);
          NavigateTo(next);
        }
      }

      ImGui::TableSetColumnIndex(1);
      ImGui::TextUnformatted(entry.modified.c_str());
      ImGui::TableSetColumnIndex(2);
      ImGui::TextUnformatted(entry.type.c_str());
      ImGui::TableSetColumnIndex(3);
      if (!entry.size.empty()) ImGui::TextUnformatted(entry.size.c_str());
    }
    ImGui::EndTable();
  }
}

// Snap the window so its bottom edge never enters the taskbar strip.
// Uses public per-window setters right after Begin, so user drags and
// resizes still stick (they are re-clamped from live values each frame).
void ClampAboveTaskbar() {
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  const float limit_y = viewport->Pos.y + viewport->Size.y - kTaskbarHeight;

  ImVec2 pos = ImGui::GetWindowPos();
  ImVec2 size = ImGui::GetWindowSize();
  if (pos.y + size.y <= limit_y) return;

  pos.y = limit_y - size.y;
  if (pos.y < viewport->Pos.y) {
    // Window is taller than the space above the taskbar: shrink it.
    pos.y = viewport->Pos.y;
    size.y = limit_y - pos.y;
    ImGui::SetWindowSize(size);
  }
  ImGui::SetWindowPos(pos);
}

}  // namespace

void Render(AppState& state) {
  if (!state.show_file_explorer) return;

  ImGui::SetNextWindowPos(ImVec2(kDefaultPosX, kDefaultPosY),
                          ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(kDefaultWidth, kDefaultHeight),
                           ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSizeConstraints(ImVec2(kMinWidth, kMinHeight),
                                      ImVec2(FLT_MAX, FLT_MAX));

  const bool visible = ImGui::Begin(kWindowTitle, &state.show_file_explorer);
  ClampAboveTaskbar();
  if (!visible) {
    ImGui::End();
    return;
  }

  if (FindPath(g_path) == nullptr) NavigateTo({"This PC"});
  const Entry* node = FindPath(g_path);

  DrawPathBar();
  ImGui::Spacing();

  DrawSidebar();
  ImGui::SameLine();
  ImGui::BeginChild("##listing", ImVec2(0.0f, 0.0f), false);
  DrawListing(*node);
  ImGui::EndChild();

  ImGui::End();
}

}  // namespace file_explorer
}  // namespace desktop_os
