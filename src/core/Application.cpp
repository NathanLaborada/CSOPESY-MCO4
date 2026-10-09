#include "core/Application.h"

#include "ui/Desktop.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <cstdio>

namespace {

// OpenGL 3.3 core is supported by every Windows GPU driver from the last
// decade and matches GLSL "#version 330" for the ImGui OpenGL3 backend.
constexpr int kGlMajor = 3;
constexpr int kGlMinor = 3;
constexpr const char* kGlslVersion = "#version 330";

// Redraw rate when nothing is happening (clock, simulated Task Manager values).
// Input wakes the loop immediately, so interaction stays responsive.
constexpr double kIdleFrameSeconds = 1.0 / 30.0;

void glfwErrorCallback(int code, const char* description)
{
    std::fprintf(stderr, "[GLFW] error %d: %s\n", code, description);
}

} // namespace

Application::~Application()
{
    shutdown();
}

bool Application::init()
{
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        std::fprintf(stderr, "[init] glfwInit failed\n");
        return false;
    }
    glfwReady_ = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kGlMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kGlMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(1280, 720, "CSOPESY Desktop OS", nullptr, nullptr);
    if (!window_) {
        std::fprintf(stderr, "[init] could not create window with OpenGL %d.%d core context\n",
                     kGlMajor, kGlMinor);
        shutdown();
        return false;
    }
    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1); // vsync: caps the loop at the monitor refresh rate
    glfwSetWindowSizeLimits(window_, 800, 500, GLFW_DONT_CARE, GLFW_DONT_CARE);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    imguiReady_ = true;
    ImGui::GetIO().IniFilename = nullptr; // same window layout on every launch
    ImGui::StyleColorsDark();             // Lorens: replace with the shared Theme

    if (!ImGui_ImplGlfw_InitForOpenGL(window_, true)) {
        std::fprintf(stderr, "[init] ImGui GLFW backend failed\n");
        shutdown();
        return false;
    }
    glfwBackendReady_ = true;

    if (!ImGui_ImplOpenGL3_Init(kGlslVersion)) {
        std::fprintf(stderr, "[init] ImGui OpenGL3 backend failed\n");
        shutdown();
        return false;
    }
    openglBackendReady_ = true;

    return true;
}

void Application::run()
{
    // Exits on PWR (state_.shutdownRequested) or the native window close button.
    while (!state_.shutdownRequested && !glfwWindowShouldClose(window_)) {
        // Sleep until input arrives or the idle timeout passes. Vsync alone did
        // not stop busy-looping on the Intel Arc test machine.
        glfwWaitEventsTimeout(kIdleFrameSeconds);

        // Minimized: block until something happens instead of spinning.
        if (glfwGetWindowAttrib(window_, GLFW_ICONIFIED)) {
            glfwWaitEvents();
            continue;
        }

        renderFrame();
    }
}

void Application::renderFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    Desktop::Render(state_);
    // Integration points: each module adds ONE call here, in this order.
    //   Taskbar::Render(state_);        Kim
    //   SystemControls::Render(state_); Lorens (clock + PWR)
    //   FileExplorer::Render(state_);   Kim
    //   Settings::Render(state_);       Kim
    //   TaskManager::Render(state_);    Byron

    ImGui::Render();

    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window_, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.04f, 0.055f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window_);
}

void Application::shutdown()
{
    // Reverse order of init. Each step runs only if its init step succeeded,
    // so this is safe after a partial init and safe to call twice.
    if (openglBackendReady_) {
        ImGui_ImplOpenGL3_Shutdown();
        openglBackendReady_ = false;
    }
    if (glfwBackendReady_) {
        ImGui_ImplGlfw_Shutdown();
        glfwBackendReady_ = false;
    }
    if (imguiReady_) {
        ImGui::DestroyContext();
        imguiReady_ = false;
    }
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    if (glfwReady_) {
        glfwTerminate();
        glfwReady_ = false;
    }
}
