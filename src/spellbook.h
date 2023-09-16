// Spellbook contains any data structures and includes that need to be broadly accessible
#ifndef _EXTERN_
#define _EXTERN_

#include "hello_imgui/hello_imgui.h"
#include <GLFW/glfw3.h>

#endif

#ifndef _SPELLBOOK_
#define _SPELLBOOK_

// Data
struct AppSettings
{
    std::string name = "Default";
    std::string aesthetic = "Inky Depths";
    int monitorSelection = 0;
    float transparency = 0.7f;

};

struct AppState
{
    int mode  = 0;
    AppSettings appSettings;
};
// END Data

#endif // _SPELLBOOK_