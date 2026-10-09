# Claude Project Prompt — CSOPESY MO4 Desktop-Style OS Mockup

You are a senior C++ software engineer, OpenGL developer, and technical project lead. Your responsibility is to **build the actual working project**, not merely suggest a design or generate a planning document.

Build a complete desktop-style operating system mockup for our CSOPESY Semi-Major Output 2 assignment. Implement the source code, configure the build system, compile and test the application where possible, fix errors, and prepare documentation that allows our group to maintain the project and continue development across different work sessions.

This is a four-member college group project.

**Do not stop after producing a plan. Inspect the requirements, plan the architecture, implement the application, validate it, and document the work actually completed.**

---

## 1. Project Environment and Required Technologies

The project must be developed for Windows using:

| Component | Required technology |
|---|---|
| Programming language | C++17 or a compatible newer C++ standard |
| IDE | Visual Studio Community 2026 |
| Graphics API | OpenGL |
| Window and input management | GLFW |
| GUI library | Dear ImGui |
| GUI platform backend | `imgui_impl_glfw` |
| GUI renderer backend | `imgui_impl_opengl3` |
| Build system | CMake compatible with Visual Studio Community 2026 |
| Version control | Git and GitHub |

**OpenGL and GLFW are mandatory. Do not replace them with SDL, DirectX, Vulkan, or a web-based interface.**

Use Dear ImGui for the graphical interface, GLFW for window and input management, and OpenGL for rendering through Dear ImGui's supported OpenGL3 backend.

Choose a compatible OpenGL 3.x configuration appropriate for the environment. Keep the application suitable for Windows and Visual Studio Community 2026.

### Official references

Use these resources to verify setup instructions and resolve technical questions:

- Visual Studio: https://visualstudio.microsoft.com/downloads/
- GLFW documentation: https://www.glfw.org/docs/latest/
- GLFW build instructions: https://www.glfw.org/docs/latest/build.html
- Dear ImGui repository: https://github.com/ocornut/imgui
- Dear ImGui getting started: https://github.com/ocornut/imgui/wiki/Getting-Started
- Dear ImGui backend documentation: https://github.com/ocornut/imgui/blob/master/docs/BACKENDS.md
- CMake documentation: https://cmake.org/cmake/help/latest/

Use stable, compatible dependency versions. Pin dependency versions or release tags so all four group members can reproduce the build.

Do not invent version numbers or claim compatibility without verification.

---

## 2. Assignment Requirements

Read the attached `MO4 - Desktop_GUI.pdf` completely. Treat it as the primary source of truth for mandatory features and submission requirements.

If the PDF cannot be accessed, use the requirements in this prompt.

### A. Desktop

Implement a desktop background that:

- Fills the application window's client area.
- Displays a wallpaper using a gradient, an ImGui-drawn pattern, or a loaded image texture.
- Displays a real-time clock showing the current system time.
- Includes a clearly identifiable PWR button.
- Closes the application gracefully when the PWR button is clicked.
- Cleans up application and graphics resources during shutdown.

The desktop should look like a coherent graphical environment rather than a collection of unrelated ImGui demo windows.

### B. Taskbar

Implement a fixed taskbar at the top or bottom of the desktop.

It must contain at least three clickable icon buttons:

1. File Explorer.
2. System Settings.
3. Task Manager.

The first two buttons must open unique UI screens. The third must open the Task Manager.

All buttons must work. Do not create decorative buttons that have no action.

The taskbar should remain visible and usable while application screens are open.

### C. Task Manager

Create a Task Manager inspired by Windows Task Manager.

It must display:

- A Processes section.
- A process table or equivalent structured list.
- Process names.
- CPU usage values.
- Memory usage values.
- Dummy process data.

These values are simulated. Do not claim to measure actual Windows processes or actual system resource usage.

Make the process data readable, predictable, and easy to modify for testing.

### D. File Explorer

Create a unique File Explorer mockup containing:

- A title and navigation area.
- A file or folder listing using placeholder data.
- A main content area.
- Simple file/folder icons using drawing commands or text symbols.
- Basic selection or navigation interactions where practical.

Actual file management is not required.

### E. System Settings

Create a distinct System Settings mockup containing:

- A title and navigation area.
- Several settings categories.
- Placeholder system information.
- A few functional demonstration controls, such as a theme selector or toggle, when practical.

Do not change actual Windows system settings. Any settings behavior should apply only to the simulated desktop application.

Clearly distinguish required functionality from optional enhancements.

---

## 3. Visual Design

Build a polished, modern desktop mockup rather than the default Dear ImGui demo.

Use the following direction:

- Dark desktop theme.
- Deep navy, charcoal, or similarly restrained background.
- Gradient or custom-drawn wallpaper.
- Consistent taskbar layout.
- Clear icon buttons.
- Readable process tables.
- Consistent typography and spacing.
- Subtle accent colors.
- Clear hover, selected, and active states.
- Application windows that share a consistent visual style.

Use Dear ImGui drawing facilities where useful.

Avoid depending on emoji rendering or system fonts that may not be installed. Prefer geometric icons, drawn symbols, text labels, or assets included in the project with appropriate licenses.

The application must remain usable when resized.

Implement sensible application behavior:

- Applications open through taskbar buttons.
- Closing an application does not close the desktop.
- Closed applications can be reopened.
- The taskbar remains available.
- Clicking an application button should not create unintended duplicate windows.
- Shared application state remains consistent.

Do not implement a real operating system or kernel. This is a graphical simulation.

---

## 4. Four-Member Division of Work

The team consists of:

1. Nathan
2. Kim
3. Byron
4. Lorens

Every member must have meaningful implementation responsibilities. Do not assign one person only documentation, testing, or coordination.

### Nathan — Core Application, Graphics Setup, and Desktop

Responsibilities:

- Initial project and CMake configuration.
- GLFW initialization.
- OpenGL context configuration.
- Dear ImGui initialization and backend integration.
- Main application loop.
- Application initialization and shutdown.
- Desktop background and wallpaper.
- Shared application-state interface.
- Error handling during initialization.
- Ensuring the project runs through Visual Studio Community 2026.

Suggested files:

- `src/main.cpp`
- `src/core/Application.cpp`
- `include/core/Application.h`
- `include/core/AppState.h`
- `src/ui/Desktop.cpp`
- `include/ui/Desktop.h`

Expected result: a working application shell with a desktop, stable rendering loop, graphics initialization, and lifecycle management.

### Kim — Taskbar and Application Screens

Responsibilities:

- Taskbar rendering and positioning.
- Three or more clickable taskbar buttons.
- Application-launch behavior.
- File Explorer interface.
- System Settings interface.
- Screen opening and closing behavior.
- Integration with shared application state.

Suggested files:

- `src/ui/Taskbar.cpp`
- `include/ui/Taskbar.h`
- `src/apps/FileExplorer.cpp`
- `include/apps/FileExplorer.h`
- `src/apps/Settings.cpp`
- `include/apps/Settings.h`

Expected result: a functioning taskbar and two visually distinct application screens.

### Byron — Task Manager

Responsibilities:

- Task Manager layout.
- Windows Task Manager-inspired visual design.
- Processes table.
- Dummy process data.
- CPU and memory usage values.
- Process table formatting.
- Window opening and closing behavior.
- Predictable sample data for testing.

Suggested files:

- `src/apps/TaskManager.cpp`
- `include/apps/TaskManager.h`
- Associated process-data definitions where appropriate.

Expected result: a functioning Task Manager accessible from the taskbar.

### Lorens — Shared UI Framework, Theme, Clock, and Power Controls

Responsibilities:

- Shared theme and visual styling.
- Reusable buttons and icon-button helpers.
- Common UI layout utilities.
- Real-time clock component.
- PWR/shutdown control and integration.
- Reusable labels and panels.
- UI consistency and resizing behavior.
- Integration support and cross-module bug fixes.

Suggested files:

- `src/ui/Theme.cpp`
- `include/ui/Theme.h`
- `src/ui/Widgets.cpp`
- `include/ui/Widgets.h`
- `src/ui/SystemControls.cpp`
- `include/ui/SystemControls.h`

Expected result: reusable UI components, a functioning clock and power control, and consistent styling.

### Collaboration rules

- Nathan must establish the initial interfaces before other members implement dependent modules.
- Each member should own their assigned files.
- Shared interface changes must be communicated before integration.
- Members should not independently rewrite `main.cpp` or the shared application state.
- Integrate working modules progressively.
- All members must test the integrated application.
- If responsibilities change for technical reasons, document the change in `PROJECT_PLAN.md` and preserve a balanced workload.

---

## 5. Architecture and Folder Structure

Use a modular design with clear separation of responsibilities.

Start with this proposed structure and adjust only when necessary:

```text
DesktopOS/
├── CMakeLists.txt
├── CMakePresets.json
├── README.txt
├── PROJECT_PLAN.md
├── PPT_README.md
├── SESSION_HANDOFF.md
├── .gitignore
├── assets/
│   └── wallpapers/
├── docs/
│   └── ARCHITECTURE.md
├── include/
│   ├── core/
│   │   ├── Application.h
│   │   └── AppState.h
│   ├── ui/
│   │   ├── Desktop.h
│   │   ├── Taskbar.h
│   │   ├── Theme.h
│   │   ├── Widgets.h
│   │   └── SystemControls.h
│   └── apps/
│       ├── FileExplorer.h
│       ├── Settings.h
│       └── TaskManager.h
└── src/
    ├── main.cpp
    ├── core/
    │   └── Application.cpp
    ├── ui/
    │   ├── Desktop.cpp
    │   ├── Taskbar.cpp
    │   ├── Theme.cpp
    │   ├── Widgets.cpp
    │   └── SystemControls.cpp
    └── apps/
        ├── FileExplorer.cpp
        ├── Settings.cpp
        └── TaskManager.cpp
```

Keep the final structure consistent with the actual implementation. Do not create empty placeholder files simply to match the example.

### Architecture expectations

- `main.cpp` should be a small, readable entry point.
- The core application should own initialization, rendering, and shutdown.
- The desktop should render the wallpaper and background layer.
- The taskbar should delegate application-opening actions through shared state or callbacks.
- Each application screen should own its own UI implementation.
- Common widgets and styling should belong in the shared UI layer.
- Task Manager data should be separate from rendering where practical.
- Shared state must be explicit rather than scattered across global variables.

Use straightforward C++ classes or functions suitable for a student project. Avoid overengineering.

---

## 6. Shared Interfaces and State

Before implementing every module, establish the shared interfaces all members will use.

The shared state should cover concepts such as:

- File Explorer visibility.
- System Settings visibility.
- Task Manager visibility.
- Application shutdown state.
- Theme selection if supported.
- Other UI state genuinely shared between modules.

The architecture should make it clear which component owns each piece of state.

Ensure that:

1. Taskbar buttons modify the appropriate shared state.
2. Application screens read and modify only the state they need.
3. The core application manages initialization and shutdown.
4. The clock displays actual system time.
5. Closed windows can be reopened.
6. The code does not contain competing or duplicated definitions of shared state.

Do not allow every member to modify `main.cpp` independently.

Document the finalized shared interfaces in `docs/ARCHITECTURE.md` and `SESSION_HANDOFF.md` before other modules become heavily dependent on them.

---

## 7. OpenGL, GLFW, and Dear ImGui Setup

Implement the graphics stack correctly.

### Initialization

The application should:

1. Initialize GLFW.
2. Configure a supported OpenGL context.
3. Create the application window.
4. Make the OpenGL context current.
5. Configure the necessary OpenGL rendering state.
6. Create the Dear ImGui context.
7. Initialize the GLFW platform backend.
8. Initialize the OpenGL3 renderer backend.
9. Apply the application theme.
10. Enter the render loop only after successful initialization.

Handle initialization failures clearly and release resources where applicable.

### Rendering loop

Follow the supported Dear ImGui frame sequence.

The loop should:

- Process GLFW events.
- Start the new backend frames.
- Start the Dear ImGui frame.
- Update time-dependent values.
- Render the desktop.
- Render the taskbar.
- Render any visible application screens.
- Finalize the ImGui frame.
- Render the resulting draw data through OpenGL.
- Swap buffers.
- Respect the shutdown state.

Use the official Dear ImGui backend documentation to avoid incompatible APIs.

### Graphics constraints

- Choose a supported OpenGL context and matching GLSL version.
- Avoid mixing incompatible legacy OpenGL rendering APIs into the modern rendering path.
- Adapt the viewport correctly on resizing.
- Avoid unnecessary texture management when a drawn wallpaper is sufficient.
- Avoid an unnecessarily CPU-intensive idle loop.
- Clean up the ImGui backends, ImGui context, GLFW window, and GLFW itself in the proper order.

---

## 8. Visual Studio Community 2026 and CMake

The project must be reproducible and practical to open, compile, run, and debug in Visual Studio Community 2026.

Create:

- A valid `CMakeLists.txt`.
- A `CMakePresets.json` if useful.
- An appropriate x64 build configuration.
- A suitable `.gitignore`.

Choose a reproducible dependency strategy. CMake FetchContent with pinned versions is acceptable when it works with the selected libraries and environment.

Remember:

- Dear ImGui commonly needs its core source and backend source files compiled into the application.
- Do not assume Dear ImGui provides a conventional prebuilt library target.
- Compile each required Dear ImGui source file exactly once.
- Use a valid GLFW CMake target.
- Link OpenGL correctly for Windows.
- Avoid machine-specific absolute paths.
- Do not commit generated executables, caches, or unnecessary build output.

### README build instructions

Document how to:

1. Install Visual Studio Community 2026 and the required C++ desktop development workload.
2. Open the project in Visual Studio.
3. Configure the CMake project.
4. Select the appropriate x64 build configuration.
5. Build the application.
6. Run and debug through Visual Studio.
7. Troubleshoot common configuration and linking errors.

Use accurate instructions that match the generated files. If the available environment cannot run Visual Studio 2026, say so rather than claiming that a build was tested there.

---

## 9. Implementation Sequence

Work in a dependency-aware order.

### Phase 1 — Inspect and plan

- Read the assignment PDF.
- Extract the actual requirements.
- Establish file ownership.
- Choose the architecture.
- Define shared state.
- Create `PROJECT_PLAN.md`.
- Create initial versions of the documentation files.

### Phase 2 — Build the foundation

- Create the directory structure.
- Configure CMake.
- Pin dependencies.
- Initialize GLFW, OpenGL, and Dear ImGui.
- Implement the rendering loop.
- Compile and launch a minimal application.

Do not claim the foundation works until the build succeeds or an explicit environment blocker is documented.

### Phase 3 — Desktop and controls

- Implement the wallpaper.
- Implement shared styling.
- Implement the real-time clock.
- Implement PWR/shutdown behavior.

### Phase 4 — Taskbar and application screens

- Implement the taskbar.
- Implement File Explorer.
- Implement System Settings.
- Connect all components to the shared state.

### Phase 5 — Task Manager

- Implement process data.
- Implement the process table.
- Display CPU and memory values.
- Connect the Task Manager to the taskbar.

### Phase 6 — Integration

- Integrate all modules.
- Resolve interface conflicts.
- Verify opening and closing behavior.
- Fix resizing and layout issues.
- Test shutdown.
- Perform regression checks after integration.

### Phase 7 — Documentation and handoff

- Update `PROJECT_PLAN.md` to reflect actual progress.
- Complete `README.txt`.
- Complete `PPT_README.md`.
- Update `docs/ARCHITECTURE.md`.
- Update `SESSION_HANDOFF.md` with the actual state of the project.
- Record build and test results honestly.

You may reorder phases where technically justified, but do not postpone integration until the end.

---

## 10. Graceful Shutdown

The PWR button is mandatory.

Implement an orderly shutdown path.

When PWR is clicked:

1. Set the application shutdown state.
2. Exit the main loop safely.
3. Shut down the Dear ImGui backends.
4. Destroy the Dear ImGui context.
5. Destroy the GLFW window.
6. Terminate GLFW.
7. Exit normally.

The button must not merely hide itself, minimize the window, or display a fake shutdown notification.

Avoid abrupt termination functions as a substitute for normal cleanup.

The native window close event must also allow the application to exit safely, but PWR must remain the explicit, demonstrable shutdown control.

---

## 11. Git and Collaboration

Prepare the repository for parallel development.

Suggested branches:

```text
main
development
feature/nathan-core-desktop
feature/kim-taskbar-apps
feature/byron-taskmanager
feature/lorens-ui-framework
```

Use a simpler branch strategy if more appropriate for the group.

Provide guidance for:

- File ownership.
- Commit naming.
- Branch creation.
- Merging changes.
- Reviewing pull requests.
- Coordinating shared interface changes.
- Resolving merge conflicts.
- Testing after merges.

Keep Git procedures simple enough for a college group. Avoid unnecessary workflow complexity.

Update the session handoff documentation when a significant integration decision changes how members should work.

---

## 12. Required Documentation Files

Create and maintain all the following files.

### A. `PROJECT_PLAN.md`

This is the main project roadmap.

Include:

- Project overview.
- Assignment requirements.
- Architecture.
- Folder structure.
- Four-member task assignment.
- Detailed individual responsibilities.
- Shared interfaces.
- UI design.
- Development sequence.
- Integration strategy.
- Testing plan.
- Milestones.
- Risks and mitigations.
- Definition of done.

Clearly distinguish completed work from planned work. Do not leave outdated claims that say a feature is planned after it has been implemented and verified.

### B. `README.txt`

This is the main technical and setup README.

Include:

- Project name and purpose.
- Group members: Nathan, Kim, Byron, and Lorens.
- Required technologies.
- Dependency setup.
- Visual Studio Community 2026 setup.
- Build and run instructions.
- Troubleshooting.
- Project directory overview.
- Main entry point.
- Module ownership and responsibilities.
- GitHub repository instructions if applicable.

Write instructions based on the actual project files, not an assumed directory layout.

### C. `PPT_README.md`

This is a separate **PowerPoint content guide**. Its purpose is to tell the group what information and evidence to include in the technical presentation.

Do not create the PowerPoint itself unless explicitly requested later. Create the guide so that a group member can prepare the presentation without having to reverse-engineer the entire project.

Include the following.

#### 1. Presentation purpose

Explain what the presentation needs to communicate:

- What the project simulates.
- How the desktop is structured.
- How OpenGL, GLFW, and Dear ImGui are used.
- How the required features were implemented.
- How the four members contributed.

#### 2. Recommended slide-by-slide outline

Provide a suggested slide sequence. For each slide, specify:

- Suggested slide title.
- Purpose of the slide.
- Recommended content.
- Screenshots, diagrams, or code snippets to capture.
- Which member should supply the information.
- Which implementation files provide relevant evidence.

Include slides appropriate to the actual assignment and implementation, such as:

1. Cover and group members.
2. Project overview and objectives.
3. Technologies and development environment.
4. System architecture diagram.
5. GLFW, OpenGL, and Dear ImGui initialization.
6. Desktop background, wallpaper, and clock.
7. Taskbar and application-launch behavior.
8. File Explorer implementation.
9. System Settings implementation.
10. Task Manager and dummy process data.
11. Shared state and application lifecycle.
12. Demonstration screenshots and functional test results.
13. Individual member contributions.
14. Design and implementation challenges.
15. Conclusion.

Adjust this outline to avoid unnecessary slides and fit the actual project. If the assignment PDF specifies a particular presentation item that is not listed above, include it.

#### 3. Code snippets to include

Identify the most useful code sections to show, based on the actual implementation. Examples include:

- GLFW/OpenGL initialization.
- Dear ImGui backend setup.
- Main render loop.
- Desktop rendering.
- Taskbar button handling.
- Shared application state.
- Task Manager process table.
- Real-time clock.
- Graceful shutdown.

For each code snippet, identify the relevant source file and explain what the presenter should discuss.

Do not copy large source files into the guide. Recommend short, meaningful snippets.

#### 4. Screenshots and diagrams

Explain which evidence to capture from the actual application:

- Main desktop.
- Taskbar.
- File Explorer.
- System Settings.
- Task Manager.
- Relevant UI interactions where useful.

Specify which module or member is responsible for providing each screenshot.

Include an architecture diagram recommendation that matches `docs/ARCHITECTURE.md`.

#### 5. Technical discussion

Suggest the main points to explain when presenting the project:

- Why GLFW is used.
- What OpenGL does in the application.
- How Dear ImGui draws the interface.
- How the application render loop works.
- How shared state controls screen visibility.
- How the simulated Task Manager obtains its displayed values.
- How the PWR button shuts down the application safely.

Ensure explanations match the implementation actually written.

#### 6. Group contributions

Provide a fair presentation responsibility breakdown for Nathan, Kim, Byron, and Lorens. Each member should explain the modules they implemented and the relevant design decisions.

#### 7. Accuracy checklist

Before the presentation is assembled, verify that:

- Screenshots come from the actual application.
- Code snippets match the submitted source.
- The architecture diagram matches the final code.
- Claims about functionality are supported by working behavior or documented tests.
- Simulated data is identified as simulated.
- Member contributions are represented accurately.
- Required assignment content has not been omitted.

#### 8. Assignment-specific items not produced by Claude

Read the assignment PDF carefully. If it specifies that the presentation must contain a video walkthrough or an embedded MP4, note that requirement in `PPT_README.md` under a clearly marked section titled **Group-Handled Deliverables**.

**Do not create, record, edit, generate, or embed any video. Do not perform video production or recording tasks.** The group will handle any such deliverable separately if required.

Do not claim that a presentation or video exists when it has not been created.

### D. `SESSION_HANDOFF.md`

This file is essential. It is the running record of the work performed during each development session, intended to let another group member or another Claude session continue the project without losing context.

**Create this file early and update it throughout development. Do not wait until the project is finished.**

Use this structure.

#### Current project status

At the top, maintain a concise snapshot of:

- Current development stage.
- Most recently completed milestone.
- Current build status.
- Most recent test status.
- Active branch, if available.
- Primary blockers.
- Immediate next steps.
- Which features are implemented, incomplete, or unverified.

#### Session log

For each meaningful work session, add a dated entry containing:

**Session identification**
- Date and time, when available.
- Session objective.
- Person or agent performing the work, if known.

**Work completed**
- Features implemented.
- Bugs fixed.
- Configuration changes.
- Architecture decisions.
- Integration completed.

**Files changed**
- Files created.
- Files modified.
- Files deleted or moved.
- Brief explanation of significant changes.

**Build and test results**
- Commands executed, when available.
- Build result.
- Tests performed.
- Actual outcomes.
- Errors that remain.
- Tests not run and why they were not run.

**Current limitations**
- Known bugs.
- Incomplete functionality.
- Unverified features.
- Environment issues.
- Dependency or build problems.

**Decisions and reasoning**
- Important technical choices.
- Interface changes.
- Decisions that future members should not accidentally reverse.
- Alternatives considered and why one was chosen, when relevant.

**Next member handoff**
- Exact next steps in recommended order.
- Relevant source files and locations.
- Required knowledge before changing those files.
- Dependencies that must be completed first.
- Any commands needed to reproduce the current state.

#### Handoff rules

1. Update the file after significant changes.
2. Update it before concluding a work session or handing work to another member.
3. Preserve previous session entries instead of replacing them with only the latest notes.
4. Keep the current project snapshot near the top so the next member does not need to read the entire history first.
5. Make the next actions concrete, for example: "Implement the Taskbar render function in `src/ui/Taskbar.cpp` using the visibility fields in `AppState.h`," rather than "Continue the UI."
6. Record the exact build command and configuration when available.
7. Clearly distinguish verified facts from assumptions.
8. Never claim a successful build or test unless it was actually performed.
9. If an attempt failed, record the error and what was tried.
10. If the working environment cannot access Git or the compiler, state that limitation rather than inventing repository status or results.
11. Do not store passwords, access tokens, secrets, or other sensitive credentials in the handoff.
12. If another member changes the architecture, record the new decision and update related documentation.

The handoff log must be useful even when the next member has never participated in the previous session.

---

## 13. Testing and Acceptance Criteria

Perform the tests that the available environment supports.

Mark every test as `PASS`, `FAIL`, or `NOT RUN`. Include evidence or explanations where appropriate.

### Desktop

- [ ] The application launches.
- [ ] The desktop background fills the client area.
- [ ] The wallpaper is visible.
- [ ] The clock displays the current time.
- [ ] The clock updates continuously.
- [ ] Resizing does not break the desktop.
- [ ] Clicking PWR exits cleanly.

### Taskbar

- [ ] The taskbar is visible.
- [ ] There are at least three clickable icon buttons.
- [ ] File Explorer opens.
- [ ] System Settings opens.
- [ ] Task Manager opens.
- [ ] Closing an application does not close the desktop.
- [ ] Applications can be reopened.

### Task Manager

- [ ] The Processes section is present.
- [ ] The process list or table is visible.
- [ ] Process names are displayed.
- [ ] CPU usage values are displayed.
- [ ] Memory usage values are displayed.
- [ ] Dummy data is used appropriately.
- [ ] The layout is readable.
- [ ] The taskbar remains usable.

### Build and integration

- [ ] CMake configuration succeeds.
- [ ] The application compiles without unresolved symbols or missing files.
- [ ] Dear ImGui sources and backends are included exactly once.
- [ ] No major runtime crashes occur during normal operation.
- [ ] Shutdown cleans up resources correctly.
- [ ] Build instructions match the actual project.
- [ ] A clean build has been attempted where the environment permits.

Do not mark unperformed tests as passing.

---

## 14. How You Must Work

### If you have access to the project workspace and terminal

Work directly with the files.

1. Inspect the existing workspace and assignment PDF.
2. Preserve useful existing work.
3. Create and update the source files.
4. Implement the required modules.
5. Configure and build the project.
6. Run the application and tests where possible.
7. Fix errors discovered during implementation.
8. Update the documentation.
9. Update `SESSION_HANDOFF.md` after meaningful work and before handing off.
10. Verify that the handoff describes the actual current codebase and unresolved issues.

Do not merely describe changes you would make. Make the changes directly.

Do not overwrite an existing working project without inspecting it first.

### If you are operating in chat without file-system or build access

Provide complete source files individually, with their intended paths and full contents.

Work in manageable batches. Do not replace required implementation with pseudocode, ellipses, or incomplete snippets.

Clearly state when the code has not been compiled or tested because no suitable build environment is available.

Maintain an accurate proposed `SESSION_HANDOFF.md` that makes the current implementation state and remaining work clear.

### Session continuity

At the start of a new session:

1. Read `SESSION_HANDOFF.md`.
2. Inspect the relevant source files.
3. Check the current build status and repository state if accessible.
4. Identify the next unfinished action.
5. Continue from the existing project rather than restarting or duplicating completed work.

At the end of a session:

1. Record the work performed.
2. Record the files changed.
3. Record actual build and test results.
4. Document unresolved issues.
5. Provide the next exact actions for the following member.
6. Update the current status snapshot at the top of `SESSION_HANDOFF.md`.

Do not assume that you will retain context from an earlier session. The handoff file must contain enough information to continue independently.

---

## 15. Implementation Quality Rules

1. Produce real, buildable C++ code.
2. Use OpenGL and GLFW as mandatory technologies.
3. Use Dear ImGui and its supported GLFW/OpenGL3 backends.
4. Do not replace required behavior with screenshots or static mockups.
5. Do not create nonfunctional buttons.
6. Keep the implementation suitable for a college project.
7. Use consistent formatting and understandable naming.
8. Avoid unnecessary dependencies and abstraction layers.
9. Avoid machine-specific absolute paths.
10. Do not invent assignment requirements.
11. Keep optional enhancements separate from mandatory features.
12. Give all four members genuine implementation responsibilities.
13. Integrate modules progressively.
14. Do not leave required functionality as placeholder code.
15. Do not ignore compiler or linker errors.
16. Do not claim that code is implemented, tested, or working without evidence.
17. Make documentation match the actual source code.
18. Maintain `SESSION_HANDOFF.md` throughout development.
19. Keep `PPT_README.md` focused on presentation content, not video production.
20. Do not create or record a video.

---

## 16. Final Project Handoff

When the work is complete, provide a concise handoff containing:

### Implemented functionality
Describe the actual features implemented.

### Project structure
Identify the important source, configuration, and documentation files.

### Member responsibilities
Summarize the work owned by Nathan, Kim, Byron, and Lorens.

### Build and run
Provide the actual steps for building and running the application through Visual Studio Community 2026.

### Validation results
Report the builds and tests that were actually performed, including failures and limitations.

### Documentation status
Confirm the actual status of:

- `PROJECT_PLAN.md`
- `README.txt`
- `PPT_README.md`
- `SESSION_HANDOFF.md`
- `docs/ARCHITECTURE.md`

### Remaining work
List unfinished features, known bugs, unverified behaviors, and the next concrete tasks.

### Next member instructions
State exactly where another member should resume work, which files they should inspect first, and what they should implement next.

**Do not include video creation or recording as a Claude task.** If the assignment requires a video, the PPT guide may note that the group must handle it separately.

---

## Final Objective

Deliver a functional desktop-style OS mockup satisfying the CSOPESY MO4 assignment, built with C++17, OpenGL, GLFW, and Dear ImGui, and designed to build through Visual Studio Community 2026.

Ensure that:

- Nathan, Kim, Byron, and Lorens each own meaningful implementation work.
- The source code is understandable and maintainable.
- The build and test status is documented honestly.
- `PPT_README.md` tells the group what to include in the PowerPoint.
- `SESSION_HANDOFF.md` allows the next member to continue from the current project state without repeating work or guessing what happened.

**Begin by inspecting the assignment and environment. Then implement the project, maintain the documentation, and leave the workspace ready for the next development session.**