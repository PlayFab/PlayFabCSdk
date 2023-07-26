#include "stdafx.h"
#include "Generated/Friends.h"
#include "JsonUtils.h"
#include "GDK/PlatformUser_GDK.h"

namespace PlayFab
{
namespace Friends
{

AsyncOp<GetFriendsListResult> FriendsAPI::ClientGetFriendsList(
    Entity const& entity,
    const ClientGetFriendsListRequest& request,
    RunContext rc
)
{
    const auto getFriendsListContinuation = [](Result<ServiceResponse> result) -> Result<GetFriendsListResult>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetFriendsListResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetFriendsListResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    };
    
    if (request.Model().user)
    {
        auto duplicateUserResult = XUser::Duplicate(request.Model().user);
        RETURN_IF_FAILED(duplicateUserResult.hr);

        return duplicateUserResult.Payload().GetTokenAndSignature(rc).Then([rc, entity, request](Result<Platform::TokenAndSignature> result) mutable -> AsyncOp<ServiceResponse>
        {
            RETURN_IF_FAILED_PLAYFAB(result);

            const char* path{ "/Client/GetFriendsList" };
            JsonValue requestBody{ request.ToJson() };
            JsonUtils::ObjectAddMember(requestBody, "XboxToken", result.ExtractPayload().token);

            return ServicesHttpClient::MakeEntityRequest(
                ServicesCacheId::FriendsClientGetFriendsList,
                entity,
                path,
                std::move(requestBody),
                rc.Derive()
            );
        })
        .Then(getFriendsListContinuation);
    }
    else
    {
        const char* path{ "/Client/GetFriendsList" };
        JsonValue requestBody{ request.ToJson() };

        auto requestOp = ServicesHttpClient::MakeEntityRequest(
            ServicesCacheId::FriendsClientGetFriendsList,
            entity,
            path,
            requestBody,
            std::move(rc)
        );

        return requestOp.Then(getFriendsListContinuation);
    }
}

} // namespace Friends
} // namespace PlayFab
