#include "hello_imgui/hello_imgui.h"
#include <windows.h>
#include <WinUser.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include "parse.h"

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


int getCursorMonitorIdx(GLFWwindow* window);
void launchSequence(AppState appState);

int main(int , char *[])
{
    
    AppState appState;
    HelloImGui::RunnerParams mainWindow;

    mainWindow.callbacks.PostInit = [&appState] {launchSequence(appState);};

    mainWindow.appWindowParams.windowGeometry.fullScreenMode = HelloImGui::FullScreenMode::NoFullScreen;
    mainWindow.appWindowParams.windowGeometry.positionMode = HelloImGui::WindowPositionMode::MonitorCenter;
    mainWindow.appWindowParams.windowGeometry.monitorIdx = 0;


    appState.appSettings.name = "cursor";

    HelloImGui::Run(mainWindow);

    return 0;
}



void launchSequence(AppState appState)
{
    

    if(appState.appSettings.name == "default")
    {
        HelloImGui::GetRunnerParams()->appWindowParams.windowGeometry.monitorIdx = 0;
    }
    else if(appState.appSettings.name == "cursor")
    {
        HelloImGui::GetRunnerParams()->appWindowParams.windowGeometry.monitorIdx = 
        getCursorMonitorIdx((GLFWwindow*) HelloImGui::GetRunnerParams()->backendPointers.glfwWindow);
    }

    HelloImGui::GetRunnerParams()->appWindowParams.windowGeometry.fullScreenMode = HelloImGui::FullScreenMode::FullScreenDesktopResolution;
}



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
        //    UNNECESSARY
        //    monitor = monitors[i];
            success = true;
            return i;
        }
    }
    return 0;
}

// DOES NOT WORK, MAY NOT BE POSSIBLE
/*
int getActiveMonitor()
{
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count); 
    
    HMONITOR activeMonitor = MonitorFromWindow(GetForegroundWindow(),MONITOR_DEFAULTTOPRIMARY);
    for (int i = 0; i < count; i++)
    {
        if (monitors[i] == activeMonitor)
        {
            return i;
        }
    }

    return 3; // This should not be possible
}
*/