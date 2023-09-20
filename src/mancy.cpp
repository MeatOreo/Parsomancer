// MANCY handles the dirty work of reading text out of the clipboard

#include <windows.h>
#include "spellbook.h"
#include "mancy.h"

void getClippyText(AppState& appState)
{
    //if (! OpenClipboard(nullptr))
        // error

    HANDLE hData = GetClipboardData(CF_TEXT);

    //if(hData == nullptr)
        // error

    char* pszText = static_cast<char*>(GlobalLock(hData));
    //if(pszText == nullptr)
        // error

    appState.novella.assign(pszText);

    GlobalUnlock(hData);

    CloseClipboard();
}