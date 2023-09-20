// MANCY handles the dirty work of reading text out of the clipboard

#include <windows.h>
#include "spellbook.h"
#include "mancy.h"

void getClippyText(AppState& appState)
{
    if (! OpenClipboard(nullptr))
    {
        exit(9);
    }

    HANDLE hData = GetClipboardData(CF_TEXT);

    if(hData == nullptr)
    {
        exit(8);
    }


    char* pszText = static_cast<char*>(GlobalLock(hData));
    if(pszText == nullptr)
    {
        exit(3);
    }

    appState.novella = pszText;

    GlobalUnlock(hData);

    CloseClipboard();
}