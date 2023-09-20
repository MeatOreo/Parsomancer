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
    // Where are we
    auto windowSize = ImGui::GetWindowSize();
    
    // Draw start button
    if(appState->currentTask == AppState::task::WAITING)
    {
        HelloImGui::GetRunnerParams()->fpsIdling.enableIdling = true;

        // TEMP
        std::string start = "PARSE";

        float textWidth = getTextSize(start).x, textHeight = getTextSize(start).y;
        textWidth = getTextSize(start).x;
        ImGui::SetCursorPosX((windowSize.x - textWidth) * 0.5f);
        ImGui::SetCursorPosY((windowSize.y) * 0.5f);

        if (ImGui::Button(start.c_str()))
        {
            appState->currentTask = AppState::task::READING;
            appState->timeOfLastWord = std::chrono::steady_clock::now();
        }
    }
    // If reading mode is engaged, we need to:
    // Find out what string we have to read
    // Find out if it's been long enough to justify displaying it yet
    // Display it or keep displaying the old one
    else if(appState->currentTask == AppState::task::READING)
    {

        // Probably don't need to be calculating this every single frame...
        // Though it does let us adjust wpm dynamically
        const std::chrono::milliseconds wordDuration((int)(1000 * (60.f / appState->appSettings.wordsPerMinute)));

        // Boy does this ever work
        HelloImGui::GetRunnerParams()->fpsIdling.enableIdling = false;

        if (!(appState->xerxesIndex >= appState->xerxes.size()))
        {
            auto msSinceLastWord = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - appState->timeOfLastWord);
            if(msSinceLastWord >= wordDuration)
            {
                // Update timestamp to match the freshly-displayed word
                appState->timeOfLastWord = std::chrono::steady_clock::now();

                // Increment our progress through String of Strings
                appState->xerxesIndex++;
            }
            else
            {
                
            }
            // Draw wherever we are in Xerxes, String of Strings, every single frame
            ImGui::PushFont(appState->readingFontActive);
            drawCenteredText(appState->xerxes[appState->xerxesIndex]);
            ImGui::PopFont();
        }
        else
        {
            // TEMP AS HECK
            HelloImGui::GetRunnerParams()->appShallExit = true;
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
    ImGui::SetCursorPosY((windowSize.y) * 0.5f);
    ImGui::Text(words.c_str());
}

void addNiceFonts(AppState* appState)
{
    // TEMP
    std::string fontFilename = "fonts\\Lato-Black.ttf";
    // Check if default font can be found, load it
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
