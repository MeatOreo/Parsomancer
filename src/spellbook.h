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

// DEFAULT TEXT IN CASE OF ERROR
#define LORUM_IPSUM "On the other hand, \
we denounce with righteous indignation and dislike \
men who are so beguiled and demoralized by the \
charms of pleasure of the moment, so blinded \
by desire, that they cannot foresee the pain \
and trouble that are bound to ensue; and equal \
blame belongs to those who fail in their duty \
through weakness of will, which is the same as \
saying through shrinking from toil and pain. \
These cases are perfectly simple and easy to \
distinguish. In a free hour, when our power of \
choice is untrammelled and when nothing prevents \
our being able to do what we like best, every \
pleasure is to be welcomed and every pain avoided. \
But in certain circumstances and owing to the \
claims of duty or the obligations of business \
it will frequently occur that pleasures have \
to be repudiated and annoyances accepted. \
The wise man therefore always holds in these \
matters to this principle of selection: \
he rejects pleasures to secure other greater \
pleasures, or else he endures pains to avoid \
worse pains."
// END DEFAULT TEXT

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

    std::string readingFontChoice = "fonts\\OpenDyslexic-Regular.ttf";
    float readingFontSize = 86.f;

    std::string guiFontChoice = "fonts\\Lato-Black.ttf";
    float guiFontSize = 64.f;

    float wordsPerMinute = 350.f;

};

struct AppState
{
    AppSettings appSettings;

    ImFont* readingFontActive;

    // String of Strings
    std::vector<std::string> xerxes;
    // String of the World
    std::string novella = LORUM_IPSUM;

    // Save what time the last word was displayed
    std::chrono::time_point<std::chrono::steady_clock> timeOfLastWord;

    // Track what the app is supposed to be doing
    enum struct task
    {
        WAITING,
        READING
    } currentTask = task::WAITING;

    // Tracks how many words or phrases have been displayed
    size_t xerxesIndex = 0;
};
// END Data

#endif // _SPELLBOOK_

