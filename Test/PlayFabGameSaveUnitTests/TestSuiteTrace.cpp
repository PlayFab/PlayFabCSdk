// Copyright (c) Microsoft Corporation
#include "pch.h"
#include "PFGameSaveFilesForDebug.h"

namespace
{
std::atomic<uint32_t> s_traceCount{};

void CALLBACK TraceCallback(
    _In_z_ const char* areaName,
    _In_ HCTraceLevel,
    _In_ uint64_t,
    _In_ uint64_t,
    _In_z_ const char* message
)
{
    if (strcmp(areaName, "PlayFab") == 0 &&
        (strstr(message, "TraceLevelTest") != nullptr ||
            strstr(message, "PFGameSaveFilesSetForceInprocForDebug") != nullptr))
    {
        ++s_traceCount;
    }
}

void EmitTraceEvents()
{
    TRACE_ERROR("TraceLevelTest Error");
    TRACE_WARNING("TraceLevelTest Warning");
    TRACE_IMPORTANT("TraceLevelTest Important");
    TRACE_INFORMATION("TraceLevelTest Information");
    TRACE_VERBOSE("TraceLevelTest Verbose");
}
}

class TestSuiteTrace : public testing::Test
{
protected:
    void SetUp() override
    {
        s_traceCount = 0;
        ASSERT_HRESULT_SUCCEEDED(PFInitialize(nullptr));
        ASSERT_TRUE(HCTraceSetClientCallback(TraceCallback));
    }

    void TearDown() override
    {
        EXPECT_TRUE(SUCCEEDED(PFSettingsSetTraceLevel(PFTraceLevel::Verbose)));

        XAsyncBlock async{};
        EXPECT_TRUE(SUCCEEDED(PFUninitializeAsync(&async)));
        EXPECT_TRUE(SUCCEEDED(XAsyncGetStatus(&async, true)));
    }
};

// Verifies that events are filtered as expected after calling PFSettingsSetTraceLevel.
TEST_F(TestSuiteTrace, FiltersPlayFabTraceEvents)
{
    struct TestCase
    {
        PFTraceLevel level;
        uint32_t expectedCount;
    };

    TestCase testCases[]
    {
        { PFTraceLevel::Off, 0 },
        { PFTraceLevel::Error, 1 },
        { PFTraceLevel::Warning, 2 },
        { PFTraceLevel::Important, 3 },
        { PFTraceLevel::Information, 4 },
        { PFTraceLevel::Verbose, 5 },
    };

    for (auto const& testCase : testCases)
    {
        s_traceCount = 0;
        ASSERT_HRESULT_SUCCEEDED(PFSettingsSetTraceLevel(testCase.level));
        EmitTraceEvents();
        EXPECT_EQ(testCase.expectedCount, s_traceCount.load());
    }
}

// Verifies that PFSettingsSetTraceLevel returns E_INVALIDARG and doesn't change the level if the arg is invalid.
TEST_F(TestSuiteTrace, RejectsInvalidTraceLevel)
{
    ASSERT_HRESULT_SUCCEEDED(PFSettingsSetTraceLevel(PFTraceLevel::Error));
    EXPECT_EQ(E_INVALIDARG, PFSettingsSetTraceLevel(static_cast<PFTraceLevel>(6)));

    EmitTraceEvents();
    EXPECT_EQ(1u, s_traceCount.load());
}

// Verifies that a trace emitted by PlayFabGameSave.dll is filtered using the level stored in PlayFabCore.dll.
TEST_F(TestSuiteTrace, FiltersGameSaveDllTraceEvents)
{
    ASSERT_HRESULT_SUCCEEDED(PFSettingsSetTraceLevel(PFTraceLevel::Error));
    ASSERT_HRESULT_SUCCEEDED(PFGameSaveFilesSetForceInprocForDebug(true));
    EXPECT_EQ(0u, s_traceCount.load());

    ASSERT_HRESULT_SUCCEEDED(PFSettingsSetTraceLevel(PFTraceLevel::Information));
    ASSERT_HRESULT_SUCCEEDED(PFGameSaveFilesSetForceInprocForDebug(true));
    EXPECT_EQ(1u, s_traceCount.load());
}
