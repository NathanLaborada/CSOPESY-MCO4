// Program entry point.

#include "core/application.h"

int main() {
  desktop_os::Application app;
  if (!app.Init()) return 1;
  app.Run();
  return 0;  // The destructor of `app` performs the orderly shutdown.
}
