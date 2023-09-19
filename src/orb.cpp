#include "spellbook.h"
// ORB contains GUI functions and handles visual settings
#include "orb.h"
#include <string>

ImFont* font1;

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

    // Just in case
    mainWindow->imGuiWindowParams.showMenuBar = false;
    mainWindow->imGuiWindowParams.showMenu_App = false;
    mainWindow->imGuiWindowParams.showMenu_View = false;
}

// Primary GUI function
void helpImTrappedInAGuiFactory(AppState appState) 
{

    // Lorum
    std::string novel = appState.xerxes.at(1);

    // Where are we
    auto windowSize = ImGui::GetWindowSize();

    // TEMP
    ImGui::PushFont(appState.readingFontActive);

    auto textWidth   = getTextSize(novel).x;

    ImGui::SetCursorPosX((windowSize.x - textWidth) * 0.5f);
    ImGui::SetCursorPosY(windowSize.y*0.5f);
    ImGui::Text(novel.c_str());

    ImGui::PopFont();

    std::string start = "PARSE";
    textWidth = getTextSize(start).x;
    ImGui::SetCursorPosX((windowSize.x - textWidth)*0.5f);
    if (ImGui::Button(start.c_str()))
        HelloImGui::GetRunnerParams()->appShallExit = true;

    // END TEMP

}

void addNiceFonts(AppState* appState)
{
    std::string fontFilename = "fonts\\Lato-Light.ttf";
    if (HelloImGui::AssetExists(fontFilename))
    {
        float fontSize = 38.f;
        ImFont* font = HelloImGui::
        LoadFontTTF_WithFontAwesomeIcons(fontFilename, fontSize, false);
    }
    appState->readingFontActive = HelloImGui::LoadFontTTF
        (
        appState->appSettings.readingFontChoice, 
        appState->appSettings.readingFontSize, 
        false
        );
}

// Convenience
ImVec2 getTextSize(std::string text)
{
    return ImGui::CalcTextSize(text.c_str());
}
