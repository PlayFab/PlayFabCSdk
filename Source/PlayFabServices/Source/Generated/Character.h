#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "CharacterTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace Character
{

class CharacterAPI
{
public:
    CharacterAPI() = delete;
    CharacterAPI(const CharacterAPI& source) = delete;
    CharacterAPI& operator=(const CharacterAPI& source) = delete;
    ~CharacterAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<ListUsersCharactersResult> ClientGetAllUsersCharacters(Entity const& entity, const ListUsersCharactersRequest& request, RunContext rc);
    static AsyncOp<ClientGetCharacterDataResult> ClientGetCharacterData(Entity const& entity, const GetCharacterDataRequest& request, RunContext rc);
    static AsyncOp<ClientGetCharacterDataResult> ClientGetCharacterReadOnlyData(Entity const& entity, const GetCharacterDataRequest& request, RunContext rc);
    static AsyncOp<ClientGrantCharacterToUserResult> ClientGrantCharacterToUser(Entity const& entity, const ClientGrantCharacterToUserRequest& request, RunContext rc);
    static AsyncOp<UpdateCharacterDataResult> ClientUpdateCharacterData(Entity const& entity, const ClientUpdateCharacterDataRequest& request, RunContext rc);
    static AsyncOp<void> ServerDeleteCharacterFromUser(Entity const& entity, const DeleteCharacterFromUserRequest& request, RunContext rc);
    static AsyncOp<ListUsersCharactersResult> ServerGetAllUsersCharacters(Entity const& entity, const ListUsersCharactersRequest& request, RunContext rc);
    static AsyncOp<ServerGetCharacterDataResult> ServerGetCharacterData(Entity const& entity, const GetCharacterDataRequest& request, RunContext rc);
    static AsyncOp<ServerGetCharacterDataResult> ServerGetCharacterInternalData(Entity const& entity, const GetCharacterDataRequest& request, RunContext rc);
    static AsyncOp<ServerGetCharacterDataResult> ServerGetCharacterReadOnlyData(Entity const& entity, const GetCharacterDataRequest& request, RunContext rc);
    static AsyncOp<ServerGrantCharacterToUserResult> ServerGrantCharacterToUser(Entity const& entity, const ServerGrantCharacterToUserRequest& request, RunContext rc);
    static AsyncOp<UpdateCharacterDataResult> ServerUpdateCharacterData(Entity const& entity, const ServerUpdateCharacterDataRequest& request, RunContext rc);
    static AsyncOp<UpdateCharacterDataResult> ServerUpdateCharacterInternalData(Entity const& entity, const ServerUpdateCharacterDataRequest& request, RunContext rc);
    static AsyncOp<UpdateCharacterDataResult> ServerUpdateCharacterReadOnlyData(Entity const& entity, const ServerUpdateCharacterDataRequest& request, RunContext rc);
};

} // namespace Character
} // namespace PlayFab
