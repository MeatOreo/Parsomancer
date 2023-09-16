#include "spellbook.h"
// ORB contains GUI functions and handles visual settings
#include "orb.h"

// Changes main window theme to whatever it reads from settings
void setThemeTweaks(AppState appState, HelloImGui::RunnerParams* mainWindow)
{
    ImGuiTheme::ImGuiTweakedTheme tweakedTheme;
    if(appState.appSettings.aesthetic == "Inky Depths")
        {
            tweakedTheme.Theme = ImGuiTheme::ImGuiTheme_BlackIsBlack;
        }
    tweakedTheme.Tweaks.AlphaMultiplier = appState.appSettings.transparency;

    mainWindow->imGuiWindowParams.tweakedTheme = tweakedTheme;

    mainWindow->imGuiWindowParams.showMenuBar = true;
    mainWindow->imGuiWindowParams.showMenu_View;
}

