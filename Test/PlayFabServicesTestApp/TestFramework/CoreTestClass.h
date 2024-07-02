#pragma once

#include "TestClass.h"

namespace PlayFab
{
namespace Test
{

/// <summary>
/// Base class for TestClasses testing PlayFabCore functionality. Providers helpers for intialization, cleanup, etc.
/// </summary>
class CoreTestClass : public TestClass
{
public:
    // Add optional way to specify Test RunContext queue and PlayFab background queue. Will default to process queue for both
    CoreTestClass(TestTitleData testTitleData);

    virtual AsyncOp<void> Initialize();
    virtual AsyncOp<void> Uninitialize();

protected:
    // RunContext for tests
    RunContext RunContext() const;

    // ServiceConfig created from testTitleData
    ServiceConfig ServiceConfig();

protected:
    TestTitleData m_testTitleData;

private:
    PlayFab::RunContext m_runContext;
    std::optional<Test::ServiceConfig> m_serviceConfig; // optional because intialization is delayed until Initialize
};

}
}
