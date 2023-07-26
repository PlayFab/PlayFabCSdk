#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "LocalizationTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace Localization
{

class LocalizationAPI
{
public:
    LocalizationAPI() = delete;
    LocalizationAPI(const LocalizationAPI& source) = delete;
    LocalizationAPI& operator=(const LocalizationAPI& source) = delete;
    ~LocalizationAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<GetLanguageListResponse> GetLanguageList(Entity const& entity, const GetLanguageListRequest& request, RunContext rc);
};

} // namespace Localization
} // namespace PlayFab
