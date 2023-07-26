#include "stdafx.h"
#include "TestIncludes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PlayFab
{
namespace UnitTests
{

AsyncTestContext::~AsyncTestContext()
{
    std::unique_lock<std::mutex> lock{ m_mutex };
    Assert::IsTrue(m_result.has_value());
}

void AsyncTestContext::Assert(bool condition)
{
    std::unique_lock<std::mutex> lock{ m_mutex };
    if (!condition && !m_result.has_value())
    {
        m_result.emplace(E_FAIL);
    }
}

void AsyncTestContext::AssertSucceeded(HRESULT hr)
{
    std::unique_lock<std::mutex> lock{ m_mutex };
    if (FAILED(hr) && !m_result.has_value())
    {
        m_result.emplace(hr);
    }
}

void AsyncTestContext::Complete()
{
    {
        std::unique_lock<std::mutex> lock{ m_mutex };
        if (!m_result.has_value())
        {
            m_result.emplace(S_OK);
        }
    }
    m_complete.Set();
}

void AsyncTestContext::AwaitResult()
{
    Assert::IsTrue(m_complete.Wait(INFINITE));

    std::unique_lock<std::mutex> lock{ m_mutex };
    Assert::IsTrue(m_result.has_value());
    Assert::IsTrue(SUCCEEDED(*m_result));
}

}
}
