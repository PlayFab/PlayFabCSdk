#include "TestAppPch.h"
#include "TestClass.h"

namespace PlayFab
{
namespace Test
{

TestList const& TestClass::GetTests()
{
    if (m_testList.empty())
    {
        AddTests();
    }
    return m_testList;
}

}
}
