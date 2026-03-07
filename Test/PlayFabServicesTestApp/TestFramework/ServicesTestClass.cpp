#include "TestAppPch.h"
#include "ServicesTestClass.h"
#include "Operations/Core/CoreOperations.h"
#include "Operations/Core/AuthenticationOperations.h"
#include "Operations/Services/ServicesOperations.h"
#include "AccountManagementOperations.h"
#include "Platform/PlayFabPal.h"

namespace PlayFab
{
namespace Test
{

AsyncOp<void> ServicesTestClass::Initialize()
{
    auto operation = CoreTestClass::Initialize().Then([this](Result<void> result) -> AsyncOp<TitleLocalUser>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        RETURN_IF_FAILED(Platform::ServicesInitialize());
        return Platform::GetDefaultLocalUser(ServiceConfig(), RunContext());
    })
    .Then([&](Result<TitleLocalUser> result) -> AsyncOp<LoginResult>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        m_defaultLocalUser.emplace(result.ExtractPayload());
        return RunOperation(MakeUnique<LocalUserLoginOperation>(*m_defaultLocalUser, RunContext()));
    })
    .Then([&](Result<LoginResult> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        m_defaultTitlePlayer.emplace(result.ExtractPayload());
        return S_OK;
    });

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    return operation.Then([&](Result<void> result) -> AsyncOp<Entity>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        return RunOperation(MakeUnique<GetEntityWithSecretKeyOperation>(ServiceConfig(), m_testTitleData.secretKey, GetEntityWithSecretKeyOperation::RequestType{}, RunContext()));
    })
    .Then([&](Result<Entity> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        m_titleEntity.emplace(std::move(result.ExtractPayload()));
        return S_OK;
    });
#else
    return operation;
#endif
}

AsyncOp<void> ServicesTestClass::Uninitialize()
{
    m_defaultLocalUser.reset();
    m_defaultTitlePlayer.reset();
#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    m_titleEntity.reset();
#endif

    return RunOperation(MakeUnique<ServicesUninitializeOperation>(RunContext())).Then([this](Result<void> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        return CoreTestClass::Uninitialize();
    });
}

TitleLocalUser ServicesTestClass::DefaultLocalUser() noexcept
{
    assert(m_defaultLocalUser.has_value());
    return *m_defaultLocalUser;
}

Entity ServicesTestClass::DefaultTitlePlayer() noexcept
{
    assert(m_defaultTitlePlayer.has_value());
    return m_defaultTitlePlayer->entity;
}

String ServicesTestClass::DefaultTitlePlayerId() noexcept
{
    assert(m_defaultTitlePlayer.has_value());
    return m_defaultTitlePlayer->loginResult.Model().playFabId;
}

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
Entity ServicesTestClass::TitleEntity() noexcept
{
    assert(m_titleEntity.has_value());
    return *m_titleEntity;
}
#endif

AsyncOp<Entity> ServicesTestClass::GetTitlePlayer(String customId) noexcept
{
    auto iter = m_playersByCustomId.find(customId);
    if (iter != m_playersByCustomId.end())
    {
        return Result<Entity>{ Entity{ iter->second.entity } };
    }

    LoginWithCustomIDOperation::RequestType request;
    request.SetCustomId(customId);
    request.SetCreateAccount(true);
    return RunOperation(MakeUnique<LoginWithCustomIDOperation>(ServiceConfig(), request, RunContext())).Then([this, customId](Result<LoginResult> result) -> Result<Entity>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        auto entity = result.Payload().entity;
        m_playersByCustomId.emplace(customId, result.ExtractPayload());
        return entity;
    });
}

AsyncOp<String> ServicesTestClass::GetPlayFabIdFromCustomId(String customId) noexcept
{
    auto iter = m_playersByCustomId.find(customId);
    if (iter != m_playersByCustomId.end())
    {
        return Result<String>{ iter->second.loginResult.Model().playFabId };
    }

    LoginWithCustomIDOperation::RequestType request;
    request.SetCustomId(customId);
    request.SetCreateAccount(true);
    return RunOperation(MakeUnique<LoginWithCustomIDOperation>(ServiceConfig(), request, RunContext())).Then([this, customId](Result<LoginResult> result) -> Result<String>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        String playFabId = result.Payload().loginResult.Model().playFabId;
        m_playersByCustomId.emplace(customId, result.ExtractPayload());
        return playFabId;
    });
}

}
}
