#include "stdafx.h"
#include "LocalStorage.h"
#include "Platform.h"
#include "XAsyncOperation.h"

namespace PlayFab
{

class LocalStorage::State
{
public:
    template<typename T>
    AsyncOp<T> StorageOperationContinuation(Result<T> operationResult)
    {
        std::unique_lock<std::mutex> lock{ m_mutex };
        m_runningOperation = nullptr;
        RunNextOperationIfPossible(std::move(lock));

        return std::move(operationResult);
    }

    template<>
    AsyncOp<void> StorageOperationContinuation(Result<void> operationResult)
    {
        std::unique_lock<std::mutex> lock{ m_mutex };
        m_runningOperation = nullptr;
        RunNextOperationIfPossible(std::move(lock));

        return std::move(operationResult);
    }

    template<typename TOp>
    auto QueueOperation(UniquePtr<TOp>&& operation) -> decltype(operation->AsAsyncOp())
    {
        std::unique_lock<std::mutex> lock{ m_mutex };

        auto asyncOp = operation->AsAsyncOp();
        m_operationQueue.emplace(operation.release());

        RunNextOperationIfPossible(std::move(lock));

        return asyncOp;
    }

private:
    void RunNextOperationIfPossible(std::unique_lock<std::mutex>&& lock)
    {
        if (!m_runningOperation && !m_operationQueue.empty())
        {
            UniquePtr<IOperation> operation = std::move(m_operationQueue.front());
            m_operationQueue.pop();
            m_runningOperation = operation.get();

            lock.unlock();

            RunOperation(std::move(operation));
        }
    }

    std::mutex m_mutex;
    IOperation* m_runningOperation; // non-owning
    Queue<UniquePtr<IOperation>> m_operationQueue;
};

class ReadOperation : public XAsyncOperation<Vector<uint8_t>>
{
public:
    ReadOperation(String&& path, PlayFab::RunContext&& rc) :
        XAsyncOperation<Vector<uint8_t>>{ Detail::GetLocalStorageHandlers().queueHandle ? rc.DeriveOnQueue(Detail::GetLocalStorageHandlers().queueHandle) : std::move(rc) },
        m_path{ std::move(path) }
    {
    }

private:
    HRESULT OnStarted(XAsyncBlock* asyncBlock) noexcept override
    {
        auto& handlers = Detail::GetLocalStorageHandlers();
        return handlers.read(handlers.context, m_path.data(), asyncBlock);
    }

    Result<Vector<uint8_t>> GetResult(XAsyncBlock* asyncBlock) noexcept override
    {
        size_t resultSize{ 0 };
        RETURN_IF_FAILED(XAsyncGetResultSize(asyncBlock, &resultSize));
        Vector<uint8_t> payload(resultSize);
        RETURN_IF_FAILED(XAsyncGetResult(asyncBlock, nullptr, payload.size(), payload.data(), nullptr));
        return payload;
    }

    String const m_path;
};

class WriteOperation : public XAsyncOperation<void>
{
public:
    WriteOperation(String&& path, Vector<uint8_t>&& data, PlayFab::RunContext&& rc) :
        XAsyncOperation<void>{ Detail::GetLocalStorageHandlers().queueHandle ? rc.DeriveOnQueue(Detail::GetLocalStorageHandlers().queueHandle) : std::move(rc) },
        m_path{ std::move(path) },
        m_data{ std::move(data) }
    {
    }

private:
    HRESULT OnStarted(XAsyncBlock* asyncBlock) noexcept override
    {
        auto& handlers = Detail::GetLocalStorageHandlers();
        return handlers.write(handlers.context, m_path.data(), m_data.size(), m_data.data(), asyncBlock);
    }

    String const m_path;
    Vector<uint8_t> const m_data;
};

class ClearOperation : public XAsyncOperation<void>
{
public:
    ClearOperation(String&& path, PlayFab::RunContext&& rc) : 
        XAsyncOperation<void>{ Detail::GetLocalStorageHandlers().queueHandle ? rc.DeriveOnQueue(Detail::GetLocalStorageHandlers().queueHandle) : std::move(rc) },
        m_path{ std::move(path) }
    {
    }

private:
    HRESULT OnStarted(XAsyncBlock* asyncBlock) noexcept override
    {
        auto& handlers = Detail::GetLocalStorageHandlers();
        return handlers.clear(handlers.context, m_path.data(), asyncBlock);
    }

    String const m_path;
};

class FlushOperation : public IOperation
{
public:
    FlushOperation(PlayFab::RunContext&& rc) :
        IOperation{ std::move(rc) },
        m_asyncContext{ MakeShared<AsyncOpContext<void>>() }
    {
    }

    AsyncOp<void> AsAsyncOp() noexcept
    {
        return m_asyncContext;
    }

private:
    void OnStarted(UniquePtr<IOperation> self) noexcept override
    {
        m_asyncContext->Complete(Result<void>{ S_OK });
    }

    SharedPtr<AsyncOpContext<void>> m_asyncContext;
};


LocalStorage::LocalStorage() : m_state{ MakeShared<State>() }
{
}

AsyncOp<Vector<uint8_t>> LocalStorage::Read(
    String path,
    RunContext runContext
) noexcept
{
    auto operation = MakeUnique<ReadOperation>(
        std::move(path),
        std::move(runContext)
    );

    return m_state->QueueOperation(std::move(operation)).Then(std::bind(&State::StorageOperationContinuation<Vector<uint8_t>>, m_state, std::placeholders::_1));
}

AsyncOp<void> LocalStorage::Write(
    String path,
    Vector<uint8_t>&& data,
    RunContext runContext
) noexcept
{
    auto operation = MakeUnique<WriteOperation>(
        std::move(path),
        std::move(data),
        std::move(runContext)
    );

    return m_state->QueueOperation(std::move(operation)).Then(std::bind(&State::StorageOperationContinuation<void>, m_state, std::placeholders::_1));
}


AsyncOp<void> LocalStorage::Clear(
    String path,
    RunContext runContext
) noexcept
{
    auto operation = MakeUnique<ClearOperation>(
        std::move(path),
        std::move(runContext)
    );

    return m_state->QueueOperation(std::move(operation)).Then(std::bind(&State::StorageOperationContinuation<void>, m_state, std::placeholders::_1));
}

AsyncOp<void> LocalStorage::Flush(
    RunContext runContext
) noexcept
{
    auto operation = MakeUnique<FlushOperation>(std::move(runContext));

    return m_state->QueueOperation(std::move(operation)).Then(std::bind(&State::StorageOperationContinuation<void>, m_state, std::placeholders::_1));
}

}
