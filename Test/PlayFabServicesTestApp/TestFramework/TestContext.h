// Copyright (C) Microsoft Corporation. All rights reserved.

#pragma once

#include "XAsyncOperation.h"
#include <EnumTraits.h>

namespace PlayFab
{
namespace Test
{

// TestClass forward declaration
class TestClass;

enum class TestActiveState
{
    PENDING, // Not started
    ACTIVE, // Currently testing
    COMPLETE, // Test Complete
};

enum class TestFinishState
{
    PENDING,
    PASSED,
    FAILED,
    SKIPPED,
    TIMEDOUT
};

/// <summary>
/// A TestContext contains metadata about a test including the TestClass it belongs to, the test function, and the result
/// </summary>
class TestContext
{
public:
    using TestFunc = std::function<void(TestContext&)>;

    TestContext(const char* name, TestFunc func);

    // Test metadata
    String const& TestName() const;
    TestActiveState ActiveState() const;
    TestFinishState FinishState() const;
    int64_t StartTime() const;
    int64_t EndTime() const;
    String const& Summary() const;

    // Start the test by invoking TestClass::SetUp followed by the TestFunc
    void StartTest();

    // Record an intermediate result but do not end the test. If any intermediate results fail, the entire test will be considered failed
    template<typename T>
    void RecordResult(Result<T>&& result) noexcept;

    // Verify a field of a result object, throwing a PlayFab::Test::Exception if validation fails
    template<typename T, std::enable_if_t<!std::is_enum_v<T>>* = nullptr>
    void AssertEqual(T const& expected, T const& actual, const char* propertyName);

    // Verify an enum field of a result object, throwing a PlayFab::Test::Exception if validation fails
    template<typename EnumT, std::enable_if_t<std::is_enum_v<EnumT>>* = nullptr>
    void AssertEqual(EnumT expected, EnumT actual, const char* propertyName);

    // Verify a string field of a result object, throwing a PlayFab::Test::Exception if validation fails
    void AssertEqual(const char* expected, const char* actual, const char* propertyName);

    // Validate a statement is true, throwing a PlayFab::Test::Exception if validation fails
    void AssertTrue(bool statement, const char* errorMessage);

    // Validate that an operation which was expected to fail actually fails
    template<typename T>
    void AssertFailed(Result<T>&& result, const char* errorMessage);

    // Add a final result and end the test
    void EndTest(Result<void>&& finalResult) noexcept;
    template<typename T>
    void EndTest(Result<T>&& finalResult) noexcept;

    // End this test immediately with TestFinishState::SKIPPED
    void Skip();
    // End this test immediately with TestFinishState::TIMEDOUT
    void Timeout();
    // Check if the test has been running for too long and should be timed out
    bool IsExpired();

private:
    void EndTest() noexcept;
    void EndTest(TestFinishState state, String resultMsg) noexcept;

    String const m_testName;
    TestActiveState m_activeState{ TestActiveState::PENDING };
    TestFinishState m_finishState{ TestFinishState::PENDING };
    String m_testResultMsg;
    TestFunc m_testFunc;
    int64_t m_startTime;
    int64_t m_endTime;
    Vector<Result<void>> m_intermediateResults;
};

template<typename T>
void TestContext::RecordResult(Result<T>&& result) noexcept
{
    if (Failed(result))
    {
        m_intermediateResults.emplace_back(result.hr, std::move(result.errorMessage));
    }
}

template<typename T, std::enable_if_t<!std::is_enum_v<T>>*>
void TestContext::AssertEqual(T const& expected, T const& actual, const char* propertyName) 
{
    if (!(expected == actual))
    {
        Stringstream ss;
        ss << "Result property verification failed: propertyName=\"" << propertyName << "\", expected=\"" << expected << "\" actual=\"" << actual << "\"";
        throw Exception{ ss.str() };
    }
}

template<typename EnumT, std::enable_if_t<std::is_enum_v<EnumT>>*>
void TestContext::AssertEqual(EnumT expected, EnumT actual, const char* propertyName)
{
    if (!(expected == actual))
    {
        Stringstream ss;
        ss << "Result property verification failed: propertyName=\"" << propertyName << "\", expected=\"" << EnumName(expected) << "\" actual=\"" << EnumName(actual) << "\"";
        throw Exception{ ss.str() };
    }
}

template<typename T>
void TestContext::AssertFailed(Result<T>&& result, const char* errorMessage)
{
    if (!Failed(result))
    {
        throw Exception{ errorMessage };
    }
}

template<typename T>
void TestContext::EndTest(Result<T>&& finalResult) noexcept
{
    RecordResult(std::move(finalResult));
    EndTest();
}

} // namespace Test
} // namespace PlayFab
