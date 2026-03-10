#include "Platform/TitleLocalUser.h"
#include <playfab/core/PFLocalUser_Xbox.h>
#include "PlayFabXUser.h"

namespace PlayFab
{
namespace Test
{

struct GDKLocalUserContext : public TitleLocalUserContext
{
    GDKLocalUserContext(PlayFab::XUser _xuser) : xuser{ std::move(_xuser) }
    {
    }

    PlayFab::XUser xuser;
};

inline PlayFab::XUser const& GetXUserFromLocalUser(TitleLocalUser const& localUser)
{
    auto context = std::dynamic_pointer_cast<GDKLocalUserContext>(localUser.context);
    assert(context);
    return context->xuser;
}

} // Test
} // PlayFab
