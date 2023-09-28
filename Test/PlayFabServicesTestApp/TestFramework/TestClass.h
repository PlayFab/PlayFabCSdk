// Copyright (C) Microsoft Corporation. All rights reserved.

#pragma once

#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

using TestList = List<SharedPtr<TestContext>>;

/// <summary>
/// A TestClass implements a set of Tests to be run by a TestRunner
/// </summary>
class TestClass
{
public:
    TestClass() = default;
    virtual ~TestClass() = default;

    /// <summary>
    /// Provide read-only access to the test list.
    /// Will trigger building the test list, if it's not already populated.
    /// <summary>
    TestList const& GetTests();

    /// <summary>
    /// During testing, this is the first function that will be called for each TestClass.
    /// This is run exactly once for this type.
    /// </summary>
    virtual void ClassSetUp() {}

    /// <summary>
    /// During testing, this will be called every tick that a test is asynchronous.
    /// This is run every tick until testContext.EndTest() is called, or until the test times out.
    /// This is considered part of the active test. A failure or exception in this method will be considered a failure for the active test.
    /// </summary>
    virtual void Tick(TestContext& /*testContext*/) {}

    /// <summary>
    /// During testing, this is the last function that will be called for each TestClass.
    /// This is run exactly once for this type.
    /// It is not considered part of any test. A failure or exception in this method will halt the test framework.
    /// </summary>
    virtual void ClassTearDown() {}

protected:
    /// <summary>
    /// Before testing, this function is called to gather the list of tests to run for each TestClass.
    /// It is not considered part of any test.
    /// <summary>
    virtual void AddTests() = 0;

    /// <summary>
    /// Helper method to add a Test to the TestList
    /// </summary>
    template <class T> 
    void AddTest(const char* testName, void(T::* testCaseFunc)(TestContext&));

private:
    TestList m_testList;
};

template <class T>
void TestClass::AddTest(const char* testName, void(T::* testCaseFunc)(TestContext&))
{
    T* testClass = static_cast<T*>(this);
    const auto& testFunc = std::bind(testCaseFunc, testClass, std::placeholders::_1);
    SharedPtr<TestContext> testContext = MakeShared<TestContext>(testName, testFunc);

    m_testList.push_back(testContext);
}

}
}
