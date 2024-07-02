#pragma once

#include <memory>
#include "Result.h"
#include "Error.h"

namespace PlayFab
{

// Forward declarations
template<typename T>
class AsyncOp;

template<typename T>
class AsyncOpContext;

namespace Detail
{

template<typename T>
struct IsAsyncOpHelper : std::false_type {};
template<typename T>
struct IsAsyncOpHelper<AsyncOp<T>> : std::true_type {};
template<typename T>
constexpr bool IsAsyncOp = IsAsyncOpHelper<T>::value;

template<typename T>
struct UnwrapAsyncHelper { typedef T type; };
template<typename T>
struct UnwrapAsyncHelper<AsyncOp<T>> { typedef T type; };
template<typename T>
using UnwrapAsyncT = typename UnwrapAsyncHelper<T>::type;

template<typename T>
struct IsResultHelper : std::false_type {};
template<typename T>
struct IsResultHelper<Result<T>> : std::true_type {};
template<typename T>
constexpr bool IsResult = IsResultHelper<T>::value;

template<typename T>
struct UnwrapResultHelper { typedef T type; };
template<typename T>
struct UnwrapResultHelper<Result<T>> { typedef T type; };
template<typename T>
using UnwrapResultT = typename UnwrapResultHelper<T>::type;

template<typename Func, typename ArgT>
struct ContinuationTraits
{
    typedef typename std::invoke_result_t<Func, ArgT> ContinuationReturnT;
    typedef Detail::UnwrapAsyncT<Detail::UnwrapResultT<ContinuationReturnT>> AsyncOpT;
};

}

// AsyncOp represents an asynchronous piece of work.
//
// An AsyncOp<T> will result in a Result<T>. Consumers should add a continuation function with either AsyncOp::Then
// or AsyncOp::Finally to handle the async result.
//
// The AsyncOp class doesn't implicitly control the thread/TaskQueue of the async operation - that is left to the implementer of
// first-class async operations. Continuation functions will be invoked synchronously on the thread where the antecedent task completed.
// If a continuation requires additional asynchronous work it is the responsibility of that continuation to schedule
// that work to appropriately.
//
// Currently there is no support for synchronously waiting for an AsyncOp to complete.
template<typename T>
class AsyncOp
{
public:
    using ResultT = Result<T>;

    AsyncOp(SharedPtr<AsyncOpContext<T>> context) noexcept;

    // Creates an AsyncOp in a completed state from a result.
    AsyncOp(HRESULT hresult) noexcept;
    AsyncOp(HRESULT hresult, String errorMessage) noexcept;
    AsyncOp(Result<T>&& result) noexcept;

    AsyncOp(const AsyncOp&) = default;
    AsyncOp& operator=(const AsyncOp&) = default;

    // Add a continuation function to be run when the current AsyncOp completes.
    // If the current op is already in a completed state, the continuation will be invoked synchronously.
    template<typename Func>
    auto Then(Func&& continuationFunc) noexcept -> AsyncOp<typename Detail::ContinuationTraits<Func, ResultT>::AsyncOpT>;

    // Add a final continuation function to be run when the current AsyncOp completes.
    // The final continuation should handle all errors from the AsyncOp chain.
    template<typename Func>
    void Finally(Func&& continuationFunc) noexcept;

    // Block and wait for the AsyncOp to complete, returning the result
    ResultT Wait() noexcept;

private:
    SharedPtr<AsyncOpContext<T>> m_context;
};


// Interface for AsyncOp Continuations. The input ArgT to the continuation should be the ResultT of the previous
// async operation.
template<typename ArgT>
struct IContinuation
{
    virtual ~IContinuation() = default;
    virtual void Run(ArgT&& arg) noexcept = 0;
};

// AsyncOpContext serves as a coordinator between the AsyncOp consumer & the AsyncOp implementation. Consumers can add a
// continuation function to be invoked when the operation completes, and AsyncOp implementers provide the result
// of the asynchronous operation when it completes. The AsyncOpContext stores the result/continuation until they are
// needed, and then passing the result along to the continuation when both have been provided. Ownership of the AsyncOpContext
// is shared between AsyncOp objects returned to consumers and AsyncOp implementers.
template<typename T>
class AsyncOpContext
{
public:
    using ResultT = Result<T>;

    AsyncOpContext();
    AsyncOpContext(Result<T>&&);
    AsyncOpContext(const AsyncOpContext&) = delete;
    AsyncOpContext& operator=(const AsyncOpContext&) = delete;
    virtual ~AsyncOpContext();

    // Called by consumers of AsyncOps (via AsyncOp.Then/Finally)
    void SetContinuation(SharedPtr<IContinuation<ResultT>> continuation) noexcept;

    // Called by AsyncOp implementations
    void Complete(ResultT&& result) noexcept;
    void Complete(std::exception_ptr exception) noexcept;

    // Called by final continuation to finalize AsyncOp chain
    void Finalize() noexcept;

private:
    // State of the async operation. Pending indicates that the operation is pending (it can mean that its either
    // not yet started or currently running). Completed indicates that the operation has completed and the result has been 
    // stashed in m_result. Finalized indicates that the result has been passed to the next continuation or that
    // the async operation chain was explicitly finalized with AsyncOp::Finally
    enum class State { Pending, Completed, Finalized } m_operationState;

    std::optional<ResultT> m_result;
    SharedPtr<IContinuation<ResultT>> m_continuation;
    mutable std::mutex m_mutex;
};

// Implementation of a Continuation to an AsyncOp. Func is a Functor that returns either Result<T> or AsyncOp<T>
// If Func returns an AsyncOp, an intermediate callback is inserted to collapse the async op chain.
template<typename Func, typename ArgT>
class Continuation : public IContinuation<ArgT>, public std::enable_shared_from_this<Continuation<Func, ArgT>>
{
public:
    using AsyncOpT = typename Detail::ContinuationTraits<Func, ArgT>::AsyncOpT;
    using FuncReturnT = typename Detail::ContinuationTraits<Func, ArgT>::ContinuationReturnT;

    Continuation(Func&& continuationFunction) noexcept;
    Continuation(const Continuation&) = delete;
    Continuation& operator=(const Continuation&) = delete;
    ~Continuation() = default;

    AsyncOp<AsyncOpT> AsAsyncOp() const noexcept;
    void Run(ArgT&& result) noexcept;

private:
    template<typename T = FuncReturnT>
    typename std::enable_if_t<std::is_void_v<T>> RunHelper(ArgT&& result) noexcept;

    template<typename T = FuncReturnT>
    typename std::enable_if_t<Detail::IsAsyncOp<T>> RunHelper(ArgT&& result) noexcept;

    template<typename T = FuncReturnT>
    typename std::enable_if_t<!std::is_void_v<T> && !Detail::IsAsyncOp<T>> RunHelper(ArgT&& result) noexcept;

    Func m_function;
    SharedPtr<AsyncOpContext<AsyncOpT>> const m_asyncContext;
};

//------------------------------------------------------------------------------
// Template implementations
//------------------------------------------------------------------------------

// AsyncOp<T> Implementation
template<typename T>
AsyncOp<T>::AsyncOp(SharedPtr<AsyncOpContext<T>> context) noexcept
    : m_context{ std::move(context) }
{
}

template<typename T>
AsyncOp<T>::AsyncOp(HRESULT hresult) noexcept
    : m_context{ MakeShared<AsyncOpContext<T>>(Result<T>{ hresult }) }
{
}

template<typename T>
AsyncOp<T>::AsyncOp(HRESULT hresult, String errorMessage) noexcept
    : m_context{ MakeShared<AsyncOpContext<T>>(Result<T>{ hresult, std::move(errorMessage) }) }
{
}

template<typename T>
AsyncOp<T>::AsyncOp(Result<T>&& result) noexcept
    : m_context{ MakeShared<AsyncOpContext<T>>(std::move(result)) }
{
}

template<typename T> template<typename Func>
auto AsyncOp<T>::Then(Func&& continuationFunc) noexcept -> AsyncOp<typename Detail::ContinuationTraits<Func, ResultT>::AsyncOpT>
{
    static_assert(Detail::IsResult<typename std::invoke_result_t<Func, ResultT>> || Detail::IsAsyncOp<typename std::invoke_result_t<Func, ResultT>>, "Continuation functions must return either Result<T> or AsyncOp<T>");

    auto continuation = MakeShared<Continuation<Func, ResultT>>(std::move(continuationFunc));
    m_context->SetContinuation(continuation);
    return continuation->AsAsyncOp();
}

template<typename T> template<typename Func>
void AsyncOp<T>::Finally(Func&& continuationFunc) noexcept
{
    static_assert(std::is_void<typename std::invoke_result_t<Func, ResultT>>::value, "Final continuation may not return a value");

    auto continuation = MakeShared<Continuation<Func, ResultT>>(std::move(continuationFunc));
    m_context->SetContinuation(continuation);
}

template<typename T>
typename AsyncOp<T>::ResultT AsyncOp<T>::Wait() noexcept
{
    std::mutex mutex;
    std::condition_variable waitCondition;
    bool operationComplete{ false };
    std::optional<ResultT> asyncResult;

    this->Finally([&](ResultT&& result)
    {
        std::lock_guard<std::mutex> lock{ mutex };
        asyncResult.emplace(std::move(result));
        operationComplete = true;
        waitCondition.notify_one();
    });

    std::unique_lock<std::mutex> lock{ mutex };
    if (!operationComplete)
    {
        waitCondition.wait(lock, [&operationComplete] { return operationComplete; });
    }

    assert(asyncResult.has_value());
    return std::move(*asyncResult);
}

// AsyncOpContext<T> Implementation
template<typename T>
AsyncOpContext<T>::AsyncOpContext()
    : m_operationState{ State::Pending }
{
}

template<typename T>
AsyncOpContext<T>::AsyncOpContext(Result<T>&& result)
    : m_operationState{ State::Completed },
    m_result{ std::move(result) }
{
}

template<typename T>
AsyncOpContext<T>::~AsyncOpContext()
{
    // Sanity check. All async ops should complete and pass the result along
    assert(m_operationState == State::Finalized);
}

template<typename T>
void AsyncOpContext<T>::SetContinuation(SharedPtr<IContinuation<ResultT>> continuation) noexcept
{
    std::lock_guard<std::mutex> lock{ m_mutex };

    // Don't allow multiple continuations to a single async operation. Logically this is like a function returning to two places and
    // it's extremely hard to reason about & debug. It also creates issues with transferring ownership of the Result object
    // to the continuation, etc.
    assert(!m_continuation);

    switch (m_operationState)
    {
    case State::Pending:
    {
        m_continuation = continuation;
        break;
    }
    case State::Completed:
    {
        continuation->Run(std::move(*m_result));
        m_operationState = State::Finalized;
        break;
    }
    case State::Finalized:
    default:
    {
        assert(false);
    }
    }
}

template<typename T>
void AsyncOpContext<T>::Complete(ResultT&& result) noexcept
{
    std::lock_guard<std::mutex> lock{ m_mutex };
    assert(m_operationState == State::Pending);

    if (m_continuation)
    {
        m_continuation->Run(std::move(result));
        m_operationState = State::Finalized;
    }
    else
    {
        m_result.emplace(std::move(result));
        m_operationState = State::Completed;
    }
}

template<typename T>
void AsyncOpContext<T>::Complete(std::exception_ptr) noexcept
{
    Complete(Result<T>{ CurrentExceptionToHR(), "Unhandled exception during async operation. Propogating to AsyncOp continuation." });
}

template<typename T>
void AsyncOpContext<T>::Finalize() noexcept
{
    std::lock_guard<std::mutex> lock{ m_mutex };
    assert(m_operationState == State::Pending);
    assert(!m_continuation);

    m_operationState = State::Finalized;
}

// Continuation implementation
template<typename Func, typename ArgT>
Continuation<Func, ArgT>::Continuation(Func&& continuationFunction) noexcept
    : m_function{ std::move(continuationFunction) },
    m_asyncContext{ MakeShared<AsyncOpContext<AsyncOpT>>() }
{
}

template<typename Func, typename ArgT>
AsyncOp<typename Continuation<Func, ArgT>::AsyncOpT> Continuation<Func, ArgT>::AsAsyncOp() const noexcept
{
    return AsyncOp<AsyncOpT>{ m_asyncContext };
}

template<typename Func, typename ArgT>
void Continuation<Func, ArgT>::Run(ArgT&& result) noexcept
{
    RunHelper(std::move(result));
}

template<typename Func, typename ArgT> template<typename T>
typename std::enable_if_t<std::is_void_v<T>> Continuation<Func, ArgT>::RunHelper(ArgT&& result) noexcept
{
    // If func doesn't return a value, finalize the async operation chain.
    m_function(std::move(result));
    m_asyncContext->Finalize();
}

template<typename Func, typename ArgT> template<typename T>
typename std::enable_if_t<Detail::IsAsyncOp<T>> Continuation<Func, ArgT>::RunHelper(ArgT&& result) noexcept
{
    try
    {
        // If func returns an AsyncOp, insert an intermediate continuation to grab its result
        m_function(std::move(result)).Finally([sharedThis = this->shared_from_this()](auto r)
        {
            sharedThis->m_asyncContext->Complete(std::move(r));
        });
    }
    catch (std::exception& ex)
    {
        m_asyncContext->Complete({ CurrentExceptionToHR(), ex.what() });
    }
    catch (...)
    {
        m_asyncContext->Complete(std::current_exception());
    }
}

template<typename Func, typename ArgT> template<typename T>
typename std::enable_if_t<!std::is_void_v<T> && !Detail::IsAsyncOp<T>> Continuation<Func, ArgT>::RunHelper(ArgT&& result) noexcept
{
    try
    {
        m_asyncContext->Complete(m_function(std::move(result)));
    }
    catch (std::exception& ex)
    {
        m_asyncContext->Complete({ CurrentExceptionToHR(), ex.what() });
    }
    catch (...)
    {
        m_asyncContext->Complete(std::current_exception());
    }
}

}
