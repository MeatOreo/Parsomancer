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
    std::string name = "Default";
    std::string aesthetic = "Inky Depths";
    int monitorSelection = 0;
    float transparency = 0.7f;
    std::string readingFontChoice = "fonts\\Lato-Regular.ttf";
    float readingFontSize = 48.f;

};

struct AppState
{
    int mode  = 0;
    AppSettings appSettings;
    ImFont* readingFontActive;
    std::vector<std::string> xerxes;
    std::string novella;
    int timeOfLastWord;

    // Track what the app is supposed to be doing
    enum struct task
    {
        WAITING,
        READING
    } currentTask = task::WAITING;

};


// END Data

#endif // _SPELLBOOK_