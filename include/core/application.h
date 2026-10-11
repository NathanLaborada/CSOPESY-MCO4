// Application lifecycle: window, OpenGL context, Dear ImGui and main loop.

#ifndef DESKTOP_OS_CORE_APPLICATION_H_
#define DESKTOP_OS_CORE_APPLICATION_H_

#include "GLFW/glfw3.h"
#include "core/app_state.h"

namespace desktop_os {

// Owns the GLFW window, the OpenGL context, the Dear ImGui context and the
// main loop. Example:
//   desktop_os::Application app;
//   if (!app.Init()) return 1;
//   app.Run();
class Application {
 public:
  Application() = default;
  Application(const Application&) = delete;
  Application& operator=(const Application&) = delete;
  ~Application();

  // Creates the window and initializes OpenGL and Dear ImGui. Returns false
  // on failure, after printing an error and releasing anything created.
  bool Init();

  // Runs the main loop until shutdown is requested or the window is closed.
  void Run();

  // Releases all resources in reverse order of creation. Safe to call more
  // than once and after a failed Init(); the destructor calls it.
  void Shutdown();

 private:
  // Builds and draws one frame: desktop, then every other module.
  void RenderFrame();

  GLFWwindow* window_ = nullptr;
  bool glfw_ready_ = false;
  bool imgui_ready_ = false;
  bool glfw_backend_ready_ = false;
  bool opengl_backend_ready_ = false;
  // Theme last passed to theme::Apply(); compared each frame with
  // state_.theme_index so the style is rebuilt only when Settings changes it.
  int applied_theme_index_ = 0;
  AppState state_;
};

}  // namespace desktop_os

#endif  // DESKTOP_OS_CORE_APPLICATION_H_
