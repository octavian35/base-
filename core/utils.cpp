#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <string>
#include "utils.h"
#include "../offsets.h"

void utils::Attach_Console()
{
    if (!AttachConsole(ATTACH_PARENT_PROCESS))
        AllocConsole(); // fallback if no parent console

    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
}

void utils::Detach_Console()
{
    HWND consoleWindow = GetConsoleWindow();
    FreeConsole();
    if (consoleWindow != NULL) {
        PostMessage(consoleWindow, WM_CLOSE, 0, 0);
    }
}
