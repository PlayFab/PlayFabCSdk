#pragma once

#include "Event.h"

namespace PlayFab
{
namespace UnitTests
{

// Helper class for marshalling result of multithreaded/asyncronous tests to the correct thread
class AsyncTestContext
{
public:
    AsyncTestContext() = default;
    AsyncTestContext(const AsyncTestContext&) = delete;
    ~AsyncTestContext();

    void Assert(bool condition);
    void AssertSucceeded(HRESULT hr);

    template<typename T>
    inline void AssertSucceeded(Result<T> result);

    void Complete();
    void AwaitResult();

private:
    std::mutex m_mutex;
    std::optional<HRESULT> m_result;
    Event m_complete;
};

template<typename T>
void AsyncTestContext::AssertSucceeded(Result<T> result)
{
    AssertSucceeded(result.hr);
}

}
}
