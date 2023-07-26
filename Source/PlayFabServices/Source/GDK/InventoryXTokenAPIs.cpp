#include "stdafx.h"
#include "Generated/Inventory.h"
#include "JsonUtils.h"
#include "GDK/PlatformUser_GDK.h"

namespace PlayFab
{
namespace Inventory
{

AsyncOp<RedeemMicrosoftStoreInventoryItemsResponse> InventoryAPI::RedeemMicrosoftStoreInventoryItems(
    Entity const& entity,
    const RedeemMicrosoftStoreInventoryItemsRequest& request,
    RunContext rc
)
{
    auto duplicateUserResult = XUser::Duplicate(request.Model().user);
    RETURN_IF_FAILED(duplicateUserResult.hr);
    
    return duplicateUserResult.Payload().GetTokenAndSignature(rc).Then([entity, request, rc](Result<Platform::TokenAndSignature> result) mutable -> AsyncOp<ServiceResponse>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        const char* path{ "/Inventory/RedeemMicrosoftStoreInventoryItems" };
        JsonValue requestBody{ request.ToJson() };
        JsonUtils::ObjectAddMember(requestBody, "XboxToken", result.ExtractPayload().token);
        
        return ServicesHttpClient::MakeEntityRequest(
            ServicesCacheId::InventoryRedeemMicrosoftStoreInventoryItems,
            entity,
            path,
            std::move(requestBody),
            rc.Derive()
        );
    })
    .Then([](Result<ServiceResponse> result) -> Result<RedeemMicrosoftStoreInventoryItemsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            RedeemMicrosoftStoreInventoryItemsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<RedeemMicrosoftStoreInventoryItemsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // namespace Inventory
} // namespace PlayFab
