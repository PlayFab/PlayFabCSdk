#include "stdafx.h"

#if HC_PLATFORM == HC_PLATFORM_GDK

#include "LocalUser.h"
#include <ApiXAsyncProvider.h>
#include <playfab/core/cpp/AuthenticationTypeWrappers.h>
#include "Generated/Authentication.h"
#include "JsonUtils.h"
#include "steam/steam_api.h"
#include "LocalUser_Steam.h"

namespace PlayFab
{

typedef ISteamUser* (*SteamAPI_SteamUser_v023_t)();
typedef CSteamID(*SteamAPI_ISteamUser_GetSteamID_t)(ISteamUser* steamUser);
typedef bool (*SteamAPI_IsSteamRunning_t)();
typedef bool (*SteamAPI_ISteamUser_BLoggedOn_t)(ISteamUser* steamUser);
typedef HAuthTicket(*SteamAPI_ISteamUser_GetAuthTicketForWebApi_t)(ISteamUser* steamUser, const char* pchIdentity);
typedef void (*SteamAPI_RegisterCallback_t)(CCallbackBase* pCallback, int iCallback);
typedef void (*SteamAPI_UnregisterCallback_t)(CCallbackBase* pCallback);
typedef void (*SteamAPI_ISteamUser_CancelAuthTicket_t)(ISteamUser* self, HAuthTicket hAuthTicket);

class SteamAPIFunctions
{
public:
    SteamAPIFunctions();
    ~SteamAPIFunctions();

    HMODULE hSteamAPI{ nullptr };
    bool isSteamAPILoaded{ false };
    SteamAPI_SteamUser_v023_t SteamAPI_SteamUser{ nullptr };
    SteamAPI_ISteamUser_GetSteamID_t SteamAPI_ISteamUser_GetSteamID{ nullptr };
    SteamAPI_IsSteamRunning_t SteamAPI_IsSteamRunning{ nullptr };
    SteamAPI_ISteamUser_BLoggedOn_t SteamAPI_ISteamUser_BLoggedOn{ nullptr };
    SteamAPI_ISteamUser_GetAuthTicketForWebApi_t SteamAPI_ISteamUser_GetAuthTicketForWebApi{ nullptr };
    SteamAPI_RegisterCallback_t SteamAPI_RegisterCallback{ nullptr };
    SteamAPI_UnregisterCallback_t SteamAPI_UnregisterCallback{ nullptr };
    SteamAPI_ISteamUser_CancelAuthTicket_t SteamAPI_ISteamUser_CancelAuthTicket{ nullptr };
};

SteamAPIFunctions::SteamAPIFunctions()
{
    hSteamAPI = LoadLibraryA("steam_api64.dll");
    if (hSteamAPI)
    {
        SteamAPI_SteamUser = (SteamAPI_SteamUser_v023_t)GetProcAddress(hSteamAPI, "SteamAPI_SteamUser_v023");
        SteamAPI_ISteamUser_GetSteamID = (SteamAPI_ISteamUser_GetSteamID_t)GetProcAddress(hSteamAPI, "SteamAPI_ISteamUser_GetSteamID");
        SteamAPI_IsSteamRunning = (SteamAPI_IsSteamRunning_t)GetProcAddress(hSteamAPI, "SteamAPI_IsSteamRunning");
        SteamAPI_ISteamUser_BLoggedOn = (SteamAPI_ISteamUser_BLoggedOn_t)GetProcAddress(hSteamAPI, "SteamAPI_ISteamUser_BLoggedOn");
        SteamAPI_ISteamUser_GetAuthTicketForWebApi = (SteamAPI_ISteamUser_GetAuthTicketForWebApi_t)GetProcAddress(hSteamAPI, "SteamAPI_ISteamUser_GetAuthTicketForWebApi");
        SteamAPI_RegisterCallback = (SteamAPI_RegisterCallback_t)GetProcAddress(hSteamAPI, "SteamAPI_RegisterCallback");
        SteamAPI_UnregisterCallback = (SteamAPI_UnregisterCallback_t)GetProcAddress(hSteamAPI, "SteamAPI_UnregisterCallback");
        SteamAPI_ISteamUser_CancelAuthTicket = (SteamAPI_ISteamUser_CancelAuthTicket_t)GetProcAddress(hSteamAPI, "SteamAPI_ISteamUser_CancelAuthTicket");

        // Check for all required functions
        if (!SteamAPI_SteamUser ||
            !SteamAPI_ISteamUser_GetSteamID ||
            !SteamAPI_IsSteamRunning ||
            !SteamAPI_ISteamUser_BLoggedOn ||
            !SteamAPI_ISteamUser_GetAuthTicketForWebApi ||
            !SteamAPI_RegisterCallback ||
            !SteamAPI_UnregisterCallback ||
            !SteamAPI_ISteamUser_CancelAuthTicket)
        {
            FreeLibrary(hSteamAPI);
            hSteamAPI = nullptr;
            TRACE_ERROR("Failed to load steam_api64.dll");
            isSteamAPILoaded = false;
        }
        else
        {
            isSteamAPILoaded = true;
        }
    }
    else
    {
        TRACE_ERROR("Failed to load steam_api64.dll");
        isSteamAPILoaded = false;
    }
}

SteamAPIFunctions::~SteamAPIFunctions()
{
    if (hSteamAPI)
    {
        TRACE_INFORMATION("Freed Steam APIs");
        FreeLibrary(hSteamAPI);
        hSteamAPI = nullptr;
    }
    isSteamAPILoaded = false;

    // Reset all function pointers to nullptr
    SteamAPI_SteamUser = nullptr;
    SteamAPI_ISteamUser_GetSteamID = nullptr;
    SteamAPI_IsSteamRunning = nullptr;
    SteamAPI_ISteamUser_BLoggedOn = nullptr;
    SteamAPI_ISteamUser_GetAuthTicketForWebApi = nullptr;
    SteamAPI_RegisterCallback = nullptr;
    SteamAPI_UnregisterCallback = nullptr;
    SteamAPI_ISteamUser_CancelAuthTicket = nullptr;
}

SharedPtr<SteamAPIFunctions> GetSteamAPIs(SharedPtr<PFCoreGlobalState> state)
{
    if (state)
    {
        if (state->m_steamAPI == nullptr)
        {
            state->m_steamAPI = MakeShared<SteamAPIFunctions>();
        }

        if (!state->m_steamAPI->isSteamAPILoaded)
        {
            // If we cannot load Steam just return nullptr
            return nullptr;
        }

        return state->m_steamAPI;
    }

    return nullptr;
}

struct SteamTicketResult
{
    SteamTicketResult() noexcept = default;
    ~SteamTicketResult() noexcept;

    String steamTicket{};
    HAuthTicket steamTicketHandle{ k_HAuthTicketInvalid };
};

SteamTicketResult::~SteamTicketResult() noexcept
{
    if (steamTicketHandle != k_HAuthTicketInvalid)
    {
        SharedPtr<PFCoreGlobalState> state;
        PFCoreGlobalState::Get(state);
        auto steamAPIs = GetSteamAPIs(state);
        if (steamAPIs)
        {
            ISteamUser* steamUser = steamAPIs->SteamAPI_SteamUser();
            steamAPIs->SteamAPI_ISteamUser_CancelAuthTicket(steamUser, steamTicketHandle);
        }
    }
}

// CSteamCallbacksDelayLoad is a template class that allows for delayed loading of Steam callbacks.
// This is a macro expansion of Steam's:
//      STEAM_CALLBACK(GetSteamTicketProvider, OnGetSessionTicket, GetTicketForWebApiResponse_t);
// with edits to support delayed loading of the Steam API
template< class T, class P, bool bGameserver = false >
class CSteamCallbacksDelayLoad : public CCallbackBase
{
public:
    typedef void (T::* func_t)(P*);
    friend class CCallbackMgr;

    CSteamCallbacksDelayLoad(T* pObj, func_t func);
    virtual ~CSteamCallbacksDelayLoad();

    void Register(T* pObj, func_t func);
    void Unregister();

    void SetGameserverFlag() { m_nCallbackFlags |= k_ECallbackFlagsGameServer; }
    virtual int GetCallbackSizeBytes() S_OVERRIDE { return sizeof(P); }
    virtual void Run(void* pvParam, bool bIOFailure, SteamAPICall_t hSteamAPICall) S_OVERRIDE;
    virtual void Run(void* pvParam) S_OVERRIDE;

protected:
    T* m_pObj;
    func_t m_Func;
};

template< class T, class P, bool bGameserver /*= false */>
CSteamCallbacksDelayLoad<T, P, bGameserver>::~CSteamCallbacksDelayLoad()
{
    if (m_nCallbackFlags & k_ECallbackFlagsRegistered)
    {
        SharedPtr<PFCoreGlobalState> state;
        PFCoreGlobalState::Get(state);
        auto steamAPIs = GetSteamAPIs(state);
        if (steamAPIs)
        {
            steamAPIs->SteamAPI_UnregisterCallback(this);
        }
    }
}

template< class T, class P, bool bGameserver /*= false */>
void CSteamCallbacksDelayLoad<T, P, bGameserver>::Run(void* pvParam)
{
    (m_pObj->*m_Func)((P*)pvParam);
}

template< class T, class P, bool bGameserver /*= false */>
void CSteamCallbacksDelayLoad<T, P, bGameserver>::Run(void* pvParam, bool /*bIOFailure*/, SteamAPICall_t /*hSteamAPICall*/)
{
    (m_pObj->*m_Func)((P*)pvParam);
}

template< class T, class P, bool bGameserver >
inline CSteamCallbacksDelayLoad< T, P, bGameserver >::CSteamCallbacksDelayLoad(T* pObj, func_t func)
    : m_pObj(nullptr), m_Func(nullptr)
{
    if (bGameserver)
    {
        this->SetGameserverFlag();
    }
    Register(pObj, func);
}

template< class T, class P, bool bGameserver >
inline void CSteamCallbacksDelayLoad< T, P, bGameserver >::Register(T* pObj, func_t func)
{
    if (!pObj || !func)
        return;

    if (this->m_nCallbackFlags & CCallbackBase::k_ECallbackFlagsRegistered)
    {
        Unregister();
    }

    m_pObj = pObj;
    m_Func = func;

    SharedPtr<PFCoreGlobalState> state;
    PFCoreGlobalState::Get(state);
    auto steamAPIs = GetSteamAPIs(state);
    if (steamAPIs)
    {
        steamAPIs->SteamAPI_RegisterCallback(this, P::k_iCallback);
    }
}

template< class T, class P, bool bGameserver >
inline void CSteamCallbacksDelayLoad< T, P, bGameserver >::Unregister()
{
    SharedPtr<PFCoreGlobalState> state;
    PFCoreGlobalState::Get(state);
    auto steamAPIs = GetSteamAPIs(state);
    if (steamAPIs)
    {
        steamAPIs->SteamAPI_UnregisterCallback(this);
    }
}

class GetSteamTicketProvider : public XAsyncProviderBase
{
public:
    template<size_t n>
    GetSteamTicketProvider(
        PlayFab::RunContext&& rc,
        XAsyncBlock* async,
        const char(&identityName)[n]
    ) noexcept :
        XAsyncProviderBase{ std::move(rc), async, identityName },
        m_callbackOnGetSessionTicket(this, &GetSteamTicketProvider::OnGetSessionTicket)
    {
        m_steamTicketResult = MakeShared<SteamTicketResult>();
    }

protected:
    HRESULT Begin(RunContext runContext) override
    {
        UNREFERENCED_PARAMETER(runContext);
        return Schedule(0);
    }

    HRESULT DoWork(RunContext runContext) override
    {
        UNREFERENCED_PARAMETER(runContext);

        if (m_ticketAcquired)
        {
            Complete(sizeof(SharedPtr<SteamTicketResult>));
            return E_PENDING;
        }

        if (m_getSessionTicketError)
        {
            TRACE_ERROR("Unable to get steam auth ticket");
            Fail(E_FAIL);
            return E_FAIL;
        }

        if (m_waiting)
        {
            // Docs have indicated tickets may take up to 10 seconds to be acquired
            if (GetMilliTicks() - m_startTime > 10000)
            {
                TRACE_ERROR("Timeout waiting for steam auth ticket");
                Fail(E_FAIL);
                return E_FAIL;
            }

            Schedule(10);
            return E_PENDING;
        }

        SharedPtr<PFCoreGlobalState> state;
        PFCoreGlobalState::Get(state);
        auto steamAPIs = GetSteamAPIs(state);
        if (steamAPIs == nullptr)
        {
            TRACE_ERROR("Steam APIs not loaded");
            Fail(E_PF_CORE_MISSING_PLATFORM);
            return E_PF_CORE_MISSING_PLATFORM; // Steam APIs are not loaded
        }

        if (!steamAPIs->SteamAPI_IsSteamRunning())
        {
            TRACE_ERROR("Steam is not running");
            Fail(E_FAIL);
            return E_FAIL;
        }

        ISteamUser* steamUser = steamAPIs->SteamAPI_SteamUser();
        if (steamUser == nullptr)
        {
            TRACE_ERROR("Failed to get ISteamUser interface");
            Fail(E_FAIL);
            return E_FAIL;
        }

        if (steamAPIs->SteamAPI_ISteamUser_BLoggedOn(steamUser) == false)
        {
            TRACE_ERROR("Steam user is not logged on");
            Fail(E_FAIL);
            return E_FAIL;
        }

        auto handle = steamAPIs->SteamAPI_ISteamUser_GetAuthTicketForWebApi(steamUser, "AzurePlayFab");
        m_startTime = GetMilliTicks();
        if (handle == k_HAuthTicketInvalid)
        {
            TRACE_ERROR("Unable to request steam auth ticket");
            Fail(E_FAIL);
            return E_FAIL;
        }

        m_steamTicketResult->steamTicketHandle = handle;
        m_waiting = true;

        Schedule(0);
        return E_PENDING;
    }

    HRESULT GetResult(void* buffer, size_t bufferSize) override
    {
        if (bufferSize < sizeof(SharedPtr<SteamTicketResult>))
        {
            return E_NOT_SUFFICIENT_BUFFER;
        }

        auto result = m_steamTicketResult;
        memcpy(buffer, &result, sizeof(SharedPtr<SteamTicketResult>));

        return S_OK;
    }

    void OnGetSessionTicket(GetTicketForWebApiResponse_t* param);

private:
    // Manual callback registration - replaces:
    // STEAM_CALLBACK(GetSteamTicketProvider, OnGetSessionTicket, GetTicketForWebApiResponse_t);
    // needed to avoid static linking with Steam API
    // and SteamAPI_RegisterCallback takes a CCallbackBase pointer 
    CSteamCallbacksDelayLoad<GetSteamTicketProvider, GetTicketForWebApiResponse_t> m_callbackOnGetSessionTicket;

    bool m_waiting{};
    bool m_ticketAcquired{};
    bool m_getSessionTicketError{};
    int64_t m_startTime{};
    SharedPtr<SteamTicketResult> m_steamTicketResult{};
};

void GetSteamTicketProvider::OnGetSessionTicket(GetTicketForWebApiResponse_t* param)
{
    // This callback could possibly be invoked for a title's steam ticket request. Ignore that case.
    if (param->m_hAuthTicket != m_steamTicketResult->steamTicketHandle)
    {
        return;
    }

    if (param->m_eResult != EResult::k_EResultOK)
    {
        m_getSessionTicketError = true;
        return;
    }

    Stringstream oss;
    for (size_t i = 0; i < param->m_cubTicket; ++i) {
        oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(param->m_rgubTicket[i]);
    }

    m_steamTicketResult->steamTicket = oss.str();
    m_ticketAcquired = true;
}

class GetSteamTicketOperation : public XAsyncOperation<SharedPtr<SteamTicketResult>>
{
public:
    GetSteamTicketOperation(
        PlayFab::RunContext&& runContext
    ) noexcept;

    ~GetSteamTicketOperation() = default;

private:
    // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<SharedPtr<SteamTicketResult>> GetResult(XAsyncBlock* async) noexcept override;

    XAsyncBlock* m_asyncBlock{};
};

GetSteamTicketOperation::GetSteamTicketOperation(
    PlayFab::RunContext&& runContext
) noexcept :
    XAsyncOperation<SharedPtr<SteamTicketResult>>{ std::move(runContext) }
{
}

HRESULT GetSteamTicketOperation::OnStarted(XAsyncBlock* async) noexcept
{
    auto provider = MakeUnique<GetSteamTicketProvider>(RunContext().DeriveOnQueue(async->queue), async, XASYNC_IDENTITY(GetSteamTicketOperation));
    return XAsyncProviderBase::Run(std::move(provider));
}

Result<SharedPtr<SteamTicketResult>> GetSteamTicketOperation::GetResult(XAsyncBlock* async) noexcept
{
    Vector<char> buffer{};
    size_t bufferSize{};
    RETURN_IF_FAILED(XAsyncGetResultSize(async, &bufferSize));
    buffer.resize(bufferSize);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer.data(), nullptr));
    SharedPtr<SteamTicketResult>* result = reinterpret_cast<SharedPtr<SteamTicketResult>*>(buffer.data());

    return std::move(*result);
}

class SteamLocalUser : public LocalUser
{
public:
    static Result<SharedPtr<SteamLocalUser>> Make(
        SharedPtr<ServiceConfig> serviceConfig,
        void* clientContext
    );

    // Disable copy and assignment to prevent misuse
    SteamLocalUser(SteamLocalUser const&) = delete;
    SteamLocalUser& operator=(SteamLocalUser&) = delete;
    ~SteamLocalUser() = default;

    String LocalId() const override;
    SharedPtr<Authentication::ILoginHandler> LoginHandler() const noexcept override;
    AsyncOp<SharedPtr<SteamTicketResult>> GetSteamTicket(RunContext runContext) noexcept;

private:
    SteamLocalUser(
        SharedPtr<ServiceConfig> serviceConfig,
        void* clientContext,
        String localId
    );

    friend class SteamLocalUserLoginHandler;

    String m_localId;
    String m_steamTicket;
    SharedPtr<SteamLocalUserLoginHandler> m_loginHandler;
};

class SteamLocalUserLoginHandler : public Authentication::ILoginHandler
{
public:
    SteamLocalUserLoginHandler(SharedPtr<SteamLocalUser> user);

    // ILoginHandler
    AsyncOp<Authentication::CombinedLoginResult> Login(
        SharedPtr<PFCoreGlobalState> state,
        SharedPtr<ServiceConfig> serviceConfig,
        bool createAccount,
        RunContext rc
    ) noexcept override;

    AsyncOp<void> ReLogin(
        SharedPtr<PlayFab::Entity> entity,
        RunContext rc
    ) noexcept override;

private:
    WeakPtr<SteamLocalUser> m_localUser;
};

Result<SharedPtr<LocalUser>> MakeSteamUser(
    _In_ SharedPtr<ServiceConfig> serviceConfig,
    _In_opt_ void* clientContext
)
{
    auto makeSteamUserResult = SteamLocalUser::Make(std::move(serviceConfig), clientContext);
    RETURN_IF_FAILED(makeSteamUserResult.hr);
    return makeSteamUserResult.ExtractPayload();
}

SteamLocalUser::SteamLocalUser(
    SharedPtr<ServiceConfig> serviceConfig,
    void* clientContext,
    String localId
) :
    LocalUser{ std::move(serviceConfig), clientContext },
    m_localId{ std::move(localId) }
{
}

Result<SharedPtr<SteamLocalUser>> SteamLocalUser::Make(
    SharedPtr<ServiceConfig> serviceConfig,
    void* clientContext
)
{
    SharedPtr<PFCoreGlobalState> state;
    PFCoreGlobalState::Get(state);
    auto steamAPIs = GetSteamAPIs(state);
    if (steamAPIs == nullptr)
    {
        TRACE_ERROR("Steam APIs not loaded");
        return E_PF_CORE_MISSING_PLATFORM;
    }

    auto steamUser = steamAPIs->SteamAPI_SteamUser();
    if (!steamUser)
    {
        TRACE_ERROR("Unable to get SteamUser, failing");
        return E_FAIL;
    }

    auto localId = FormatString("%llu", steamUser->GetSteamID().ConvertToUint64());

    Allocator<SteamLocalUser> a;
    SharedPtr<SteamLocalUser> user{ new (a.allocate(1)) SteamLocalUser{ serviceConfig, clientContext, localId }, Deleter<SteamLocalUser>{}, a };
    user->m_loginHandler = MakeShared<SteamLocalUserLoginHandler>(user);

    return std::move(user);
}

String SteamLocalUser::LocalId() const
{
    return m_localId;
}

SharedPtr<Authentication::ILoginHandler> SteamLocalUser::LoginHandler() const noexcept
{
    return m_loginHandler;
}

AsyncOp<SharedPtr<SteamTicketResult>> SteamLocalUser::GetSteamTicket(RunContext runContext) noexcept
{
    auto operation = MakeUnique<GetSteamTicketOperation>(
        std::move(runContext)
    );

    return RunOperation(std::move(operation));
}

SteamLocalUserLoginHandler::SteamLocalUserLoginHandler(SharedPtr<SteamLocalUser> user) : m_localUser{ std::move(user) }
{
}

AsyncOp<Authentication::CombinedLoginResult> SteamLocalUserLoginHandler::Login(
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<ServiceConfig> serviceConfig,
    bool createAccount,
    RunContext rc
) noexcept
{
    SharedPtr<SteamLocalUser> localUser = m_localUser.lock();
    if (!localUser)
    {
        TRACE_INFORMATION("Unable to login because the LocalUser was destroyed");
        return E_FAIL;
    }

    SharedPtr<SteamTicketResult> ticketHandle = MakeShared<SteamTicketResult>();
    return localUser->GetSteamTicket(rc).Then([createAccount, serviceConfig, rc, ticketHandle](Result<SharedPtr<SteamTicketResult>> getSteamTicketResult) mutable -> AsyncOp<ServiceResponse>
    {
        RETURN_IF_FAILED(getSteamTicketResult.hr);

        auto result = getSteamTicketResult.ExtractPayload();
        std::swap(*ticketHandle, *result);

        Authentication::LoginWithSteamRequest request;
        request.SetCreateAccount(createAccount);
        request.SetSteamTicket(ticketHandle->steamTicket);
        request.SetTicketIsServiceSpecific(true);

        JsonValue requestBody = request.ToJson();
        RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));

        return serviceConfig->HttpClient()->MakePostRequest(
            "/Client/LoginWithSteam",
            UnorderedMap<String, String>{},
            std::move(requestBody),
            CacheId::AuthenticationLoginWithSteam,
            rc.Derive()
        );

    }).Then([state{ std::move(state) }, serviceConfig, loginHandler{ localUser->LoginHandler() }, ticketHandle](Result<ServiceResponse> result)->Result<Authentication::CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode == 200)
        {
            return Authentication::CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginHandler);
        }
        else
        {
            return Result<Authentication::CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> SteamLocalUserLoginHandler::ReLogin(
    SharedPtr<PlayFab::Entity> entity,
    RunContext rc
) noexcept
{
    SharedPtr<SteamLocalUser> localUser = m_localUser.lock();
    if (!localUser)
    {
        TRACE_INFORMATION("Unable to relogin because the LocalUser was destroyed");
        return E_FAIL;
    }

    SharedPtr<SteamTicketResult> ticketHandle = MakeShared<SteamTicketResult>();
    return localUser->GetSteamTicket(rc).Then([entity, rc, ticketHandle](Result<SharedPtr<SteamTicketResult>> getSteamTicketResult) mutable -> AsyncOp<ServiceResponse>
    {
        RETURN_IF_FAILED(getSteamTicketResult.hr);

        auto result = getSteamTicketResult.ExtractPayload();
        std::swap(*ticketHandle, *result);

        Authentication::LoginWithSteamRequest request;
        request.SetSteamTicket(ticketHandle->steamTicket);
        request.SetTicketIsServiceSpecific(true);

        JsonValue requestBody = request.ToJson();
        RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

        return entity->ServiceConfig()->HttpClient()->MakePostRequest(
            "/Client/LoginWithSteam",
            UnorderedMap<String, String>{},
            std::move(requestBody),
            CacheId::AuthenticationLoginWithSteam,
            rc.Derive()
        );

    }).Then([entity, ticketHandle](Result<ServiceResponse> result)->Result<void>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode == 200)
        {
            Authentication::EntityTokenResponse entityToken;
            RETURN_IF_FAILED(JsonUtils::ObjectGetMember(serviceResponse.Data, "EntityToken", entityToken));
            RETURN_IF_FAILED(entity->OnEntityTokenRefreshed(entityToken));
            return S_OK;
        }
        else
        {
            return Result<void>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

bool IsRunningOnSteam()
{
    SharedPtr<PFCoreGlobalState> state;
    PFCoreGlobalState::Get(state);
    auto steamAPIs = GetSteamAPIs(state);
    if (steamAPIs == nullptr)
    {
        return false;
    }

    if (!steamAPIs->SteamAPI_IsSteamRunning())
    {
        return false;
    }

    ISteamUser* steamUser = steamAPIs->SteamAPI_SteamUser();
    if (steamUser == nullptr)
    {
        return false;
    }

    if (steamAPIs->SteamAPI_ISteamUser_BLoggedOn(steamUser) == false)
    {
        return false;
    }

    return true;
}

}

#endif