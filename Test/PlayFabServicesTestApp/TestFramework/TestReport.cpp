// Copyright (C) Microsoft Corporation. All rights reserved.

#include "TestAppPch.h"
#include "TestReport.h"
#include "EnumTraits.h"

using namespace PlayFab;

namespace PlayFab
{
namespace Test
{

TestReport::TestReport(String testReportName)
{
    m_internalReport.name = std::move(testReportName);
    m_internalReport.timeStamp = GetMilliTicks();
    m_internalReport.tests = 0;
    m_internalReport.failures = 0;
    m_internalReport.errors = 0;
    m_internalReport.skipped = 0;
    m_internalReport.passed = 0;
}

void TestReport::TestStarted()
{
    m_internalReport.tests += 1;
}

void TestReport::TestComplete(String testName, TestFinishState testFinishState, int64_t testDurationMs, String message)
{
    // Add a new TestCaseReport for the completed test.
    std::shared_ptr<TestCaseReport> testReport = std::make_shared<TestCaseReport>();
    m_internalReport.testResults.push_back(testReport);

    testReport->classname = m_internalReport.name;
    testReport->name = testName;
    testReport->time = std::chrono::duration<double>(testDurationMs).count();
    testReport->message = message;
    testReport->finishState = testFinishState;
    testReport->failureText = EnumName(testFinishState);

    // Update statistics.
    switch (testFinishState)
    {
    case TestFinishState::PASSED: m_internalReport.passed += 1; break;
    case TestFinishState::FAILED: m_internalReport.failures += 1; break;
    case TestFinishState::SKIPPED: m_internalReport.skipped += 1; break;
    case TestFinishState::PENDING:
        break;
    case TestFinishState::TIMEDOUT:
        break;
    default:
        break;
    }

    // Update overall runtime.
    // TODO: Add hooks for SuiteSetUp and SuiteTearDown, so this can be estimated more accurately
    m_internalReport.time = (GetMilliTicks() - m_internalReport.timeStamp) / 1000.0; // For now, update the duration on every test complete - the last one will be essentially correct
}

bool TestReport::AllTestsPassed() const
{
    return (m_internalReport.tests > 0) &&
        ((m_internalReport.passed + m_internalReport.skipped) == m_internalReport.tests) &&
        (0 == m_internalReport.failures);
}

} // namespace Test
} // namespace PlayFab
