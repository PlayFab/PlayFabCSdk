#include "TestAppPch.h"
#include "ExperimentationTests.h"

namespace PlayFab
{
namespace Test
{

void ExperimentationTests::AddTests()
{
    AddTest("TestGetTreatmentAssignment", &ExperimentationTests::TestGetTreatmentAssignment, true);

}

}
}
