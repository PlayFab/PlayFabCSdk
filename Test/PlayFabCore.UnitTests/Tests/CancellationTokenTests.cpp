#include "stdafx.h"
#include "TestIncludes.h"
#include "AsyncOp.h"
#include "CancellationToken.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PlayFab
{
namespace UnitTests
{

AsyncOp<void> AwaitCancelAsync(CancellationToken ct)
{
    struct AwaitCancelOperation : ICancellationListener
    {
        AwaitCancelOperation(CancellationToken _ct) :
            ct{ std::move(_ct) },
            opContext{ std::make_shared<AsyncOpContext<void>>() }
        {
            bool cancelled = ct.RegisterForNotificationAndCheck(*this);
            if (cancelled)
            {
                opContext->Complete(E_ABORT);
            }
        }

        void OnCancellation() noexcept override
        {
            std::unique_ptr<AwaitCancelOperation> reclaim{ this };
            opContext->Complete(E_ABORT);
        }

        CancellationToken ct;
        std::shared_ptr<AsyncOpContext<void>> opContext;
    };
    
    auto op = std::make_unique<AwaitCancelOperation>(std::move(ct)).release();

    return op->opContext;
}

TEST_CLASS(CancellationTokenTests)
{
public:
    TEST_METHOD(BasicCancellation)
    {
        AsyncTestContext tc;
        auto ct = CancellationToken::Root();

        AwaitCancelAsync(ct).Finally([&tc](Result<void> result)
        {
            tc.Assert(result.hr == E_ABORT);
            tc.Complete();
        });

        ct.Cancel();
        tc.AwaitResult();
    }

    TEST_METHOD(DerivedTokenCancelledByParent)
    {
        AsyncTestContext tc;
        auto ct = CancellationToken::Root();

        AwaitCancelAsync(ct.Derive()).Finally([&tc](Result<void> result)
        {
            tc.Assert(result.hr == E_ABORT);
            tc.Complete();
        });

        ct.Cancel();
        tc.AwaitResult();
    }

    TEST_METHOD(CancelMultipleOperationsWithParentToken)
    {
        AsyncTestContext tc;
        auto rootToken = CancellationToken::Root();
        CancellationToken derivedToken{ rootToken.Derive() };

        std::atomic<size_t> opsCompleted{ 0 };
        auto continuation = [&](Result<void> result)
        {
            tc.Assert(result.hr == E_ABORT);
            auto completedCount = ++opsCompleted;
            if (completedCount == 3)
            {
                tc.Complete();
            }
        };

        // Start 3 operations using derived tokens
        AwaitCancelAsync(derivedToken).Finally([&](Result<void> result)
        {
            continuation(std::move(result));
        });
        AwaitCancelAsync(derivedToken.Derive()).Finally([&](Result<void> result)
        {
            continuation(std::move(result));
        });
        AwaitCancelAsync(derivedToken.Derive().Derive()).Finally([&](Result<void> result)
        {
            continuation(std::move(result));
        });

        rootToken.Cancel();
        tc.AwaitResult();
    }

    TEST_METHOD(PreCancelledOperation)
    {
        AsyncTestContext tc;
        auto ct = CancellationToken::Root();
        ct.Cancel();

        AwaitCancelAsync(ct).Finally([&](Result<void> result)
        {
            tc.Assert(result.hr == E_ABORT);
            tc.Complete();
        });

        tc.AwaitResult();
    }
};

} // UnitTests
} // PlayFab
