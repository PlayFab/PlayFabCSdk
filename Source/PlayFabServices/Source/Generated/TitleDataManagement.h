#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "TitleDataManagementTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace TitleDataManagement
{

class TitleDataManagementAPI
{
public:
    TitleDataManagementAPI() = delete;
    TitleDataManagementAPI(const TitleDataManagementAPI& source) = delete;
    TitleDataManagementAPI& operator=(const TitleDataManagementAPI& source) = delete;
    ~TitleDataManagementAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<GetPublisherDataResult> ClientGetPublisherData(Entity const& entity, const GetPublisherDataRequest& request, RunContext rc);
    static AsyncOp<GetTimeResult> ClientGetTime(Entity const& entity, RunContext rc);
    static AsyncOp<GetTitleDataResult> ClientGetTitleData(Entity const& entity, const GetTitleDataRequest& request, RunContext rc);
    static AsyncOp<GetTitleNewsResult> ClientGetTitleNews(Entity const& entity, const GetTitleNewsRequest& request, RunContext rc);
    static AsyncOp<GetPublisherDataResult> ServerGetPublisherData(Entity const& entity, const GetPublisherDataRequest& request, RunContext rc);
    static AsyncOp<GetTimeResult> ServerGetTime(Entity const& entity, RunContext rc);
    static AsyncOp<GetTitleDataResult> ServerGetTitleData(Entity const& entity, const GetTitleDataRequest& request, RunContext rc);
    static AsyncOp<GetTitleDataResult> ServerGetTitleInternalData(Entity const& entity, const GetTitleDataRequest& request, RunContext rc);
    static AsyncOp<GetTitleNewsResult> ServerGetTitleNews(Entity const& entity, const GetTitleNewsRequest& request, RunContext rc);
    static AsyncOp<void> ServerSetPublisherData(Entity const& entity, const SetPublisherDataRequest& request, RunContext rc);
    static AsyncOp<void> ServerSetTitleData(Entity const& entity, const SetTitleDataRequest& request, RunContext rc);
    static AsyncOp<void> ServerSetTitleInternalData(Entity const& entity, const SetTitleDataRequest& request, RunContext rc);
};

} // namespace TitleDataManagement
} // namespace PlayFab
