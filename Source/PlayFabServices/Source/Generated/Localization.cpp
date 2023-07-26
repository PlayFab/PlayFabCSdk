#include "stdafx.h"
#include "Localization.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace Localization
{


AsyncOp<GetLanguageListResponse> LocalizationAPI::GetLanguageList(
    Entity const& entity,
    const GetLanguageListRequest& request,
    RunContext rc
)
{
    const char* path{ "/Locale/GetLanguageList" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::LocalizationGetLanguageList,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetLanguageListResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetLanguageListResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetLanguageListResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // namespace Localization
} // namespace PlayFab
