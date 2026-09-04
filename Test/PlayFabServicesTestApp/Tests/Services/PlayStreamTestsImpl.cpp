#include "TestAppPch.h"
#include "PlayStreamTests.h"

namespace PlayFab
{
namespace Test
{

AsyncOp<void> PlayStreamTests::Initialize()
{
    return ServicesTestClass::Initialize();
}

AsyncOp<void> PlayStreamTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

}
}
