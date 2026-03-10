#include "Platform/TitleLocalUser.h"

namespace PlayFab
{
namespace Test
{

// A local user context that can be used to log a Local user into PlayFab on any platform using a customId
struct CustomLocalUserContext : public TitleLocalUserContext
{
    CustomLocalUserContext(String customId);

    static HRESULT CALLBACK LocalUserLoginHandler(
        PFLocalUserHandle localUserHandle,
        PFServiceConfigHandle serviceConfigHandle,
        PFEntityHandle entityHandle,
        XAsyncBlock* async
    );

    String const customId;
};

} // Test
} // PlayFab
