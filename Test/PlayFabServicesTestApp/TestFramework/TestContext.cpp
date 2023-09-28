// Copyright (C) Microsoft Corporation. All rights reserved.

#include "TestAppPch.h"
#include "TestContext.h"
#include "TestClass.h"
#include "EnumTraits.h"
#include "TestReport.h"

namespace PlayFab
{
namespace Test
{

TestContext::TestContext(const char* name, TestFunc func) :
    m_testName{ name },
    m_testFunc{ std::move(func) }
{
}

String const& TestContext::TestName() const
{
    return m_testName;
}

TestActiveState TestContext::ActiveState() const
{
    return m_activeState;
}

TestFinishState TestContext::FinishState() const
{
    return m_finishState;
}

int64_t TestContext::StartTime() const
{
    return m_startTime;
}

int64_t TestContext::EndTime() const
{
    return m_endTime;
}

String const& TestContext::Summary() const
{
    return m_testResultMsg;
}

void TestContext::StartTest()
{
    assert(m_activeState == TestActiveState::PENDING);

    m_startTime = GetMilliTicks();
    m_activeState = TestActiveState::ACTIVE;
    m_testFunc(*this);
}

void TestContext::AssertEqual(const char* expected, const char* actual, const char* propertyName)
{
    if (expected && actual && std::strcmp(expected, actual))
    {
        Stringstream ss;
        ss << "Result property verification failed: propertyName=\"" << propertyName << "\", expected=\"" << expected << "\" actual=\"" << actual << "\"";
        throw Exception{ ss.str() };
    }
    else if ((!expected && actual) || (expected && !actual))
    {
        Stringstream ss;
        ss << "Result property verification failed: propertyName=\"" << propertyName << "\"";
        throw Exception{ ss.str() };
    }
}

void TestContext::AssertTrue(bool statement, const char* errorMessage)
{
    if (!statement)
    {
        throw Exception{ errorMessage };
    }
}

void TestContext::EndTest(Result<void>&& finalResult) noexcept
{
    RecordResult(std::move(finalResult));
    EndTest();
}

void TestContext::EndTest() noexcept
{
    TestFinishState finishState{ TestFinishState::PASSED };

    Stringstream ss;
    ss << "Test failed with " << m_intermediateResults.size() << " intermediate result(s):";
    for (auto& result : m_intermediateResults)
    {
        if (Failed(result))
        {
            finishState = TestFinishState::FAILED;
        }
        ss << "\n\thr=" << std::hex << result.hr << ", errorMessage=" << result.errorMessage;
    }
    EndTest(finishState, finishState == TestFinishState::FAILED ? ss.str() : String{});
}

void TestContext::EndTest(TestFinishState state, String resultMsg) noexcept
{
    if (m_finishState == TestFinishState::PENDING) // This means that we finish successfully
    {
        auto now = GetMilliTicks();
        if (m_activeState == TestActiveState::PENDING)
        {
            // Test was never started. Set start time now so runtime calculation works
            m_startTime = now;
        }

        m_endTime = now;
        m_testResultMsg = resultMsg;
        m_finishState = state;
        m_activeState = TestActiveState::COMPLETE;
    }
    else
    {
        // Test was previously completed
        if (!m_testResultMsg.empty())
        {
            m_testResultMsg += "\n";
        }
        m_testResultMsg += EnumName(m_finishState) + String{ "->" } + EnumName(state) + " - Cannot declare test finished twice.";
        if (!resultMsg.empty())
        {
            m_testResultMsg += "\n: " + resultMsg;
        }
        m_finishState = TestFinishState::FAILED;
    }
}

void TestContext::Skip()
{
    // Tests should not be skipped after results have been recorded
    assert(m_intermediateResults.empty());
    EndTest(TestFinishState::SKIPPED, String{});
}

void TestContext::Timeout()
{
    EndTest(TestFinishState::TIMEDOUT, String{});
}

static const int64_t TEST_TIMEOUT_MILLISECONDS = 150000;

bool TestContext::IsExpired()
{
    int64_t timeNow = GetMilliTicks();
    bool timeExpired = (timeNow - m_startTime) > TEST_TIMEOUT_MILLISECONDS;
    return timeExpired;
}

} // namespace Test
} // namespace PlayFab
