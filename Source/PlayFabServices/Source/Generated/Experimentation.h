#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "ExperimentationTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace Experimentation
{

class ExperimentationAPI
{
public:
    ExperimentationAPI() = delete;
    ExperimentationAPI(const ExperimentationAPI& source) = delete;
    ExperimentationAPI& operator=(const ExperimentationAPI& source) = delete;
    ~ExperimentationAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<GetTreatmentAssignmentResult> GetTreatmentAssignment(Entity const& entity, const GetTreatmentAssignmentRequest& request, RunContext rc);
};

} // namespace Experimentation
} // namespace PlayFab
