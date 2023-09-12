#include "hello_imgui/hello_imgui.h"

#include "parse.h"


int main(int , char *[])
{
    
    int monitorChoice = getActiveMonitor(); // EXTREMELY TEMPORARY, READ FROM SETTINGS FILE INSTEAD

    HelloImGui::RunnerParams testWindow;

    testWindow.appWindowParams.windowGeometry.fullScreenMode = HelloImGui::FullScreenMode::FullScreenDesktopResolution;
    testWindow.appWindowParams.windowGeometry.positionMode = HelloImGui::WindowPositionMode::MonitorCenter;
    testWindow.appWindowParams.windowGeometry.monitorIdx = monitorChoice;

    HelloImGui::Run(testWindow);

    return 0;
}

int getActiveMonitor()
{

    return 0; // DEFAULT CASE
}