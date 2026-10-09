#pragma once

#include "core/AppState.h"

struct GLFWwindow;

// Owns the window, OpenGL context, Dear ImGui context and the main loop.
// Usage: Application app; if (!app.init()) return 1; app.run();
class Application {
public:
    Application() = default;
    ~Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    bool init();     // false on failure (already cleaned up, error printed)
    void run();      // returns once shutdown is requested
    void shutdown(); // idempotent; also called by the destructor

private:
    void renderFrame();

    GLFWwindow* window_ = nullptr;
    bool glfwReady_ = false;
    bool imguiReady_ = false;
    bool glfwBackendReady_ = false;
    bool openglBackendReady_ = false;
    AppState state_;
};
