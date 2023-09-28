#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "CloudScriptTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace CloudScript
{

class CloudScriptAPI
{
public:
    CloudScriptAPI() = delete;
    CloudScriptAPI(const CloudScriptAPI& source) = delete;
    CloudScriptAPI& operator=(const CloudScriptAPI& source) = delete;
    ~CloudScriptAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<ExecuteCloudScriptResult> ClientExecuteCloudScript(Entity const& entity, const ExecuteCloudScriptRequest& request, RunContext rc);
    static AsyncOp<ExecuteCloudScriptResult> ServerExecuteCloudScript(Entity const& entity, const ExecuteCloudScriptServerRequest& request, RunContext rc);
    static AsyncOp<ExecuteCloudScriptResult> ExecuteEntityCloudScript(Entity const& entity, const ExecuteEntityCloudScriptRequest& request, RunContext rc);
    static AsyncOp<ExecuteFunctionResult> ExecuteFunction(Entity const& entity, const ExecuteFunctionRequest& request, RunContext rc);
};

} // namespace CloudScript
} // namespace PlayFab
