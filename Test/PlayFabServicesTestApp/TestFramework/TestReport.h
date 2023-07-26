// Copyright (C) Microsoft Corporation. All rights reserved.

#pragma once

#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

/// <summary>
/// Data container defining the test-case data saved to JUnit XML format
/// </summary>
class TestCaseReport
{
public:
    String classname; // suite class name
    String name; // test name
    double time; // Duration in seconds
    // Sub-Fields in the XML spec
    /// <summary> message is the descriptive text used to debug the test failure </summary>
    String message;
    /// <summary> The xml spec allows failureText to be an arbitrary string.  When possible it should match FinishState (But not required) </summary>
    String failureText;
    TestFinishState finishState;
};

/// <summary>
/// Data container defining the test-suite data saved to JUnit XML format
/// </summary>
class TestSuiteReport
{
public:
    String name = "default"; // suite class name
    int tests; // total test count
    int failures; // count tests in state
    int errors; // count tests in state
    int skipped; // count tests in state
    double time; // Duration in seconds
    // Useful for debugging but not part of the serialized format
    int64_t timeStamp;
    int passed; // Could be calculated from the others, but sometimes knowing if they don't add up means something
    List<SharedPtr<TestCaseReport>> testResults;
};

class TestReport
{
public:
    TestReport(String testReportName);

    void TestStarted();
    void TestComplete(String testName, TestFinishState testFinishState, int64_t testDurationMs, String message);
    bool AllTestsPassed() const;

private:
    TestSuiteReport m_internalReport;

};

} // namespace Test
} // namespace PlayFab
