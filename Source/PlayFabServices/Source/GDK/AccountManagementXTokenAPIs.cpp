#include "stdafx.h"
#include "Generated/AccountManagement.h"
#include "JsonUtils.h"
#include "GDK/PlatformUser_GDK.h"

namespace PlayFab
{
namespace AccountManagement
{

AsyncOp<void> AccountManagementAPI::ClientLinkXboxAccount(
    Entity const& entity,
    const ClientLinkXboxAccountRequest& request,
    RunContext rc
)
{
    auto duplicateUserResult = XUser::Duplicate(request.Model().user);
    RETURN_IF_FAILED(duplicateUserResult.hr);

    return duplicateUserResult.Payload().GetTokenAndSignature(rc).Then([entity, request, rc](Result<Platform::TokenAndSignature> result) mutable -> AsyncOp<ServiceResponse>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        const char* path{ "/Client/LinkXboxAccount" };
        JsonValue requestBody{ request.ToJson() };
        JsonUtils::ObjectAddMember(requestBody, "XboxToken", result.ExtractPayload().token);

        return ServicesHttpClient::MakeEntityRequest(
            ServicesCacheId::AccountManagementClientLinkXboxAccount,
            entity,
            path,
            requestBody,
            std::move(rc)
        );
    })
    .Then([](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return S_OK;
        }
        else
        {
            return Result<void>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // namespace AccountManagement
} // namespace PlayFab
