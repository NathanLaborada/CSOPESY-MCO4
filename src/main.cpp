#include "core/Application.h"

int main()
{
    Application app;
    if (!app.init())
        return 1;
    app.run();
    return 0; // app's destructor performs the orderly shutdown
}
