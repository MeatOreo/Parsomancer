// Spellbook contains any data structures and includes that need to be broadly accessible
#ifndef _EXTERN_
#define _EXTERN_

#include "hello_imgui/hello_imgui.h"
#include <GLFW/glfw3.h>

#endif

#ifndef _SPELLBOOK_
#define _SPELLBOOK_

#include <vector>
#include <chrono>

// Data
struct AppSettings
{
    // TEMP tracks what kind of window mode the user wants
    std::string name = "Default";

    // TEMP
    std::string aesthetic = "Inky Depths";

    // Monitor index the user wants the app to display on
    int monitorSelection = 0;

    // Main window background alpha
    float transparency = 0.7f;

    std::string readingFontChoice = "fonts\\Lato-Regular.ttf";
    float readingFontSize = 48.f;

    int wordsPerMinute = 300;

};

struct AppState
{
    // Does nothing?
    int mode  = 0;

    AppSettings appSettings;

    ImFont* readingFontActive;

    // String of Strings
    std::vector<std::string> xerxes;
    // String of the World
    std::string novella;

    // Save what time the last word was displayed
    int timeOfLastWord;

    // Track what the app is supposed to be doing
    enum struct task
    {
        WAITING,
        READING
    } currentTask = task::WAITING;

    // Tracks how many words or phrases have been displayed
    int xerxesIndex = 0;
};


// END Data

#endif // _SPELLBOOK_