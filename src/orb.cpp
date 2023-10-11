#include "spellbook.h"
// ORB contains GUI functions and handles visual settings
#include "orb.h"
#include <string>

void drawCenteredText(std::string words);
void calcRampUp(AppState& appState);
void drawStartButton(AppState& appState);


// PRIMARY GUI FUNCTION
void helpImTrappedInAGuiFactory(AppState& appState) 
{
    if(appState.currentTask == AppState::task::LAUNCHING)
    {   

        // ENDS LAUNCH SECTION
        appState.currentTask = AppState::task::WAITING;
    }
    // Draw start button
    else if(appState.currentTask == AppState::task::WAITING)
    {
        HelloImGui::GetRunnerParams()->fpsIdling.enableIdling = true;


        drawStartButton(appState);
    }
    // If reading mode is engaged, we need to:
    // Find out what string we have to read
    // Find out if it's been long enough to justify displaying it yet
    // Display it or keep displaying the old one
    else if(appState.currentTask == AppState::task::READING)
    {

        // GOTTA GO FAST
        HelloImGui::GetRunnerParams()->fpsIdling.enableIdling = false;

        // Calculating this here lets us dynamically change reading speed
        const std::chrono::milliseconds wordDuration((int)(1000 * 
            (60.f / appState.currentWordsPerMinute)));

        // Still not sure why the -1 is needed but it works
        if (appState.xerxesIndex < (appState.xerxes.size() - 1))
        {
            auto msSinceLastWord = std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::steady_clock::now() - appState.timeOfLastWord);

            if(msSinceLastWord >= wordDuration)
            {
                appState.timeOfLastWord = std::chrono::steady_clock::now();

                // Increment our progress through String of Strings
                appState.xerxesIndex++;

                calcRampUp(appState);
            }
            else
            {
                // Do nothing i suppose
            }
            // Draw wherever we are in Xerxes, String of Strings, every single frame
            ImGui::PushFont(appState.readingFontActive);
            drawCenteredText(appState.xerxes[appState.xerxesIndex]);
            ImGui::PopFont();
        }
        else 
        {
            // TEMP AS HECK
            HelloImGui::GetRunnerParams()->appShallExit = true;
        }
    }
}
// END PRIMARY GUI FUNCTION


// CONVENIENCE FUNCTIONS
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

ImVec2 getTextSize(std::string text)
{
    return ImGui::CalcTextSize(text.c_str());
}
// END CONVENIENCE FUNCTIONS


// AESTHETIC FUNCTIONS
// Changes main window theme to whatever it reads from settings
void setThemeTweaks(AppState appState, HelloImGui::RunnerParams* mainWindow)
{
    ImGuiTheme::ImGuiTweakedTheme tweakedTheme;
    if(appState.appSettings.aesthetic == "Inky Depths")
        {
            tweakedTheme.Theme = ImGuiTheme::ImGuiTheme_BlackIsBlack;
        }
    tweakedTheme.Tweaks.AlphaMultiplier = appState.appSettings.transparency;
    tweakedTheme.Tweaks.Rounding = 40.f;
    tweakedTheme.Tweaks.ValueMultiplierFront = 2.f;
    

    mainWindow->imGuiWindowParams.tweakedTheme = tweakedTheme;

    // Just in case
    mainWindow->imGuiWindowParams.showMenuBar = false;
    mainWindow->imGuiWindowParams.showMenu_App = false;
    mainWindow->imGuiWindowParams.showMenu_View = false;
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
// END AESTHETIC FUNCTIONS


// CONTROL FUNCTIONS
// Handles curving WPM towards dynamically set value
void calcRampUp(AppState& appState)
{
    if (appState.appSettings.rampUp == true)
    {
        appState.currentWordsPerMinute = 
        (appState.currentWordsPerMinute + appState.appSettings.wordsPerMinute) * 0.5f;
    }
    else
    {
        appState.currentWordsPerMinute = appState.appSettings.wordsPerMinute;
    }
}

void drawStartButton(AppState& appState)
{   
    // TEMP choose from list of fun start messages instead
    std::string start = "START READING";

    // Where are we
    ImVec2 windowSize = ImGui::GetWindowSize();
    // Pad the button around its label
    ImVec2 buttonTextSize = getTextSize(start);
    ImVec2 buttonSize = {buttonTextSize.x + 60.f, buttonTextSize.y + 20.f};

    // Place button in center of screen
    ImVec2 buttonPosition = {(windowSize.x - buttonSize.x) * 0.5f, 
        (windowSize.y) * 0.5f};
    ImGui::SetCursorPos(buttonPosition);

    if (ImGui::Button(start.c_str(), buttonSize) 
        && appState.currentTask == AppState::task::WAITING)
    {
        appState.currentTask = AppState::task::READING;
        appState.timeOfLastWord = std::chrono::steady_clock::now();

        // Not strictly neccesary
        appState.xerxesIndex = 0;
    }
}

void centerCursor(GLFWwindow* appWindow)
{       
    int windowX, windowY;
    // Where are we
    glfwGetWindowSize(appWindow, &windowX, &windowY);
    // Move cursor to middle of screen for instant start clicking
    // NOT WORKING???
    // glfwSetInputMode((GLFWwindow*) HelloImGui::GetRunnerParams()->
    //     backendPointers.glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPos(appWindow, windowX * 0.5, windowY * 0.52);

    // glfwSetInputMode((GLFWwindow*) HelloImGui::GetRunnerParams()->
    //     backendPointers.glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}
// END CONTROL FUNCTIONS