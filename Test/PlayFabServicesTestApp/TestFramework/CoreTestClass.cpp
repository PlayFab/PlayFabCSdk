#include "TestAppPch.h"
#include "CoreTestClass.h"
#include "Operations/Core/CoreOperations.h"
#include "Platform/PlayFabPal.h"

namespace PlayFab
{
namespace Test
{

CoreTestClass::CoreTestClass(TestTitleData testTitleData) :
    m_testTitleData{ std::move(testTitleData) },
    m_runContext{ RunContext::Root(nullptr) }
{
}

void CoreTestClass::ClassSetUp()
{
    THROW_IF_FAILED(Initialize().Wait().hr);
}

void CoreTestClass::ClassTearDown()
{
    THROW_IF_FAILED(Uninitialize().Wait().hr);
}

AsyncOp<void> CoreTestClass::Initialize()
{
    RETURN_IF_FAILED(Platform::CoreInitialize(nullptr));
    m_serviceConfig.emplace(m_testTitleData.connectionString.data(), m_testTitleData.titleId.data());
    return S_OK;
}

AsyncOp<void> CoreTestClass::Uninitialize()
{
    // Destroy ServiceConfig
    m_serviceConfig.reset();
    return RunOperation(MakeUnique<CoreUninitializeOperation>(RunContext()));
}

RunContext CoreTestClass::RunContext() const
{
    return m_runContext;
}

ServiceConfig CoreTestClass::ServiceConfig()
{
    assert(m_serviceConfig.has_value());
    return *m_serviceConfig;
}

}
}
