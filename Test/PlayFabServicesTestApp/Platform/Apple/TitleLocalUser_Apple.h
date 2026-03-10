#pragma once

#include "Platform/TitleLocalUser.h"

namespace PlayFab
{
namespace Test
{

#ifdef __cplusplus
extern "C"
{
#endif

// This is a Swift method that we call to get the Identity Token when using Apple Sign-In
extern const char* GetIdentityToken();

#ifdef __cplusplus
}
#endif

struct AppleLocalUserContext : public TitleLocalUserContext
{
    AppleLocalUserContext(String customId);

    static Result<String> GetUserIdentityToken();
    
    static HRESULT LocalUserLoginHandler(
        PFLocalUserHandle localUserHandle,
        PFServiceConfigHandle serviceConfigHandle,
        PFEntityHandle entityHandle,
        XAsyncBlock* async
    );

    String const customId;
    
};

} // Test
} // PlayFab
