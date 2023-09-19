#include "spellbook.h"
// ORB contains GUI functions and handles visual settings
#include "orb.h"
#include <string>

void drawCenteredText(std::string words);

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

// Primary GUI function - Pretty sure this is going to become a statesync nightmare :)
void helpImTrappedInAGuiFactory(AppState* appState) 
{

    // Lorum
    std::string novel = appState->xerxes[2];

    // Where are we
    auto windowSize = ImGui::GetWindowSize();

    
    // Draw start button
    if(appState->currentTask == AppState::task::WAITING)
    {
        // TEMP
        std::string start = "PARSE";

        float textWidth = getTextSize(start).x, textHeight = getTextSize(start).y;
        textWidth = getTextSize(start).x;
        ImGui::SetCursorPosX((windowSize.x - textWidth) * 0.5f);
        ImGui::SetCursorPosY((windowSize.y - textHeight) * 0.5f);

        if (ImGui::Button(start.c_str()))
        {
            ImGui::PushFont(appState->readingFontActive);
            drawCenteredText(novel);
            ImGui::PopFont();
        }
    }



}

void drawCenteredText(std::string words)
{
    // Where are we
    auto windowSize = ImGui::GetWindowSize();
    float textWidth = getTextSize(words).x, textHeight = getTextSize(words).y;

    ImGui::SetCursorPosX((windowSize.x - textWidth) * 0.5f);
    // This is a bit strange...
    ImGui::SetCursorPosY((windowSize.y - textHeight) * 0.5f);
    ImGui::Text(words.c_str());
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
