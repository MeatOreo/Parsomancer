#include "spellbook.h"
// ORB contains GUI functions and handles visual settings
#include "orb.h"
#include <string>

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
    std::string novel = "Check out this T E X T tho";

    // Where are we
    auto windowSize = ImGui::GetWindowSize();

    // TEMP
    auto textWidth   = getTextSize(novel).x;

    ImGui::SetCursorPosX((windowSize.x - textWidth) * 0.5f);
    ImGui::SetCursorPosY(windowSize.y*0.5f);
    ImGui::Text(novel.c_str());

    //ImGui::PushFont(appState.readingFontActive);
    std::string start = "PARSE";
    textWidth = getTextSize(start).x;
    ImGui::SetCursorPosX((windowSize.x - textWidth)*0.5f);
    if (ImGui::Button(start.c_str()))
        HelloImGui::GetRunnerParams()->appShallExit = true;
    //ImGui::PopFont();

    // END TEMP

}

void addNiceFonts(AppState appState)
{
    std::string fontFilename = "fonts\\Lato-Regular.ttf";
    if (HelloImGui::AssetExists(fontFilename))
    {
        float fontSize = 38.f;
        ImFont* font = HelloImGui::
        LoadFontTTF_WithFontAwesomeIcons(fontFilename, fontSize, false);
        //appState.readingFontActive = HelloImGui::
        //LoadFontTTF_WithFontAwesomeIcons(appState.appSettings.readingFontChoice, 12.f, false);
        // (void)font; // wHY???
    }
}

// Convenience
ImVec2 getTextSize(std::string text)
{
    return ImGui::CalcTextSize(text.c_str());
}
