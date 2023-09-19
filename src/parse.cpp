// PARSE contains functions related to deciding which words to put on screen
#include "spellbook.h"
#include <string>
#include "parse.h"


void readSelection(AppState* appState)
{
    // TEMP
    appState->novella = LORUM_IPSUM;
    std::stringstream darius(appState->novella);
    std::string word;
    while(darius >> word)
    {
        appState->xerxes.push_back(word);
    }
} 