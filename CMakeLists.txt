cmake_minimum_required(VERSION 3.24)
project(DesktopOS LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Use fixed versions so all group members build the same dependencies.
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
    GIT_SHALLOW    TRUE)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.91.9b
    GIT_SHALLOW    TRUE)

FetchContent_MakeAvailable(glfw imgui)

find_package(OpenGL REQUIRED)

# Build Dear ImGui and its GLFW and OpenGL backends as one library.
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

# Add all .cc files under src/. Re-run CMake when you add a source file.
# Use lower_snake_case for source and header file names.
file(GLOB_RECURSE DESKTOPOS_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cc)

add_executable(DesktopOS ${DESKTOPOS_SOURCES})
target_include_directories(DesktopOS PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(DesktopOS PRIVATE imgui)

if(MSVC)
    target_compile_options(DesktopOS PRIVATE /W4 /utf-8)
    # Show error messages in Debug. Hide the console in Release.
    set_target_properties(DesktopOS PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)
    target_link_options(DesktopOS PRIVATE $<$<CONFIG:Release>:/ENTRY:mainCRTStartup>)
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT DesktopOS)
endif()
