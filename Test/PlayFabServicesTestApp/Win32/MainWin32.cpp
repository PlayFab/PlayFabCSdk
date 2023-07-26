// Copyright (C) Microsoft Corporation. All rights reserved.

#include "TestAppPch.h"
#include "TestRunner.h"

// Win32 Entry Point
int main()
{
    PlayFab::Test::TestRunner testRunner;
    THROW_IF_FAILED(testRunner.Initialize());

    while (!testRunner.Update())
    {
        Sleep(10);
    }

    bool allTestsPassed = testRunner.Cleanup();

    // Return 0 (success) if all tests passed. Otherwise, return 1 (error).
    return allTestsPassed ? 0 : 1;
}
