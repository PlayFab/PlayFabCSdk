// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#include "pch.h"
#include "main.h"
#include "actions.h"
#include "MemoryManager.h"

GameState g_gameState;

class FooEnvironment : public ::testing::Environment
{
public:
    ~FooEnvironment() override {}

    void SetUp() override
    {
        HCTraceSetTraceToDebugger(false);
        g_gameState.customPlayerId = "cloudsave-player-automated1";
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
        std::string saveFolder;
        char path[MAX_PATH];
        if (GetModuleFileNameA(NULL, path, MAX_PATH) != 0)
        {
            std::string processPath(path);
            size_t lastBackslashPos = processPath.find_last_of("\\");
            if (lastBackslashPos != std::string::npos)
            {
                processPath = processPath.substr(0, lastBackslashPos);
                processPath += "\\GameSaveTestData\\";
                saveFolder = processPath;
            }
        }
        g_gameState.localRoot = saveFolder.c_str();
#endif

        debug_set_data_folder(FilePALTestApp::AppendPath(g_gameState.localRoot, "MockPFEntityFiles"));
    }

    void TearDown() override
    {
    }
};

int main(int argc, char** argv)
{
    MemoryManager::Instance().SetHooks();
    //while (true)
    {
        testing::InitGoogleTest(&argc, argv);
        testing::AddGlobalTestEnvironment(new FooEnvironment);
        RUN_ALL_TESTS();
    }
}
