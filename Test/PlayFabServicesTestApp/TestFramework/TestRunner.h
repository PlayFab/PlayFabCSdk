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
    void AddLog(HCTraceLevel level, _In_z_ _Printf_format_string_ const char* format, ...);
    static void CALLBACK PFHCTraceCallback(_In_z_ const char* areaName, _In_ HCTraceLevel level, _In_ uint64_t threadId, _In_ uint64_t timestamp, _In_z_ const char* message);
    void ProcessRetries();
    void SetretryableHRs(Set<String> retryableHRs);

private:
    std::mutex m_mutex;

    TestReport m_testReport{ "PlayFabServicesTestReport" };

    List<SharedPtr<TestClass>> m_testClasses;
    List<SharedPtr<TestClass>>::iterator m_activeTestClass;

    enum class TestClassState
    {
        Pending,
        Initializing,
        Active,
        Uninitializing,
        Complete
    } m_activeTestClassState{ TestClassState::Pending };

    List<SharedPtr<TestContext>> m_activeTestClassTests;
    List<SharedPtr<TestContext>>::iterator m_activeTest;

    Set<String> m_testList;
    Set<String> m_testRetryList;
    Set<HRESULT> m_retryableHRs;
	bool m_allowRetries{ true };

    static std::atomic<TestRunner*> s_hcTraceCallbackContext;
};

} // namespace Test
} // namespace PlayFab
