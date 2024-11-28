#include "stdafx.h"
#include "TestIncludes.h"
#include "TaskQueue.h"
#include "RunContext.h"
#include "ApiXAsyncProvider.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PlayFab
{
namespace UnitTests
{

enum class EventType
{
    TaskQueueWorkComplete,
    TaskQueueWorkCancelled,
    TaskQueueWorkDestroyed,
    XAsyncProviderCompleted,
    XAsyncProviderAborted,
    XAsyncProviderDestroyed,
    XAsyncOperationComplete,
    TerminationComplete
};

// Simple thread safe list of events that have occurred during a test
class EventList
{
public:
    EventList() : m_state{ std::make_shared<SharedState>() }
    {
        static const size_t kEventListInitialSize = 10;
        m_state->m_events.reserve(kEventListInitialSize);
    }

    void PushBack(EventType event)
    {
        std::lock_guard<std::mutex> lock{ m_state->m_mutex };
        m_state->m_events.push_back(event);
    }

    size_t Size() const
    {
        std::lock_guard<std::mutex> lock{ m_state->m_mutex };
        return m_state->m_events.size();
    }

    EventType operator[](size_t index) const
    {
        std::lock_guard<std::mutex> lock{ m_state->m_mutex };
        return m_state->m_events[index];
    }

    size_t Find(EventType event) const
    {
        std::lock_guard<std::mutex> lock{ m_state->m_mutex };
        for (size_t i = 0; i < m_state->m_events.size(); ++i)
        {
            if (m_state->m_events[i] == event)
            {
                return i;
            }
        }
        return std::numeric_limits<size_t>::max();
    }

    bool Has(EventType event) const
    {
        return Find(event) != std::numeric_limits<size_t>::max();
    }

private:
    struct SharedState
    {
        mutable std::mutex m_mutex; // consider a fair mutex here
        std::vector<EventType> m_events;
    };

    std::shared_ptr<SharedState> m_state;
};

// Default TaskQueue Work timeout if not terminated/cancelled.
// Don't want a number to large so tests don't hang if cancellation/termiantion doesn't work as expected
static constexpr uint32_t kDefaultWorkTimeout{ 10000 }; // 10 seconds in MS

class CancellableWork : public ITaskQueueWork, public ICancellationListener
{
public:
    static void Run(RunContext&& rc, Event* completionEvent, EventList eventList, uint32_t timeout = kDefaultWorkTimeout)
    {
        std::shared_ptr<CancellableWork> work{ new CancellableWork(std::move(rc), completionEvent, eventList) };
        work->m_rc.TaskQueueSubmitWork(work, timeout);
    }

    ~CancellableWork()
    {
        m_rc.CancellationToken().UnregisterForNotificationAndCheck(*this);
        m_eventList.PushBack(EventType::TaskQueueWorkDestroyed);
    }

private:
    CancellableWork(RunContext&& rc, Event* completionEvent, EventList eventList) :
        m_rc{ std::move(rc) },
        m_completionEvent{ completionEvent },
        m_eventList{ std::move(eventList) }
    {
        bool cancelled = m_rc.CancellationToken().RegisterForNotificationAndCheck(*this);
        Assert::IsFalse(cancelled);
    }

    void Run() override
    {
        m_eventList.PushBack(EventType::TaskQueueWorkComplete);
        if (m_completionEvent)
        {
            m_completionEvent->Set();
        }
    }

    void WorkCancelled()
    {
        m_eventList.PushBack(EventType::TaskQueueWorkCancelled);
        if (m_completionEvent)
        {
            m_completionEvent->Set();
        }
    }

    void OnCancellation() noexcept override
    {
        // Cancel submitted work by terminating the TaskQueue
        m_rc.TaskQueueTerminate();
    }

    RunContext m_rc;
    Event* const m_completionEvent;
    EventList m_eventList;
};

class UncancellableWork : public ITaskQueueWork
{
public:
    static void Run(RunContext&& rc, Event* completionEvent, EventList eventList, uint32_t timeout = kDefaultWorkTimeout)
    {
        std::shared_ptr<UncancellableWork> op{ new UncancellableWork(completionEvent, eventList) };
        rc.TaskQueueSubmitWork(op, timeout);
    }

    ~UncancellableWork()
    {
        m_eventList.PushBack(EventType::TaskQueueWorkDestroyed);
    }

private:
    UncancellableWork(Event* completionEvent, EventList eventList) :
        m_completionEvent{ completionEvent },
        m_eventList{ std::move(eventList) }
    {
    }

    void Run() override
    {
        TRACE_VERBOSE(__FUNCTION__);

        m_eventList.PushBack(EventType::TaskQueueWorkComplete);
        if (m_completionEvent)
        {
            m_completionEvent->Set();
        }
    }

    void WorkCancelled()
    {
        TRACE_VERBOSE(__FUNCTION__);

        Sleep(1000);

        m_eventList.PushBack(EventType::TaskQueueWorkComplete);
        if (m_completionEvent)
        {
            m_completionEvent->Set();
        }
    }

    Event* const m_completionEvent;
    EventList m_eventList;
};

class TerminationListener: public ITerminationListener
{
public:
    TerminationListener(EventList eventList) : m_eventList{ std::move(eventList) }
    {
    }

    Event const& TerminationEvent() const
    {
        return m_terminationEvent;
    }

private:
    void OnTerminated(void* /*context*/) override
    {
        m_eventList.PushBack(EventType::TerminationComplete);
        m_terminationEvent.Set();
    }

    Event m_terminationEvent;
    EventList m_eventList;
};

class TestProvider : public XAsyncProviderBase
{
public:
    static constexpr uint32_t kOperationResult = 1337; // arbitrary result value for successful operation

    TestProvider(RunContext&& rc, XAsyncBlock* async, EventList* eventList, uint32_t timeout = kDefaultWorkTimeout) :
        XAsyncProviderBase{ std::move(rc), async },
        m_eventList{ eventList },
        m_timeout{ timeout }
    {
    }

    ~TestProvider()
    {
        if (m_eventList)
        {
            m_eventList->PushBack(EventType::XAsyncProviderDestroyed);
        }
    }

    HRESULT Begin(RunContext rc) override
    {
        rc.TaskQueueSubmitWork(std::shared_ptr<Work>{new Work{ *this }}, m_timeout);
        return S_OK;
    }

    HRESULT GetResult(void* buffer, size_t bufferSize)
    {
        assert(bufferSize == sizeof(uint32_t));
        UNREFERENCED_PARAMETER(bufferSize);

        uint32_t* resultPtr = static_cast<uint32_t*>(buffer);
        *resultPtr = kOperationResult;
        return S_OK;
    }

private:
    struct Work : public ITaskQueueWork
    {
        Work(TestProvider& p) : provider{ p } {}
        void Run() override
        {
            provider.m_eventList->PushBack(EventType::XAsyncProviderCompleted);
            provider.Complete(sizeof(uint32_t));
        }
        void WorkCancelled() override
        {
            provider.m_eventList->PushBack(EventType::XAsyncProviderAborted);
            provider.Fail(E_ABORT);
        }
        TestProvider& provider;
    };

    EventList* m_eventList;
    uint32_t m_timeout;
};


TEST_CLASS(RunContextTests)
{
public:
    TEST_CLASS_INITIALIZE(Initialize)
    {
        HCTraceInit();
        HCSettingsSetTraceLevel(HCTraceLevel::Verbose);
        HCTraceSetTraceToDebugger(true);
    }

    TEST_CLASS_CLEANUP(Cleanup)
    {
        HCTraceCleanup();
    }

    TEST_METHOD(TerminateSimple)
    {
        EventList events;

        // Trivially terminate RunContext
        RunContext root = RunContext::Root(nullptr);
        TerminationListener listener{ events };
        root.Terminate(listener, nullptr);

        Assert::IsTrue(listener.TerminationEvent().Wait());
        Assert::IsTrue(events.Size() == 1);
        Assert::IsTrue(events[0] == EventType::TerminationComplete);
    }

    TEST_METHOD(TestCancellableWork)
    {
        Event completionEvent;
        EventList events;
        RunContext root = RunContext::Root(nullptr);

        // Run work and immediately cancel
        CancellableWork::Run(root.Derive(), &completionEvent, events);
        root.CancellationToken().Cancel();

        Assert::IsTrue(completionEvent.Wait());
        Assert::IsTrue(events.Size() >= 1); // TaskQueueWorkDestroyed could also happen, but isn't guaranteed without a terminate
        Assert::IsTrue(events[0] == EventType::TaskQueueWorkCancelled);
    }

    TEST_METHOD(TestUncancellableWork)
    {
        Event completionEvent;
        EventList events;
        RunContext root = RunContext::Root(nullptr);

        // Attempt to cancel uncancellable work
        constexpr uint32_t timeout = 1000; // use a short timeout since this should run until timeout
        UncancellableWork::Run(root.DeriveOnQueue(nullptr), &completionEvent, events, timeout);
        root.CancellationToken().Cancel();

        Assert::IsTrue(completionEvent.Wait());
        Assert::IsTrue(events.Size() >= 1); // TaskQueueWorkDestroyed could also happen, but isn't guaranteed without a terminate
        Assert::IsTrue(events[0] == EventType::TaskQueueWorkComplete);
    }

    TEST_METHOD(TestTerminateWithUncancellableWork)
    {
        EventList events;
        RunContext root = RunContext::Root(nullptr);
        TerminationListener listener{ events };

        // Terminate uncancellable work
        UncancellableWork::Run(root.DeriveOnQueue(nullptr), nullptr, events);
        root.Terminate(listener, nullptr);

        Assert::IsTrue(listener.TerminationEvent().Wait());
        Assert::AreEqual((size_t)3, events.Size());
        // Termination should guarantee everything is destroyed before completing
        Assert::IsTrue(events[0] == EventType::TaskQueueWorkComplete);
        Assert::IsTrue(events[1] == EventType::TaskQueueWorkDestroyed);
        Assert::IsTrue(events[2] == EventType::TerminationComplete);
    }

    TEST_METHOD(TestTerminateWithInProgressXAsyncOperation)
    {
        EventList events;
        RunContext rc = RunContext::Root(nullptr);
        TerminationListener listener{ events };

        struct XAsyncContext
        {
            EventList& events;
            Event completionEvent;
            HRESULT hr{ E_PENDING };
            uint32_t result{ 0 };

            static void CALLBACK CompletionCallback(XAsyncBlock* async)
            {
                auto context = static_cast<XAsyncContext*>(async->context);
                context->events.PushBack(EventType::XAsyncOperationComplete);
                context->hr = XAsyncGetResult(async, nullptr, sizeof(uint32_t), &context->result, nullptr);
                context->completionEvent.Set();
                Sleep(1000);
            }
        } context{ events };

        XAsyncBlock async{ nullptr, &context, XAsyncContext::CompletionCallback, 0 };
        auto provider = MakeUnique<TestProvider>(rc.DeriveOnQueue(nullptr), &async, &events, 1000000);
        VERIFY_SUCCEEDED(XAsyncProviderBase::Run(std::move(provider)));

        // Terminate
        rc.Terminate(listener, nullptr);

        Assert::IsTrue(listener.TerminationEvent().Wait());
        // SDK only guarantees the clients completion callback is submitted to the TaskQueue before termintaion. If the call is made
        // on its on TaskQueue (or a threadpool queue) the callback may not be invoked until after termination. This isn't
        // necessarily and issue; its possible we can clean up the provider cleanup can happen before the callback runs.
        Assert::IsTrue(context.completionEvent.Wait());

        Assert::AreEqual((size_t)4, events.Size());
        Assert::IsTrue(context.hr == E_ABORT);
        // Provider should always complete the operation first
        Assert::IsTrue(events[0] == EventType::XAsyncProviderAborted);
        // All cleanup should be done before termination completes
        Assert::IsTrue(events.Has(EventType::TerminationComplete) && events.Find(EventType::XAsyncProviderDestroyed) < events.Find(EventType::TerminationComplete));
        Assert::IsTrue(events.Has(EventType::XAsyncOperationComplete));
    }

    TEST_METHOD(TestTerminateWithCompletedXAsyncOperationWithPendingResult)
    {
        EventList events;
        RunContext rc = RunContext::Root(nullptr);
        TerminationListener listener{ events };

        constexpr uint32_t timeout = 2000; // use a short timeout since this should run until timeout
        XAsyncBlock async{ nullptr, nullptr, nullptr, 0 };
        auto provider = MakeUnique<TestProvider>(rc.DeriveOnQueue(nullptr), &async, &events, timeout);
        VERIFY_SUCCEEDED(XAsyncProviderBase::Run(std::move(provider)));

        // Wait for XAsync operation to complete but don't extract result
        HRESULT hr = XAsyncGetStatus(&async, true);
        VERIFY_SUCCEEDED(hr);

        // Terminate
        rc.Terminate(listener, nullptr);

        Assert::IsFalse(listener.TerminationEvent().Wait(1000)); // Terminate shouldn't complete because XAsync result is pending
        Assert::IsTrue(events.Size() == 1);
        Assert::IsTrue(events[0] == EventType::XAsyncProviderCompleted);
    }

};

} // UnitTests
} // PlayFab
