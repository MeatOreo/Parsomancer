#include "spellbook.h"
#include "ponder.h"
#include "orb.h"

#define USER_QUIT 0

#include "parse.h"
#include "mancy.h"

// CONFIG FUNCTIONS
void configureTransparency();
void configureWindowAPI(HelloImGui::RunnerParams* mainWindow);
void launchSequence(AppState appState);
// END CONFIG FUNCTIONS

// CALLBACK FUNCTIONS
void exitCallback(GLFWwindow* appWindow, int key, int scancode, int action, int mods);
// END CALLBACK FUNCTIONS


int main(int , char *[])
{
    // INITIALIZE CORE DATA
    struct AppState appState;
    HelloImGui::RunnerParams mainWindow;
    // END CORE DATA

    // PREPARE READING MATERIALS
    getClippyText(appState);
    readSelection(appState);
    // END READING MATERIALS

    // CALL CONFIG FUNCTIONS
    configureTransparency();
    configureWindowAPI(&mainWindow);
    setThemeTweaks(appState, &mainWindow);
    // END CONFIG FUNCTIONS

    // Hello_ImGui CALLBACKS
    mainWindow.callbacks.PostInit = [&appState] {launchSequence(appState);};
    mainWindow.callbacks.LoadAdditionalFonts = [&appState] {addNiceFonts(&appState);};
    mainWindow.callbacks.ShowGui = [&appState] {helpImTrappedInAGuiFactory(&appState);};
    // END Hello_ImGui CALLBACKS

    // TEMP
    appState.appSettings.name = "cursor";
    // TEMP

    // THE REAL DEAL
    HelloImGui::Run(mainWindow);

    // HERE BE DRAGONS
    return 0;
}






// Prepares for the existence of the glfw window
void launchSequence(AppState appState)
{
    moveAppToMonitor(appState);

    glfwSetKeyCallback((GLFWwindow*) HelloImGui::GetRunnerParams()->backendPointers.glfwWindow, exitCallback);

}

void exitCallback(GLFWwindow* appWindow, int key, int scancode, int action, int mods)
{
    if(!(key == GLFW_KEY_SPACE) && action == GLFW_PRESS)
    {
        HelloImGui::GetRunnerParams()->appShallExit = true;
    }
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

    int monitorCoords[2] = {0,0};
    float monitorScale[2] = {0.f, 0.f};
    // Get resolution etc for selected monitor
    const GLFWvidmode* mode = glfwGetVideoMode(monitorArray[selectedMonitor]);

    // Find upper left corner of chosen monitor
    glfwGetMonitorPos(monitorArray[selectedMonitor], &monitorCoords[0], &monitorCoords[1]);

    // glfwGetMonitorWorkarea(monitorArray[selectedMonitor], &monitorCoords[0], &monitorCoords[1], nullptr, nullptr);

    // Determine monitor scaling to set window size proportionately
    glfwGetMonitorContentScale(monitorArray[selectedMonitor], &monitorScale[0], &monitorScale[1]);

    // Move app window to upper left corner of selected monitor and set scale
    // glfwSetWindowMonitor(appWindow, nullptr, monitorCoords[0], monitorCoords[1], 
    //     (int)(mode->width / monitorScale[0]), (int)(300), mode ->refreshRate);

    glfwSetWindowPos(appWindow, monitorCoords[0], monitorCoords[1]);
    glfwSetWindowSize(appWindow, (int)(mode->width / monitorScale[0]), 
        (int)(mode->height/ monitorScale[1]));
    
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
    glfwWindowHint(GLFW_FLOATING, true);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, false);
}

/* Sets up first pass at a window, which will then
be moved to fullscreen on a chosen monitor */
void configureWindowAPI(HelloImGui::RunnerParams* mainWindow)
{
    mainWindow->appWindowParams.windowGeometry.position = {0,0};

    mainWindow->appWindowParams.windowGeometry.fullScreenMode 
        = HelloImGui::FullScreenMode::NoFullScreen;
    mainWindow->appWindowParams.windowGeometry.positionMode 
        = HelloImGui::WindowPositionMode::FromCoords;

    // A E S T H E T I C ?
    mainWindow->appWindowParams.borderless = true;
    mainWindow->appWindowParams.resizable = false;

    // Set no background color
    mainWindow->imGuiWindowParams.backgroundColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

    // In case we want to reduce CPU usage, UNTESTED
    mainWindow->fpsIdling.fpsIdle = 5;
    // Start FAST
    mainWindow->fpsIdling.enableIdling = false;
}