// ORB contains GUI functions and handles visual settings
#include "spellbook.h"

#ifndef _ORB_
#define _ORB_

void setThemeTweaks(AppState appState, HelloImGui::RunnerParams* runnerParams);

void helpImTrappedInAGuiFactory(AppState& appState);

void addNiceFonts(AppState* appState);

ImVec2 getTextSize(std::string text);

void centerCursor(GLFWwindow* appWindow);

#endif // _ORB_