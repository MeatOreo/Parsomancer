#include "spellbook.h"
#include "ponder.h"
#include "orb.h"

// CONFIG FUNCTIONS
void configureTransparency();
void configureWindowAPI(HelloImGui::RunnerParams* mainWindow);
// END CONFIG FUNCTIONS


int main(int , char *[])
{

    // INITIALIZE CORE DATA
    AppState appState;
    HelloImGui::RunnerParams mainWindow;
    // END CORE DATA

    // CALL CONFIG FUNCTIONS
    configureTransparency();

    configureWindowAPI(&mainWindow);
    
    setThemeTweaks(appState, &mainWindow);

    // END CALL CONFIG FUNCTIONS

    // Hello_ImGui CALLBACKS
    mainWindow.callbacks.PostInit = [&appState] {moveAppToMonitor(appState);};
    // END Hello_ImGui CALLBACKS

    // TEMP
    mainWindow.callbacks.ShowGui = helpImTrappedInAGuiFactory;

    // TEMP
    appState.appSettings.name = "cursor";

    HelloImGui::Run(mainWindow);

    return 0;
}








// Moves the app window to the settings-selected monitor.
void moveAppToMonitor(AppState appState)
{
    int count; 
    // For use after determining which monitor ID we want
    GLFWmonitor** monitorArray = glfwGetMonitors(&count);
    GLFWwindow* appWindow = (GLFWwindow*) HelloImGui::GetRunnerParams()->backendPointers.glfwWindow;
    
    // Default to primary monitor
    int selectedMonitor = 0;
    
    // Choose monitor based on settings
    if(!(appState.appSettings.name == "default"))
    {
        if(appState.appSettings.name == "cursor")
        {
            selectedMonitor = getCursorMonitorIdx(appWindow);
        }
        else if(appState.appSettings.name == "manual")
        {
            selectedMonitor = appState.appSettings.monitorSelection;
        }
    }

    // Get resolution etc for selected monitor
    const GLFWvidmode* mode = glfwGetVideoMode(monitorArray[selectedMonitor]);
    // Set app window to fullscreen in chosen monitor
    glfwSetWindowMonitor(appWindow, monitorArray[selectedMonitor], 0, 0, 
        mode->width, mode->height, mode->refreshRate);
}

// Returns the monitor IDX of whichever screen the cursor is on
int getCursorMonitorIdx(GLFWwindow* window)
{
    bool success = false;

    // X and Y coord of cursor relative to window
    double cursorPosition[2] = {0};
    glfwGetCursorPos(window, &cursorPosition[0], &cursorPosition[1]);

    // X and Y coord of window relative to virtual desktop space
    int windowPosition[2] = {0};
    glfwGetWindowPos(window, &windowPosition[0], &windowPosition[1]);

    int monitorCount = 0;
    // Holds array of GLFW monitor handles
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

    // Gets cursor position relative to virtual desktop
    cursorPosition[0] += windowPosition[0];
    cursorPosition[1] += windowPosition[1];

    // Iterates through list of monitor handles
    for (int i = 1; ((!success) && (i < monitorCount)); ++i)
    {
        // Find X and Y coord of upper left corner of monitor in virtual space
        int monitorPosition[2] = {0};
        glfwGetMonitorPos(monitors[i], &monitorPosition[0], &monitorPosition[1]);

        // Finds video mode for width and height of display (in screen coordinates, not pixels)
        const GLFWvidmode* monitorVideoMode = glfwGetVideoMode(monitors[i]);

        // Checks if cursor is NOT outside of monitor[i]'s space
        if 
        (!(
            (cursorPosition[0] < monitorPosition[0]) ||
            (cursorPosition[0] > (monitorPosition[0] + monitorVideoMode->width)) ||
            (cursorPosition[1] < monitorPosition[1]) ||
            (cursorPosition[1] > (monitorPosition[1] + monitorVideoMode->height))
        ))
        // And if so, chooses monitor i as the returned handle
        {
            success = true;
            return i;
        }
    }
    return 0;
}

// Makes the background of the window transparent
void configureTransparency()
{
    if(!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    // Ensures app window itself has no effect on aesthetics
    glfwWindowHint(GLFW_DECORATED, false);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
}

/* Sets up first pass at a window, which will then
be moved to fullscreen on a chosen monitor */
void configureWindowAPI(HelloImGui::RunnerParams* mainWindow)
{
    // Tests on primary monitor
    mainWindow->appWindowParams.windowGeometry.monitorIdx = 0;

    mainWindow->appWindowParams.windowGeometry.fullScreenMode 
        = HelloImGui::FullScreenMode::NoFullScreen;
    mainWindow->appWindowParams.windowGeometry.positionMode 
        = HelloImGui::WindowPositionMode::MonitorCenter;

    // A E S T H E T I C ?
    mainWindow->appWindowParams.borderless = true;
    mainWindow->appWindowParams.resizable = false;

    // Set no background color
    mainWindow->imGuiWindowParams.backgroundColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
}