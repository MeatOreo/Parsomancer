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
            tweakedTheme.Tweaks.Rounding = 40.f;
            tweakedTheme.Tweaks.ValueMultiplierFront = 2.f;
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

        // TEMP choose from list of fun start messages instead
        std::string start = "START READING";

        // Pad the button around its label
        ImVec2 buttonTextSize = getTextSize(start);
        ImVec2 buttonSize = {buttonTextSize.x + 60.f, buttonTextSize.y + 20.f};

        // Place button in center of screen
        ImVec2 buttonPosition = {(windowSize.x - buttonSize.x) * 0.5f, 
            (windowSize.y) * 0.5f};
        ImGui::SetCursorPos(buttonPosition);



        if (ImGui::Button(start.c_str(), buttonSize))
        {
            appState->currentTask = AppState::task::READING;
            appState->timeOfLastWord = std::chrono::steady_clock::now();

            // Not strictly neccesary
            appState->xerxesIndex = 0;
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

        // Why is the -1 necessary? Does this actually work?
        if (appState->xerxesIndex < (appState->xerxes.size() - 1))
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
                // Do nothing i suppose
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
    // Check if default font can be found, load it
    if (HelloImGui::AssetExists(appState->appSettings.guiFontChoice))
    {
        float fontSize = 38.f;
        ImFont* guiFont = HelloImGui::
            LoadFontTTF_WithFontAwesomeIcons(appState->appSettings.guiFontChoice, 
            appState->appSettings.guiFontSize, false);
    }
    if (HelloImGui::AssetExists(appState->appSettings.readingFontChoice))
    {
        appState->readingFontActive = HelloImGui::
            LoadFontTTF(appState->appSettings.readingFontChoice, 
            appState->appSettings.readingFontSize, false);
    }
}

// Convenience
ImVec2 getTextSize(std::string text)
{
    return ImGui::CalcTextSize(text.c_str());
}
