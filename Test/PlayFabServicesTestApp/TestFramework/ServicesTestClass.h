#pragma once

#include "CoreTestClass.h"

namespace PlayFab
{
namespace Test
{

class ServicesTestClass : public CoreTestClass
{
public:
    using CoreTestClass::CoreTestClass;

    AsyncOp<void> Initialize() override;
    AsyncOp<void> Uninitialize() override;

protected:
    // Default LocalUser.
    TitleLocalUser DefaultLocalUser() noexcept;

    // Default TitlePlayer. Implementation may vary by platform.
    // On GDK this will be a TitlePlayer associated with the default XUser.
    Entity DefaultTitlePlayer() noexcept;

    // PlayFabId for DefaultTitlePlayer
    String DefaultTitlePlayerId() noexcept;

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    // Title Entity, need to call certain APIs
    Entity TitleEntity() noexcept;
#endif

    // Get TitlePlayer by customId, performing a login if needed
    AsyncOp<Entity> GetTitlePlayer(String customId) noexcept;

    // Get a PlayFabId from a customId. This may create a player with customId if one doesn't exist
    AsyncOp<String> GetPlayFabIdFromCustomId(String customId) noexcept;

private:
    std::optional<TitleLocalUser> m_defaultLocalUser;
    std::optional<LoginResult> m_defaultTitlePlayer;
    Map<String, LoginResult> m_playersByCustomId;
#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    std::optional<Entity> m_titleEntity;
#endif
};

}
}
