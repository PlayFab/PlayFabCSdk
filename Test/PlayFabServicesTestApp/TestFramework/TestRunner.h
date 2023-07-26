// Copyright (C) Microsoft Corporation. All rights reserved.

#pragma once

#include "TestReport.h"
#include "TestClass.h"
#include "TestTypes.h"

namespace PlayFab
{
namespace Test
{

/// <summary>
/// Class the controls execution and flow of a Suite of tests
/// </summary>
class TestRunner
{
public:
    TestRunner() = default;

    // Initialize TestRunner
    HRESULT Initialize();

    // Set a specific subset of tests to be run. Useful for debugging failing tests.
    // If this is not called, all tests will be run by default.
    void SetTestList(Set<String> testNames);

    // Tick current test. To be called from game loop.
    // Returns true if testing is complete.
    bool Update();

    // Cleanup TestRunner
    // Returns true if all tests passed
    bool Cleanup();

private:
    String GenerateTestSummary();
    void AddLog(PFHCTraceLevel level, _In_z_ _Printf_format_string_ const char* format, ...);
    static void CALLBACK PFHCTraceCallback(_In_z_ const char* areaName, _In_ PFHCTraceLevel level, _In_ uint64_t threadId, _In_ uint64_t timestamp, _In_z_ const char* message);

private:
    std::mutex m_mutex;

    bool m_bCleanup{ false };

    TestReport m_testReport{ "PlayFabServicesTestReport" };

    List<SharedPtr<TestClass>> m_testClasses;
    List<SharedPtr<TestClass>>::iterator m_activeTestClass;
    bool m_activeTestClassInitialized{ false };

    List<SharedPtr<TestContext>> m_activeTestClassTests;
    List<SharedPtr<TestContext>>::iterator m_activeTest;

    Set<String> m_testList;

    static std::atomic<TestRunner*> s_hcTraceCallbackContext;
};

} // namespace Test
} // namespace PlayFab
