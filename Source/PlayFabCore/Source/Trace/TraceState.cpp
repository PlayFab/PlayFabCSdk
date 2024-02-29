#include "stdafx.h"
#include "TraceState.h"
#include "JsonUtils.h"
#include "Platform/LocalStorage.h"
#include "LocalStorageTraceOutput.h"
#include "DebugTraceOutput.h"

namespace PlayFab
{

static constexpr char s_configFile[] = "PlayFabTraceSettings.json";

TraceSettings& GetTraceSettings()
{
    static TraceSettings s_settings;
    return s_settings;
}

enum class AccessMode
{
    Initialize,
    Get,
    Cleanup
};

HRESULT AccessTraceState(AccessMode mode, SharedPtr<TraceState>& traceState)
{
    struct TraceStateHolder
    {
        SharedPtr<TraceState> const state;
    };

    static std::atomic<TraceStateHolder*> s_traceStateHolder{ nullptr };
    assert(s_traceStateHolder.is_lock_free());

    switch (mode)
    {
    case AccessMode::Initialize:
    {
        RETURN_HR_IF(E_PF_CORE_ALREADY_INITIALIZED, s_traceStateHolder.load() != nullptr);
        RETURN_HR_INVALIDARG_IF_NULL(traceState);

        UniquePtr<TraceStateHolder> stateHolder{ new (Allocator<TraceStateHolder>{}.allocate(1)) TraceStateHolder{ traceState } };

        TraceStateHolder* expected{ nullptr };
        if (!s_traceStateHolder.compare_exchange_strong(expected, stateHolder.get()))
        {
            return E_PF_CORE_ALREADY_INITIALIZED;
        }

        stateHolder.release(); // reclaimed via AccessMode::Cleanup

        return S_OK;
    }
    case AccessMode::Get:
    {
        TraceStateHolder* traceStateHolder = s_traceStateHolder.load();

        RETURN_HR_IF(E_PF_CORE_NOT_INITIALIZED, !traceStateHolder);
        assert(traceStateHolder->state);
        traceState = traceStateHolder->state;

        return S_OK;
    }
    case AccessMode::Cleanup:
    {
        UniquePtr<TraceStateHolder> traceStateHolder{ s_traceStateHolder.exchange(nullptr) };

        RETURN_HR_IF(E_PF_CORE_NOT_INITIALIZED, !traceStateHolder);
        traceState = traceStateHolder->state;

        return S_OK;
    }
    default:
    {
        assert(false);
        return E_UNEXPECTED;
    }
    }
}

TraceState::TraceState(RunContext&& runContext, LocalStorage localStorage) noexcept :
    m_runContext{ std::move(runContext) }
{
    // Init LibHttpClient Tracing
    HCTraceInit();
#ifdef _DEBUG
    HCSettingsSetTraceLevel(HCTraceLevel::Verbose);
#endif
#if HC_PLATFORM_IS_MICROSOFT
    HCTraceSetEtwEnabled(true);
#endif
    HCTraceSetClientCallback(TraceCallback);

    // Synchronously load trace settings. TraceSettings loaded from file override those configured via API
    // TODO document location, name, and format of TraceSettings file. It should just be a Json serialized
    // version of the TraceSettings struct:
    // {
    //   "enableTraceToFile": true,
    //   "traceFileDirectory": "",
    //   "traceToDebugger": false
    // }
    // If the trace settings file is not present or is malformed, it will simply be ignored and default values
    // will be used for TraceSettings.

    TraceSettings& settings = GetTraceSettings();

    Result<Vector<uint8_t>> readResult = localStorage.Read(s_configFile, m_runContext).Wait();
    if (SUCCEEDED(readResult.hr))
    {
        auto fileData{ readResult.ExtractPayload() };

        JsonDocument fileJson{ &JsonUtils::allocator, JsonUtils::kDefaultStackCapacity, &JsonUtils::allocator };
        fileJson.Parse(reinterpret_cast<char*>(fileData.data()), fileData.size());
        if (!fileJson.HasParseError())
        {
            JsonUtils::ObjectGetMember(fileJson, "enableTraceToFile", settings.enableTraceToFile);
            JsonUtils::ObjectGetMember(fileJson, "traceToDebugger", settings.traceToDebugger);
            String traceFileDirectory;
            if (SUCCEEDED(JsonUtils::ObjectGetMember(fileJson, "traceFileDirectory", traceFileDirectory)))
            {
                StrCpy(settings.traceFileDirectory, sizeof(settings.traceFileDirectory), traceFileDirectory.data());
            }
        }
    }

    // Add appropriate TraceOutputs based on TraceSettings and Platform
    if (settings.enableTraceToFile)
    {
        m_outputs.emplace_back(MakeShared<LocalStorageTraceOutput>(m_runContext.Derive(), std::move(localStorage), settings.traceFileDirectory));
    }
    if (settings.traceToDebugger)
    {
        m_outputs.emplace_back(MakeShared<DebugTraceOutput>());
    }
}

TraceState::~TraceState() noexcept
{
    // Intentionally skip calling HCTraceSetClientCallback(nullptr) here. If PFInitialize is called twice,
    // the second call will fail, but not before creating a second TraceState oject. When the second TraceState
    // object is destroyed, it will reset the static callback used by the original TraceState instance.

    HCTraceCleanup();
}

HRESULT TraceState::Create(RunContext&& runContext, LocalStorage localStorage) noexcept
{
    Allocator<TraceState> a{};
    SharedPtr<TraceState> traceState{ new (a.allocate(1)) TraceState{ std::move(runContext), std::move(localStorage) }, Deleter<TraceState>{}, a };

    return AccessTraceState(AccessMode::Initialize, traceState);
}

HRESULT TraceState::Get(SharedPtr<TraceState>& state) noexcept
{
    return AccessTraceState(AccessMode::Get, state);
}

struct CleanupContext
{
    SharedPtr<TraceState> traceState;
    XAsyncBlock* asyncBlock;
};

HRESULT TraceState::CleanupAsync(XAsyncBlock* async) noexcept
{
    try
    {
        UniquePtr<CleanupContext> context = MakeUnique<CleanupContext>();
        RETURN_IF_FAILED(XAsyncBegin(async, context.get(), __FUNCTION__, __FUNCTION__, CleanupAsyncProvider));
        context.release();
        return S_OK;
    }
    catch (...)
    {
        return CurrentExceptionToHR();
    }
}

HRESULT CALLBACK TraceState::CleanupAsyncProvider(XAsyncOp op, XAsyncProviderData const* data) noexcept
{
    static constexpr uint32_t s_pollDelayMs{ 10 };
    CleanupContext* context{ static_cast<CleanupContext*>(data->context) };

    switch (op)
    {
    case XAsyncOp::Begin:
    {
        // After this no other thread will be able to access TraceState if they don't already have a reference to it
        RETURN_IF_FAILED(AccessTraceState(AccessMode::Cleanup, context->traceState));
        context->asyncBlock = data->async;

        return XAsyncSchedule(data->async, 0);
    }
    case XAsyncOp::DoWork:
    {
        // If there aren't any other remaining reference to TraceState, Terminate the RunContext and destroy it. Otherwise
        // wait until we have the last remaining reference
        if (context->traceState.use_count() > 1)
        {
            RETURN_IF_FAILED(XAsyncSchedule(data->async, s_pollDelayMs));
            return E_PENDING;
        }

        // Give TraceOutputs a chance to finish naturally before terminating RunContext. Terminating RunContext immediately may lead to 
        // lost trace messages in some cases
        for (auto& output : context->traceState->m_outputs)
        {
            output->Stop();
        }

        // Forcibily terminate any remaining work
        context->traceState->m_runContext.Terminate(*context->traceState, context);
        return E_PENDING;
    }
    default:
    {
        return S_OK;
    }
    }
}

void TraceState::OnTerminated(void* c) noexcept
{
    UniquePtr<CleanupContext> context{ static_cast<CleanupContext*>(c) };
    XAsyncBlock* asyncBlock{ context->asyncBlock }; // Keep copy of asyncBlock pointer to complete after cleaning up context

    // Cleanup context. TraceState will be destroyed here
    context.reset();

    // TraceState fully cleaned up!
    XAsyncComplete(asyncBlock, S_OK, 0);
}

// Trace helpers
template<size_t SIZE>
int stprintf_s(char(&buffer)[SIZE], _Printf_format_string_ char const* format ...) noexcept
{
    va_list varArgs{};
    va_start(varArgs, format);
    auto result = vsnprintf(buffer, SIZE, format, varArgs);
    va_end(varArgs);
    return result;
}

int stprintf_s(char* buffer, size_t size, _Printf_format_string_ char const* format ...) noexcept
{
    va_list varArgs{};
    va_start(varArgs, format);
    auto result = vsnprintf(buffer, size, format, varArgs);
    va_end(varArgs);
    return result;
}

template<size_t SIZE>
int vstprintf_s(char(&buffer)[SIZE], _Printf_format_string_ char const* format, va_list varArgs) noexcept
{
    return vsnprintf(buffer, SIZE, format, varArgs);
}

String FormatString(_In_z_ _Printf_format_string_ const char* format, ...)
{
    va_list args1;
    va_start(args1, format);

    va_list args2;
    va_copy(args2, args1);

    Vector<char> buffer(1 + std::vsnprintf(NULL, 0, format, args1));
    va_end(args1);
    
    std::vsnprintf(buffer.data(), buffer.size(), format, args2);
    va_end(args2);
    
    String strBuffer(buffer.data(), buffer.size());
    
    return strBuffer;
}

void CALLBACK TraceState::TraceCallback(
    _In_z_ const char* areaName,
    _In_ HCTraceLevel level,
    _In_ uint64_t threadId,
    _In_ uint64_t timestamp,
    _In_z_ const char* message
) noexcept
{
    SharedPtr<TraceState> state;
    TraceState::Get(state);
    if (!state)
    {
        return;
    }
    
    TraceSettings& settings = GetTraceSettings();
    if (settings.traceCallback)
    {
        settings.traceCallback(areaName, level, threadId, timestamp, message);
    }
    
    if (state->m_outputs.empty())
    {
        // Early out if we have no outputs
        return;
    }

    // Needs to match the HCTraceLevel enum
    static char const* traceLevelNames[] =
    {
        "Off",
        "E",
        "W",
        "P",
        "I",
        "V",
    };

    std::time_t timeTInSec = static_cast<std::time_t>(timestamp / 1000);
    uint32_t fractionMSec = static_cast<uint32_t>(timestamp % 1000);
    std::tm fmtTime = {};

#if _WIN32
    localtime_s(&fmtTime, &timeTInSec);
#elif HC_PLATFORM_IS_PLAYSTATION
    localtime_s(&timeTInSec, &fmtTime);
#else
    localtime_r(&timeTInSec, &fmtTime);
#endif
    
    String formattedMessage = FormatString("[%04llX][%s][%02d:%02d:%02d.%03u][%s] %s\r\n",
        threadId,
        traceLevelNames[static_cast<size_t>(level)],
        fmtTime.tm_hour,
        fmtTime.tm_min,
        fmtTime.tm_sec,
        fractionMSec,
        areaName,
        message
        );
    
    auto& outputs = state->m_outputs;
    for (auto& output : outputs)
    {
        output->TraceMessage(formattedMessage.c_str());
    }
}

}
