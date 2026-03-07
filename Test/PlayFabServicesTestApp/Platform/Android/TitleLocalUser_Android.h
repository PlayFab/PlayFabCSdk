#include "Platform/TitleLocalUser.h"

namespace PlayFab
{
namespace Test
{

struct AndroidLocalUserContext : public TitleLocalUserContext
{
    AndroidLocalUserContext(String customId);

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
