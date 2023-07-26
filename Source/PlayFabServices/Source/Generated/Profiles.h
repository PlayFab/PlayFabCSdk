#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "ProfilesTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace Profiles
{

class ProfilesAPI
{
public:
    ProfilesAPI() = delete;
    ProfilesAPI(const ProfilesAPI& source) = delete;
    ProfilesAPI& operator=(const ProfilesAPI& source) = delete;
    ~ProfilesAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<GetEntityProfileResponse> GetProfile(Entity const& entity, const GetEntityProfileRequest& request, RunContext rc);
    static AsyncOp<GetEntityProfilesResponse> GetProfiles(Entity const& entity, const GetEntityProfilesRequest& request, RunContext rc);
    static AsyncOp<GetTitlePlayersFromMasterPlayerAccountIdsResponse> GetTitlePlayersFromMasterPlayerAccountIds(Entity const& entity, const GetTitlePlayersFromMasterPlayerAccountIdsRequest& request, RunContext rc);
    static AsyncOp<SetProfileLanguageResponse> SetProfileLanguage(Entity const& entity, const SetProfileLanguageRequest& request, RunContext rc);
    static AsyncOp<SetEntityProfilePolicyResponse> SetProfilePolicy(Entity const& entity, const SetEntityProfilePolicyRequest& request, RunContext rc);
};

} // namespace Profiles
} // namespace PlayFab
