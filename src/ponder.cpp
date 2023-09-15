#include "ponder.h"

int main(int , char *[])
{

    if(!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_DECORATED, false);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
    
    AppState appState;
    HelloImGui::RunnerParams mainWindow;


    mainWindow.callbacks.PostInit = [&appState] {moveAppToMonitor(appState);};

    // Parameters for initial test window, which is immediately reshaped to user setting
    mainWindow.appWindowParams.windowGeometry.fullScreenMode 
        = HelloImGui::FullScreenMode::NoFullScreen;
    mainWindow.appWindowParams.windowGeometry.positionMode 
        = HelloImGui::WindowPositionMode::MonitorCenter;
    mainWindow.appWindowParams.windowGeometry.monitorIdx = 0;
    mainWindow.appWindowParams.borderless = true;
    mainWindow.appWindowParams.resizable = false;

    mainWindow.imGuiWindowParams.showMenu_View_Themes = true;
    mainWindow.imGuiWindowParams.showMenuBar = true;

    // Set initial background color from settings
    mainWindow.imGuiWindowParams.backgroundColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

    mainWindow.callbacks.ShowGui = MyGui;


    appState.appSettings.name = "cursor";




    HelloImGui::Run(mainWindow);

    return 0;
}


// TEST FUNCTION
void MyGui() {
    ImGui::Text("Hello, world");
    if (ImGui::Button("Exit"))
        HelloImGui::GetRunnerParams()->appShallExit = true;
  

}



// Moves the app window to the settings-selected monitor. Maybe should be two functions...
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

