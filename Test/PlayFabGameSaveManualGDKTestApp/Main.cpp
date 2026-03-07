// Copyright (C) Microsoft Corporation. All rights reserved.
#include "pch.h"

#ifdef __clang__
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif

#pragma warning(disable : 4061)

// Text log variables
constexpr int MAX_LOG_LINES = 50;
std::vector<std::string> g_logLines;
std::mutex g_logLinesMutex; // Add mutex for thread safety
HFONT g_hFont = NULL;
COLORREF g_textColor = RGB(255, 255, 255); // White text
COLORREF g_bgColor = RGB(0, 0, 128);       // Blue background

Sample_GameSaveState m_sampleGameSaveState;
Sample_GameSaveState* GetSampleGameSaveState() 
{ 
    return &m_sampleGameSaveState; 
}

// Function to log text to the window
void LogToWindow(const std::string& text)
{
    std::lock_guard<std::mutex> lock(g_logLinesMutex); // Thread-safe access
    
    // Add the new line to log
    g_logLines.push_back(text);
    
    // Keep only the last MAX_LOG_LINES - more efficient approach
    if (g_logLines.size() > MAX_LOG_LINES)
    {
        // Remove elements from the front more efficiently
        size_t elementsToRemove = g_logLines.size() - MAX_LOG_LINES;
        g_logLines.erase(g_logLines.begin(), g_logLines.begin() + elementsToRemove);
    }
    
    // Force a repaint of the window
    if (GetSampleGameSaveState() && !GetSampleGameSaveState()->quit)
    {
        HWND hwnd = FindWindow(L"PlayFabGameSaveWindowClass", NULL);
        if (hwnd)
        {
            InvalidateRect(hwnd, NULL, TRUE);
        }
    }
}

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        // Create the font for the window
        g_hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, L"Consolas");
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        
        // Set the background color to blue
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        HBRUSH bgBrush = CreateSolidBrush(g_bgColor);
        FillRect(hdc, &clientRect, bgBrush);
        DeleteObject(bgBrush);
        
        // Set up for text drawing
        SetBkColor(hdc, g_bgColor);
        SetTextColor(hdc, g_textColor);
        SelectObject(hdc, g_hFont);

        // Draw the log lines
        int yPos = 10;
        {
            std::lock_guard<std::mutex> lock(g_logLinesMutex); // Thread-safe access
            for (const auto& line : g_logLines)
            {
                TextOutA(hdc, 10, yPos, line.c_str(), static_cast<int>(line.length()));
                yPos += 20; // Line spacing
            }
        }
        
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        if (g_hFont)
        {
            DeleteObject(g_hFont);
            g_hFont = NULL;
        }
        GetSampleGameSaveState()->quit = true;
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE || wParam == 'Q')
        {
            GetSampleGameSaveState()->quit = true;
            PostQuitMessage(0);
            return 0;
        }
        else if (wParam == 'O')
        {
            Sample_GameSaveState* state = GetSampleGameSaveState();
            ShellExecuteA(nullptr, nullptr, "explorer.exe", state->saveFolder.c_str(), nullptr, SW_SHOWNORMAL);
        }
        else if (wParam == 'U')
        {
        //    LogToWindow("Uploading");
        //    WriteStatusFile(state, "Preupload");
        //    WriteStatusFile(state, "Uploading");
        //    hr = Sample_GameSave_Upload(state->localUserHandle);
        //    RETURN_IF_FAILED(hr);
        //    WriteStatusFile(state, "Uploaded");
        }
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Entry point
int WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(nCmdShow);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_LEAK_CHECK_DF);
    HRESULT hr;

    // Initialize logging
    LogToWindow("PlayFab Game Save Test starting...");

    hr = XGameRuntimeInitialize();
    if (FAILED(hr))
    {
        if (hr == E_GAMERUNTIME_DLL_NOT_FOUND || hr == E_GAMERUNTIME_VERSION_MISMATCH)
        {
            std::string errorMsg = "Game Runtime is not installed on this system";
            LogToWindow(errorMsg);
            return 1;
        }
    }

    bool success = Sample_GameSave_ParseArgs(lpCmdLine, GetSampleGameSaveState());
    if (!success)
    {
        return 1;
    }

    hr = XTaskQueueCreate(XTaskQueueDispatchMode::ThreadPool, XTaskQueueDispatchMode::ThreadPool, &GetSampleGameSaveState()->taskQueue);
    if (FAILED(hr))
    {
        LogToWindow("Failed to create task queue");
        return 1;
    }
    XTaskQueueSetCurrentProcessTaskQueue(GetSampleGameSaveState()->taskQueue);

    // Register window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"PlayFabGameSaveWindowClass";
    RegisterClassEx(&wc);

    // Create window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        L"PlayFabGameSaveWindowClass",  // Window class
        L"PlayFab Game Save Test",      // Window text
        WS_OVERLAPPEDWINDOW,            // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr,                        // Parent window    
        nullptr,                        // Menu
        hInstance,                      // Instance handle
        nullptr                         // Additional application data
    );

    if (hwnd == nullptr)
    {
        LogToWindow("Failed to create window");
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg = {};
    while (!GetSampleGameSaveState()->quit)
    {
        // Process any messages in the queue
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            
            if (msg.message == WM_QUIT)
            {
                GetSampleGameSaveState()->quit = true;
                break;
            }
        }
        
        // Run the game update
        if (!GetSampleGameSaveState()->quit)
        {
            hr = Sample_GameSave_Update(&m_sampleGameSaveState);
            if (FAILED(hr))
            {
                return 1;
            }
        }
    }

    Sample_GameSave_Cleanup(GetSampleGameSaveState());
   
    XTaskQueueSetCurrentProcessTaskQueue(nullptr);
    Sleep(2000); // Simple wait for any pending tasks to complete
    XGameRuntimeUninitialize();

    return static_cast<int>(0);
}

