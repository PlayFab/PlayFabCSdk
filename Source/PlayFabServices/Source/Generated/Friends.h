#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "FriendsTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace Friends
{

class FriendsAPI
{
public:
    FriendsAPI() = delete;
    FriendsAPI(const FriendsAPI& source) = delete;
    FriendsAPI& operator=(const FriendsAPI& source) = delete;
    ~FriendsAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<AddFriendResult> ClientAddFriend(Entity const& entity, const ClientAddFriendRequest& request, RunContext rc);
    static AsyncOp<GetFriendsListResult> ClientGetFriendsList(Entity const& entity, const ClientGetFriendsListRequest& request, RunContext rc);
    static AsyncOp<void> ClientRemoveFriend(Entity const& entity, const ClientRemoveFriendRequest& request, RunContext rc);
    static AsyncOp<void> ClientSetFriendTags(Entity const& entity, const ClientSetFriendTagsRequest& request, RunContext rc);
    static AsyncOp<void> ServerAddFriend(Entity const& entity, const ServerAddFriendRequest& request, RunContext rc);
    static AsyncOp<GetFriendsListResult> ServerGetFriendsList(Entity const& entity, const ServerGetFriendsListRequest& request, RunContext rc);
    static AsyncOp<void> ServerRemoveFriend(Entity const& entity, const ServerRemoveFriendRequest& request, RunContext rc);
    static AsyncOp<void> ServerSetFriendTags(Entity const& entity, const ServerSetFriendTagsRequest& request, RunContext rc);
};

} // namespace Friends
} // namespace PlayFab
