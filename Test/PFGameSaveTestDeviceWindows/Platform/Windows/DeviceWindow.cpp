#include "pch.h"
#include "Common/DeviceGameSaveState.h"
#include "DeviceWindow.h"
#include "Common/DeviceLogging.h"

LRESULT CALLBACK DeviceWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        DeviceLoggingInitializePlatform(hwnd);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        DeviceLoggingPaintPlatform(hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        DeviceLoggingHandleDestroy();
        GetSampleGameSaveState()->quit = true;
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            GetSampleGameSaveState()->quit = true;
            PostQuitMessage(0);
            return 0;
        }
        else if (wParam == 'Q' || wParam == 'q')
        {
            DeviceGameSaveState* state = GetSampleGameSaveState();
            if (state->waitingForUserInput)
            {
                // Pass Q to the interactive handler instead of quitting
                state->lastUserInputKey = static_cast<int>(wParam);
                state->userInputReceived = true;
            }
            else
            {
                state->quit = true;
                PostQuitMessage(0);
            }
            return 0;
        }
        else if (wParam == 'O')
        {
            DeviceGameSaveState* state = GetSampleGameSaveState();
            ShellExecuteA(nullptr, nullptr, "explorer.exe", state->saveFolder.c_str(), nullptr, SW_SHOWNORMAL);
        }
        else if (wParam == VK_SPACE || wParam == VK_RETURN)
        {
            DeviceGameSaveState* state = GetSampleGameSaveState();
            if (state->waitingForUserInput)
            {
                state->lastUserInputKey = static_cast<int>(wParam);
                state->userInputReceived = true;
            }
        }
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
