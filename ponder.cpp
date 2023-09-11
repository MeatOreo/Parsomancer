#include "hello_imgui/hello_imgui.h"

int main(int , char *[])
{
    HelloImGui::RunnerParams testWindow;

    testWindow.appWindowParams.windowGeometry.fullScreenMode = HelloImGui::FullScreenMode::FullMonitorWorkArea;
    testWindow.appWindowParams.windowGeometry.positionMode = HelloImGui::WindowPositionMode::MonitorCenter;

    HelloImGui::Run(testWindow);

    return 0;
}