// Application setup, main loop, and shutdown.

#ifndef DESKTOP_OS_CORE_APPLICATION_H_
#define DESKTOP_OS_CORE_APPLICATION_H_

#include "GLFW/glfw3.h"
#include "core/app_state.h"

namespace desktop_os {

// Owns the GLFW window, OpenGL context, ImGui context, and main loop. Create an Application, call Init(), and then call Run().
class Application {
 public:
  Application() = default;
  Application(const Application&) = delete;
  Application& operator=(const Application&) = delete;
  ~Application();

  // Create the window and start OpenGL and ImGui. Return false if setup fails. Print an error and release resources that were created.
  bool Init();

  // Run until shutdown is requested or the user closes the window.
  void Run();

  // Release resources in the reverse order of setup. Safe to call more than once, including after Init() fails. The destructor calls it.
  void Shutdown();

 private:
  // Draw one frame. Draw the desktop first, then the other modules.
  void RenderFrame();

  GLFWwindow* window_ = nullptr;
  bool glfw_ready_ = false;
  bool imgui_ready_ = false;
  bool glfw_backend_ready_ = false;
  bool opengl_backend_ready_ = false;
  // Store the theme index last applied by theme::Apply(). Compare it with state_.theme_index to detect changes.
  int applied_theme_index_ = 0;
  AppState state_;
};

}  // namespace desktop_os

#endif  // DESKTOP_OS_CORE_APPLICATION_H_
