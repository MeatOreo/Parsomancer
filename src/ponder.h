// PONDER controls the app window itself and configures Hello_ImGui

#include "hello_imgui/hello_imgui.h"
#include <windows.h>
#include <WinUser.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include "parse.h"

// Data
struct AppSettings
{
    std::string name = "Default";
    int monitorSelection = 0;

};

struct AppState
{
    int mode  = 0;
    AppSettings appSettings;
};
// END Data


// Callbacks
void moveAppToMonitor(AppState appState);
// END Callbacks

// Custom
int getCursorMonitorIdx(GLFWwindow* window);
// END Custom

// TEMP
void MyGui();
// END TEMP