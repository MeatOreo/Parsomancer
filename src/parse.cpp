// PARSE contains functions related to deciding which words to put on screen
#include "spellbook.h"
#include <string>
#include "parse.h"


void readSelection(AppState& appState)
{
    char separator = ' ';

    // Custom split function
    int startIndex = 0, endIndex = 0;
    for(int i = 0; i <= appState.novella.size(); i++)
    {
        if(appState.novella[i] == separator || i == appState.novella.size()) 
        {
            endIndex = i;
            std::string temp;
            temp.append(appState.novella, startIndex, endIndex - startIndex);
            appState.xerxes.push_back(temp);
            startIndex = endIndex + 1;
        }
    }
} 