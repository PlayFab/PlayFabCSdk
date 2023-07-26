#include "stdafx.h"
#include "TestIncludes.h"
#include "AsyncOp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PlayFab
{
namespace UnitTests
{

AsyncOp<void> AsyncSleep(uint32_t sleepMs)
{
    auto opContext = std::make_shared<AsyncOpContext<void>>();

    std::thread asyncOp([opContext, sleepMs]()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
        opContext->Complete(Result<void>{ S_OK });
    });

    asyncOp.detach();

    return AsyncOp<void>{ opContext };
}

AsyncOp<int> AsyncIncrement(int input)
{
    auto opContext = std::make_shared<AsyncOpContext<int>>();

    std::thread asyncOp([opContext, input]()
    {
        opContext->Complete(Result<int>(input + 1));
    });

    asyncOp.detach();
    return AsyncOp<int>{ opContext };
}

TEST_CLASS(AsyncOpTests)
{
public:
    TEST_METHOD(BasicContinuation)
    {
        AsyncTestContext tc;
        const std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

        AsyncSleep(100).Finally([start, &tc](Result<void> result) mutable
            {
                tc.AssertSucceeded(result);
                tc.Assert(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() >= 100);
                tc.Complete();
            });

        tc.AwaitResult();
    }

    TEST_METHOD(ChainedOperation)
    {
        AsyncTestContext tc;

        AsyncIncrement(0).Then([](Result<int> result) -> AsyncOp<int>
        {
            if (Failed(result))
            {
                return result.hr;
            }
            return AsyncIncrement(result.Payload());
        })
        .Finally([&tc](Result<int> result) mutable
        {
            tc.Assert(Succeeded(result) && result.Payload() == 2);
            tc.Complete();
        });

        tc.AwaitResult();
    }

    TEST_METHOD(ContinuationException)
    {
        AsyncTestContext tc;

        AsyncSleep(0).Then([](Result<void>) -> Result<void>
        {
            throw std::bad_alloc();
        })
        .Finally([&tc](Result<void> result) mutable
        {
            tc.Assert(result.hr == E_OUTOFMEMORY); // std::bad_alloc should get translated to E_OUTOFMEMORY
            tc.Complete();
        });

        tc.AwaitResult();
    }
};

} // UnitTests
} // PlayFab
