// Copyright (C) Microsoft Corporation. All rights reserved.

#include "TestAppPch.h"
#include "playfab/services/PFServices.h"
#include "Nlohmann.h"
#include "TestRunner.h"
#include "../PlatformUtils.h"
#include "MainApple.h"

void WriteLogToFile(const char* line, const char* strFileName)
{
    char buffer[256];

    //HOME is the home directory of your application
    //points to the root of your sandbox
    strcpy(buffer,getenv("HOME"));

    //concatenating the path string returned from HOME
    strcat(buffer,"/Documents/");
    strcat(buffer,strFileName);

   FILE *file = fopen(buffer, "a"); // "a" for append mode

    if (file == NULL) {
        std::cout << "Error opening file";
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s\n", line);
    fclose(file);
}

int MainApple()
{
    PlayFab::Test::TestRunner testRunner;
    THROW_IF_FAILED(testRunner.Initialize());

    while (!testRunner.Update())
    {
        PlayFab::Test::Platform::Sleep(10);
    }

    bool allTestsPassed = testRunner.Cleanup();

    // Logging result in a separate file inside the simulator to make it easier for the pipeline script to assess if the test app run was successful or not
    std::string resultStr = allTestsPassed ? "PASS" : "FAIL";

#if HC_PLATFORM == HC_PLATFORM_IOS
    WriteLogToFile(resultStr.c_str(), "PFTestAppResult.txt");
#endif

    // Return 0 (success) if all tests passed. Otherwise, return 1 (error).
    return allTestsPassed ? 0 : 1;
}
