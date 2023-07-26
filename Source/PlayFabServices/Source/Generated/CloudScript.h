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
    static AsyncOp<ExecuteFunctionResult> ExecuteFunction(Entity const& entity, const ExecuteFunctionRequest& request, RunContext rc);
};

} // namespace CloudScript
} // namespace PlayFab
