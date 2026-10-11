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

// Use OpenGL 3.3 and GLSL 330 for the ImGui OpenGL backend.
constexpr int kGlMajorVersion = 3;
constexpr int kGlMinorVersion = 3;
constexpr const char* kGlslVersion = "#version 330";

constexpr int kInitialWidth = 1280;
constexpr int kInitialHeight = 720;
constexpr int kMinWidth = 800;
constexpr int kMinHeight = 500;

// Wait up to this interval when idle. The clock and sample values still update.
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
  // Stop when PWR is clicked or the window is closed.
  while (!state_.shutdown_requested && !glfwWindowShouldClose(window_)) {
    // Wait for input or the idle timeout to avoid busy looping.
    glfwWaitEventsTimeout(kIdleFrameSeconds);

    // When minimized, wait for an event instead of drawing frames.
    if (glfwGetWindowAttrib(window_, GLFW_ICONIFIED)) {
      glfwWaitEvents();
      continue;
    }

    RenderFrame();
  }
}

void Application::RenderFrame() {
  // Apply the selected theme after the settings value changes.
  if (state_.theme_index != applied_theme_index_) {
    theme::Apply(state_.theme_index);
    applied_theme_index_ = state_.theme_index;
  }

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // Draw the wallpaper, system controls, taskbar, and app windows in this order. Draw system controls before the taskbar so the taskbar does not cover the clock or PWR button.
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
  // Release resources in the reverse order of setup. Check each setup flag. This is safe after a failure and after more than one call.
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
