#pragma once

#include "ServicesTestClass.h"

namespace PlayFab
{
namespace Test
{

class ExperimentationTests : public ServicesTestClass
{
public:
    using ServicesTestClass::ServicesTestClass;

private:
    void AddTests() override;

    AsyncOp<void> Initialize() override;
    AsyncOp<void> Uninitialize() override;

    SharedPtr<struct ExperimentationTestsState> m_state;

private:
    // Autogenerated tests
    void TestGetTreatmentAssignment(TestContext& testContext);

};

} // namespace Test
} // namespace PlayFab
