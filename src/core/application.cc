#include "core/application.h"

#include <cstdio>

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "apps/file_explorer.h"
#include "apps/settings.h"
#include "apps/task_manager.h"
#include "ui/desktop.h"
#include "ui/system_controls.h"
#include "ui/taskbar.h"
#include "ui/theme.h"

namespace desktop_os {
namespace {

// OpenGL 3.3 core is supported by every Windows GPU driver from the last
// decade and matches GLSL "#version 330" for the ImGui OpenGL3 backend.
constexpr int kGlMajorVersion = 3;
constexpr int kGlMinorVersion = 3;
constexpr const char* kGlslVersion = "#version 330";

constexpr int kInitialWidth = 1280;
constexpr int kInitialHeight = 720;
constexpr int kMinWidth = 800;
constexpr int kMinHeight = 500;

// Redraw interval when nothing is happening (clock, simulated Task Manager
// values). Input wakes the loop immediately, so interaction stays responsive.
constexpr double kIdleFrameSeconds = 1.0 / 30.0;

void GlfwErrorCallback(int code, const char* description) {
  std::fprintf(stderr, "[GLFW] error %d: %s\n", code, description);
}

}  // namespace

Application::~Application() { Shutdown(); }

bool Application::Init() {
  glfwSetErrorCallback(GlfwErrorCallback);
  if (!glfwInit()) {
    std::fprintf(stderr, "[init] glfwInit failed\n");
    return false;
  }
  glfw_ready_ = true;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kGlMajorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kGlMinorVersion);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(kInitialWidth, kInitialHeight,
                             "CSOPESY Desktop OS", nullptr, nullptr);
  if (window_ == nullptr) {
    std::fprintf(stderr,
                 "[init] could not create window with OpenGL %d.%d core "
                 "context\n",
                 kGlMajorVersion, kGlMinorVersion);
    Shutdown();
    return false;
  }
  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1);  // Vsync: never faster than the monitor refresh rate.
  glfwSetWindowSizeLimits(window_, kMinWidth, kMinHeight, GLFW_DONT_CARE,
                          GLFW_DONT_CARE);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  imgui_ready_ = true;
  ImGui::GetIO().IniFilename = nullptr;  // Same window layout every launch.
  theme::Apply(state_.theme_index);
  applied_theme_index_ = state_.theme_index;

  if (!ImGui_ImplGlfw_InitForOpenGL(window_, true)) {
    std::fprintf(stderr, "[init] ImGui GLFW backend failed\n");
    Shutdown();
    return false;
  }
  glfw_backend_ready_ = true;

  if (!ImGui_ImplOpenGL3_Init(kGlslVersion)) {
    std::fprintf(stderr, "[init] ImGui OpenGL3 backend failed\n");
    Shutdown();
    return false;
  }
  opengl_backend_ready_ = true;

  return true;
}

void Application::Run() {
  // Exits on PWR (state_.shutdown_requested) or the native close button.
  while (!state_.shutdown_requested && !glfwWindowShouldClose(window_)) {
    // Sleep until input arrives or the idle timeout passes. Vsync alone did
    // not stop busy-looping on the Intel Arc test machine.
    glfwWaitEventsTimeout(kIdleFrameSeconds);

    // Minimized: block until something happens instead of spinning.
    if (glfwGetWindowAttrib(window_, GLFW_ICONIFIED)) {
      glfwWaitEvents();
      continue;
    }

    RenderFrame();
  }
}

void Application::RenderFrame() {
  // Settings writes theme_index during a frame; restyle before the next one
  // starts, and only when it actually changed.
  if (state_.theme_index != applied_theme_index_) {
    theme::Apply(state_.theme_index);
    applied_theme_index_ = state_.theme_index;
  }

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // Order matters: wallpaper first, then the clock/PWR strip and taskbar, then
  // the app windows (which clamp themselves above the taskbar).
  // system_controls runs BEFORE taskbar on purpose: ImGui inserts a new
  // NoBringToFrontOnFocus window at the back of the stack, so the window
  // created last ends up lowest. In the planned order the opaque taskbar
  // covered the clock and swallowed PWR clicks.
  desktop::Render(state_);
  system_controls::Render(state_);
  taskbar::Render(state_);
  file_explorer::Render(state_);
  settings::Render(state_);
  task_manager::Render(state_);

  ImGui::Render();

  int framebuffer_width = 0;
  int framebuffer_height = 0;
  glfwGetFramebufferSize(window_, &framebuffer_width, &framebuffer_height);
  glViewport(0, 0, framebuffer_width, framebuffer_height);
  glClearColor(0.04f, 0.055f, 0.12f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window_);
}

void Application::Shutdown() {
  // Reverse order of Init(). Each step runs only if its init step succeeded,
  // so this is safe after a partial init and safe to call twice.
  if (opengl_backend_ready_) {
    ImGui_ImplOpenGL3_Shutdown();
    opengl_backend_ready_ = false;
  }
  if (glfw_backend_ready_) {
    ImGui_ImplGlfw_Shutdown();
    glfw_backend_ready_ = false;
  }
  if (imgui_ready_) {
    ImGui::DestroyContext();
    imgui_ready_ = false;
  }
  if (window_ != nullptr) {
    glfwDestroyWindow(window_);
    window_ = nullptr;
  }
  if (glfw_ready_) {
    glfwTerminate();
    glfw_ready_ = false;
  }
}

}  // namespace desktop_os
