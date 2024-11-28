// Copyright (C) Microsoft Corporation. All rights reserved.

#include "TestAppPch.h"
#include <thread>
#include "TestRunner.h"
#include "../GDK/Keyboard.h"

#include <playfab/services/PFServices.h>

#if defined(_GAMING_DESKTOP)
#include <shlobj.h>
#endif

using namespace DirectX;
void SetClearColor(float r, float g, float b);

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)
#define THROW_IF_FAILED_LOG(hr) do { HRESULT __hrRet = hr; if (FAILED(__hrRet)) { \
    SetClearColor(1.0f, 0.0f, 0.0f);\
    Tracer::AddLog(LOG_TYPE::Important, __FILE__);\
    Tracer::AddLog(LOG_TYPE::Important, STRINGIZE(__LINE__));\
    Tracer::AddLog(LOG_TYPE::Important, "FAILURE: 0x%0.8x", __hrRet); throw std::exception(); \
}} while (0, 0)

#if HC_PLATFORM != HC_PLATFORM_GDK
void SetClearColor(float r, float g, float b)
{
}
#endif

// private debug functions.  only defined with _DEBUG
#if _DEBUG
//extern "C" PF_API_ATTRIBUTES void PFDebugSetTokenRefreshDebugState(bool expireToken);
//extern "C" PF_API_ATTRIBUTES void PFDebugSetSetInterval(uint32_t interval);
#endif

namespace PlayFab
{
namespace Test
{

void WriteLogToFile(const char* strIn, const char* strFileName, bool firstLogLine = false);

enum class LOG_TYPE
{
    Important,
    Verbose
};

struct Tracer
{
    Tracer();
    ~Tracer();

    static void AddLog(LOG_TYPE logType, _In_z_ _Printf_format_string_ const char* format, ...);
    static void CALLBACK PFHCTraceCallback(_In_z_ const char* areaName, _In_ PFHCTraceLevel level, _In_ uint64_t threadId, _In_ uint64_t timestamp, _In_z_ const char* message);

    std::mutex mutex;
};

Tracer* g_tracer{ nullptr };
TestRunner* g_testRunner{ nullptr };

void TestRunner::Initialize()
{
    g_testRunner = this;

    // Init tracing. Not destroyed until shutdown
    if (!g_tracer)
    {
        g_tracer = std::make_unique<Tracer>().release();
    }

    m_keyboard = std::make_unique<DirectX::Keyboard>();

    WriteLogToFile("Starting new run\r\n", "PFTestAppEventsLog.txt", true);
    WriteLogToFile("Starting new run\r\n", "PFTestAppSummary.txt", true);
    Tracer::AddLog(LOG_TYPE::Important, "1) XUserAddAsync");
    Tracer::AddLog(LOG_TYPE::Important, "2) PFInitialize / PFServiceConfigCreateHandle");
    Tracer::AddLog(LOG_TYPE::Important, "3) PFAuthenticationLoginWithXUserAsync");
    Tracer::AddLog(LOG_TYPE::Important, "4) PFTitleDataManagementClientGetTimeAsync / PFDataGetFilesAsync");
    Tracer::AddLog(LOG_TYPE::Important, "A) Async call of PFAuthenticationLoginWithXUserAsync");
    Tracer::AddLog(LOG_TYPE::Important, "B) Async call of PFTitleDataManagementClientGetTimeAsync / PFDataGetFilesAsync");
    Tracer::AddLog(LOG_TYPE::Important, "C) PFAuthenticationLoginWithXboxAsync (use instead of #4)");
    Tracer::AddLog(LOG_TYPE::Important, "D) XblInitialize (not needed)");
    Tracer::AddLog(LOG_TYPE::Important, "T) Allow offline errors");
    Tracer::AddLog(LOG_TYPE::Important, "X) Force token expire");
    Tracer::AddLog(LOG_TYPE::Important, "Z) Force 404 on all calls");
    Tracer::AddLog(LOG_TYPE::Important, "Ctrl+#) Cleanup that step");
}

bool TestRunner::Update()
{
    auto kb = m_keyboard->GetState();
    m_keyboardButtons.Update(kb);

    bool isControlPressed = kb.LeftControl || kb.RightControl;

    if (WasPressed(Keyboard::Keys::Escape))
    {
        return true;
    }

    if (WasPressed(Keyboard::Keys::D))
    {
        Tracer::AddLog(LOG_TYPE::Important, "Step D)");
        InitXBL(isControlPressed);
    }

    if (WasPressed(Keyboard::Keys::D1))
    {
        Tracer::AddLog(LOG_TYPE::Important, "Step 1)");
        SigninWithXUser(isControlPressed);
    }

    if (WasPressed(Keyboard::Keys::D2))
    {
        Tracer::AddLog(LOG_TYPE::Important, "Step 2)");
        InitPlayFab(isControlPressed);
    }

    if (WasPressed(Keyboard::Keys::D3))
    {
        Tracer::AddLog(LOG_TYPE::Important, "Step 3)");
        LoginToPFWithXUser(isControlPressed);
    }

    if (WasPressed(Keyboard::Keys::D4) && !isControlPressed)
    {
        Tracer::AddLog(LOG_TYPE::Important, "Step 4)");
        CallPFGetTime();
        CallPFGetFiles();
    }

    if (WasPressed(Keyboard::Keys::D5))
    {
        Tracer::AddLog(LOG_TYPE::Important, "Step 5) PFEventPipelineCreateTelemetry");
        CallPFEventPipelineCreate(isControlPressed, true, true);
    }

    if (WasPressed(Keyboard::Keys::A))
    {
        Tracer::AddLog(LOG_TYPE::Important, "Step A)");
        LoginToPFWithXUserAsync(isControlPressed);
    }

    if (WasPressed(Keyboard::Keys::B) && !isControlPressed)
    {
        Tracer::AddLog(LOG_TYPE::Important, "Step B)");
        CallPFGetTimeAsync();
        CallPFGetFilesAsync();
    }

#if HC_PLATFORM != HC_PLATFORM_GDK
    if (WasPressed(Keyboard::Keys::C))
    {
        Tracer::AddLog(LOG_TYPE::Important, "Step C)");
        LoginToPFWithXToken(isControlPressed);
    }
#endif

    if (WasPressed(Keyboard::Keys::T))
    {
        Tracer::AddLog(LOG_TYPE::Important, "Step T)");
        m_offlineTesting = !m_offlineTesting;
        Tracer::AddLog(LOG_TYPE::Important, (m_offlineTesting) ? "Allowing offline errors" : "Disabled allowing offline errors");
    }

#if _DEBUG
    if (WasPressed(Keyboard::Keys::X))
    {
        Tracer::AddLog(LOG_TYPE::Important, "Step X) Force token expire");
        //PFDebugSetTokenRefreshDebugState(true);
        //PFDebugSetSetInterval(1000 * 30); // 30 seconds
    }
#endif

    if (WasPressed(Keyboard::Keys::Z))
    {
        Tracer::AddLog(LOG_TYPE::Important, "Step Z)");
        if (!m_pfInit)
        {
            Tracer::AddLog(LOG_TYPE::Important, "Call PFInit step first");
        }
        else
        {
            m_offlineTesting = true;
            m_mock = std::make_shared<HttpMock>("POST", nullptr, 404);
            Tracer::AddLog(LOG_TYPE::Important, "404 mocks setup");
        }
    }

    m_keyboardButtonsLast = m_keyboardButtons;

    return false;
}

void TestRunner::CallPFGetFilesAsync()
{
    auto async = std::make_unique<XAsyncBlock>();
    async->callback = [](XAsyncBlock* async)
    {
        std::unique_ptr<XAsyncBlock> asyncBlockPtr{ async }; // take ownership of XAsyncBlock

        size_t resultSize;
        HRESULT hr = PFDataGetFilesGetResultSize(async, &resultSize);
        if (SUCCEEDED(hr))
        {
            std::vector<char> getFilesResultBuffer(resultSize);
            PFDataGetFilesResponse* getFilesResponseResult{ nullptr };
            PFDataGetFilesGetResult(async, getFilesResultBuffer.size(), getFilesResultBuffer.data(), &getFilesResponseResult, nullptr);

            Tracer::AddLog(LOG_TYPE::Important, "GetFiles metadataCount: %d", getFilesResponseResult->metadataCount);
        }
    };

    PFDataGetFilesRequest requestFiles{};
    requestFiles.entity = m_pEntityKey;
    HRESULT hr = PFDataGetFilesAsync(m_entityHandle, &requestFiles, async.get());
    if (SUCCEEDED(hr))
    {
        async.release(); // at this point, the callback will be called so release the unique ptr
    }
    Tracer::AddLog(LOG_TYPE::Important, "PFDataGetFilesAsync");
}

void TestRunner::CallPFGetTimeAsync()
{
    // Async version of calling PFTitleDataManagementClientGetTimeAsync / PFDataGetFilesAsync

    auto async = std::make_unique<XAsyncBlock>();
    async->callback = [](XAsyncBlock* async)
    {
        std::unique_ptr<XAsyncBlock> asyncBlockPtr{ async }; // take ownership of XAsyncBlock

        PFTitleDataManagementGetTimeResult result{};
        HRESULT hr = PFTitleDataManagementClientGetTimeGetResult(async, &result);
        if (SUCCEEDED(hr))
        {
            char timeFormatted[256] = {};
            struct tm time;
            localtime_s(&time, &result.time);
            strftime(timeFormatted, sizeof(timeFormatted), "%a %Y-%m-%d %H:%M:%S %Z", &time);
            Tracer::AddLog(LOG_TYPE::Important, "Server time: %s", timeFormatted);

            const time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            localtime_s(&time, &now);
            strftime(timeFormatted, sizeof(timeFormatted), "%a %Y-%m-%d %H:%M:%S %Z", &time);
            Tracer::AddLog(LOG_TYPE::Important, "Local time: %s", timeFormatted);
        }
    };

    HRESULT hr = PFTitleDataManagementClientGetTimeAsync(m_entityHandle, async.get());
    if (SUCCEEDED(hr))
    {
        async.release(); // at this point, the callback will be called so release the unique ptr
    }
    Tracer::AddLog(LOG_TYPE::Important, "PFTitleDataManagementClientGetTimeAsync");
}

void TestRunner::LoginToPFWithXUserAsync(bool isControlPressed)
{
    if (!isControlPressed)
    {
        // Async version of calling PFAuthenticationLoginWithXUserAsync
        PFAuthenticationLoginWithXUserRequest request{};
        request.createAccount = true;
        request.user = m_userHandle1;

        struct LoginWithXUserContext
        {
            TestRunner* pThis; // better if this is a shared ptr to avoid cleanup race condition
        };

        auto async = std::make_unique<XAsyncBlock>();
        auto context = std::make_unique<LoginWithXUserContext>();
        context->pThis = this;
        async->context = context.get();
        async->callback = [](XAsyncBlock* async)
        {
            std::unique_ptr<XAsyncBlock> asyncBlockPtr{ async }; // take ownership of XAsyncBlock
            std::unique_ptr<LoginWithXUserContext> contextPtr{ static_cast<LoginWithXUserContext*>(async->context) }; // take ownership of context ptr
            auto pThis = contextPtr->pThis;

            std::vector<char> loginResultBuffer;
            PFAuthenticationLoginResult const* loginResult;
            size_t bufferSize;
            HRESULT hr = PFAuthenticationLoginWithXUserGetResultSize(async, &bufferSize);
            THROW_IF_FAILED_LOG(hr);
            if (SUCCEEDED(hr))
            {
                loginResultBuffer.resize(bufferSize);
                hr = PFAuthenticationLoginWithXUserGetResult(async, &pThis->m_entityHandle, loginResultBuffer.size(), loginResultBuffer.data(), &loginResult, nullptr);
                THROW_IF_FAILED_LOG(hr);
                Tracer::AddLog(LOG_TYPE::Important, "PFAuthenticationLoginWithXUserGetResult");
            }

            size_t size{};
            hr = PFEntityGetEntityKeySize(pThis->m_entityHandle, &size);
            if (SUCCEEDED(hr))
            {
                pThis->m_entityHandleBuffer.resize(size);
                PFEntityGetEntityKey(pThis->m_entityHandle, pThis->m_entityHandleBuffer.size(), pThis->m_entityHandleBuffer.data(), &pThis->m_pEntityKey, nullptr);
            }
            THROW_IF_FAILED_LOG(hr);
        };

        HRESULT hr = PFAuthenticationLoginWithXUserAsync(m_serviceConfigHandle, &request, async.get());
        if (SUCCEEDED(hr))
        {
            async.release(); // at this point, the callback will be called so release the unique ptr
            context.release(); // at this point, the callback will be called so release the unique ptr
        }
        THROW_IF_FAILED_LOG(hr);
        Tracer::AddLog(LOG_TYPE::Important, "PFAuthenticationLoginWithXUserAsync");
    }
    else
    {
        // Cleanup
        if (m_entityHandle)
        {
            PFEntityCloseHandle(m_entityHandle);
            m_entityHandle = nullptr;
            Tracer::AddLog(LOG_TYPE::Important, "PFEntityCloseHandle");
        }
    }
}

#if HC_PLATFORM != HC_PLATFORM_GDK
void TestRunner::LoginToPFWithXToken(bool isControlPressed)
{
    if (!isControlPressed)
    {
        XAsyncBlock async1{};
        THROW_IF_FAILED_LOG(XUserGetTokenAndSignatureAsync(
            m_userHandle1,
            XUserGetTokenAndSignatureOptions::None,
            "POST",
            "https://playfabapi.com/",
            0, nullptr,
            0, nullptr,
            &async1
        ));
        Tracer::AddLog(LOG_TYPE::Important, "XUserGetTokenAndSignatureAsync");
        THROW_IF_FAILED_LOG(XAsyncGetStatus(&async1, true));
        size_t bufferSize1;
        THROW_IF_FAILED_LOG(XUserGetTokenAndSignatureResultSize(&async1, &bufferSize1));
        std::vector<char> tokenBuffer(bufferSize1);
        XUserGetTokenAndSignatureData* getTokenAndSignatureData{ nullptr };
        THROW_IF_FAILED_LOG(XUserGetTokenAndSignatureResult(&async1, tokenBuffer.size(), tokenBuffer.data(), &getTokenAndSignatureData, nullptr));
        Tracer::AddLog(LOG_TYPE::Important, "PFAuthenticationLoginWithXUserGetResult");

        PFAuthenticationLoginWithXboxRequest request{};
        request.createAccount = true;
        std::string xboxToken = getTokenAndSignatureData->token;
        request.xboxToken = xboxToken.c_str();

        XAsyncBlock async{};
        THROW_IF_FAILED_LOG(PFAuthenticationLoginWithXboxAsync(m_serviceConfigHandle, &request, &async));
        Tracer::AddLog(LOG_TYPE::Important, "PFAuthenticationLoginWithXboxAsync");
        THROW_IF_FAILED_LOG(XAsyncGetStatus(&async, true));

        std::vector<char> loginResultBuffer;
        PFAuthenticationLoginResult const* loginResult;
        size_t bufferSize;
        THROW_IF_FAILED_LOG(PFAuthenticationLoginWithXboxGetResultSize(&async, &bufferSize));
        loginResultBuffer.resize(bufferSize);
        THROW_IF_FAILED_LOG(PFAuthenticationLoginWithXboxGetResult(&async, &m_entityHandle, loginResultBuffer.size(), loginResultBuffer.data(), &loginResult, nullptr));
        Tracer::AddLog(LOG_TYPE::Important, "PFAuthenticationLoginWithXboxGetResult");

        size_t size{};
        THROW_IF_FAILED_LOG(PFEntityGetEntityKeySize(m_entityHandle, &size));
        m_entityHandleBuffer.resize(size);
        THROW_IF_FAILED_LOG(PFEntityGetEntityKey(m_entityHandle, m_entityHandleBuffer.size(), m_entityHandleBuffer.data(), &m_pEntityKey, nullptr));
    }
    else
    {
        // Cleanup
        if (m_entityHandle)
        {
            PFEntityCloseHandle(m_entityHandle);
            m_entityHandle = nullptr;
            Tracer::AddLog(LOG_TYPE::Important, "PFEntityCloseHandle");
        }
    }
}
#endif

void CALLBACK MyPFEventPipelineBatchUploadSucceededEventHandler(
    _In_opt_ void* context,
    _In_ PFUploadedEvent const* const* eventPipelineUploadedEvents,
    _In_ size_t eventPipelineUploadedEventsCount
)
{
    Tracer::AddLog(LOG_TYPE::Important, "PFEventPipeline UploadSucceeded: %d", eventPipelineUploadedEventsCount);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(eventPipelineUploadedEvents);
    UNREFERENCED_PARAMETER(eventPipelineUploadedEventsCount);
}

void CALLBACK MyPFEventPipelineBatchUploadFailedEventHandler(
    _In_opt_ void* context,
    _In_ HRESULT translatedUploadError,
    _In_ const char* errorMessage,
    _In_ PFEvent const* const* failedEvents,
    _In_ size_t failedEventsCount
)
{
    Tracer::AddLog(LOG_TYPE::Important, "PFEventPipeline UploadFailed: %s", errorMessage);
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(translatedUploadError);
    UNREFERENCED_PARAMETER(errorMessage);
    UNREFERENCED_PARAMETER(failedEvents);
    UNREFERENCED_PARAMETER(failedEventsCount);
}

void TestRunner::CallPFEventPipelineCreate(bool isControlPressed, bool createTelemetryPipeline, bool useKey)
{
    if (!isControlPressed)
    {
        if (m_eventPipelineHandle == nullptr)
        {
            if (createTelemetryPipeline)
            {
                if (useKey)
                {
                    PFEventPipelineTelemetryKeyConfig key = {};
                    key.serviceConfigHandle = m_serviceConfigHandle;
                    key.telemetryKey = "1234"; // expected: invalid key error during upload

                    Tracer::AddLog(LOG_TYPE::Important, "PFEventPipeline Create TelemetryPipeline With Key");
                    THROW_IF_FAILED_LOG(PFEventPipelineCreateTelemetryPipelineHandleWithKey(
                        &key,
                        nullptr,
                        MyPFEventPipelineBatchUploadSucceededEventHandler,
                        MyPFEventPipelineBatchUploadFailedEventHandler,
                        nullptr,
                        &m_eventPipelineHandle));
                }
                else
                {
                    Tracer::AddLog(LOG_TYPE::Important, "PFEventPipeline Create TelemetryPipeline With Entity");
                    THROW_IF_FAILED_LOG(PFEventPipelineCreateTelemetryPipelineHandleWithEntity(
                        m_entityHandle,
                        nullptr,
                        MyPFEventPipelineBatchUploadSucceededEventHandler,
                        MyPFEventPipelineBatchUploadFailedEventHandler,
                        nullptr,
                        &m_eventPipelineHandle));
                }
            }
            else
            {
                Tracer::AddLog(LOG_TYPE::Important, "PFEventPipeline Create PlayStreamPipeline");
                THROW_IF_FAILED_LOG(PFEventPipelineCreatePlayStreamPipelineHandle(
                    m_entityHandle,
                    nullptr,
                    MyPFEventPipelineBatchUploadSucceededEventHandler,
                    MyPFEventPipelineBatchUploadFailedEventHandler,
                    nullptr,
                    &m_eventPipelineHandle));
            }
        }

        PFEvent e = {};
        e.eventNamespace = "custom.playfab.events.PlayFab.Test.TelemetryEventPipelineTests";
        e.name = "eventName";
        e.clientId = nullptr;
        e.payloadJson = "{\"Test\": 12}";

        THROW_IF_FAILED_LOG(PFEventPipelineEmitEvent(m_eventPipelineHandle, &e));
        e.name = "eventName2";
        THROW_IF_FAILED_LOG(PFEventPipelineEmitEvent(m_eventPipelineHandle, &e));
    }
    else
    {
        PFEventPipelineCloseHandle(m_eventPipelineHandle);
        m_eventPipelineHandle = nullptr;
        Tracer::AddLog(LOG_TYPE::Important, "PFEventPipeline handle closed");
    }
}

void TestRunner::CallPFGetFiles()
{
    XAsyncBlock async{};
    PFDataGetFilesRequest requestFiles{};
    requestFiles.entity = m_pEntityKey;
    THROW_IF_FAILED_LOG(PFDataGetFilesAsync(m_entityHandle, &requestFiles, &async));
    HRESULT hr1 = XAsyncGetStatus(&async, true);

    size_t resultSize;
    HRESULT hr2 = PFDataGetFilesGetResultSize(&async, &resultSize);
    std::vector<char> getFilesResultBuffer(resultSize);
    PFDataGetFilesResponse* getFilesResponseResult{ nullptr };
    HRESULT hr3 = PFDataGetFilesGetResult(&async, getFilesResultBuffer.size(), getFilesResultBuffer.data(), &getFilesResponseResult, nullptr);

    if (m_offlineTesting)
    {
        bool offlineErrors =
            (hr1 == HTTP_E_STATUS_NOT_FOUND || hr2 == HTTP_E_STATUS_NOT_FOUND || hr3 == HTTP_E_STATUS_NOT_FOUND ||
             hr1 == HTTP_E_STATUS_UNEXPECTED || hr2 == HTTP_E_STATUS_UNEXPECTED || hr3 == HTTP_E_STATUS_UNEXPECTED);
        if (offlineErrors)
        {
            Tracer::AddLog(LOG_TYPE::Important, "Got expected offline failures. HTTP_E_STATUS_NOT_FOUND");
            return;
        }
    }
    THROW_IF_FAILED_LOG(hr1);
    THROW_IF_FAILED_LOG(hr2);
    THROW_IF_FAILED_LOG(hr3);

    Tracer::AddLog(LOG_TYPE::Important, "GetFiles metadataCount: %d", getFilesResponseResult->metadataCount);
    m_blueColor += 0.25f;
    m_greenColor += 0.25f;
    SetClearColor(0.0f, m_greenColor, m_blueColor);
}

void TestRunner::CallPFGetTime()
{
    XAsyncBlock async{};
    THROW_IF_FAILED_LOG(PFTitleDataManagementClientGetTimeAsync(m_entityHandle, &async));
    Tracer::AddLog(LOG_TYPE::Important, "PFTitleDataManagementClientGetTimeAsync");
    HRESULT hr1 = XAsyncGetStatus(&async, true);

    PFTitleDataManagementGetTimeResult result{};
    HRESULT hr2 = PFTitleDataManagementClientGetTimeGetResult(&async, &result);

    if (m_offlineTesting)
    {
        bool offlineErrors =
            (hr1 == HTTP_E_STATUS_NOT_FOUND || hr2 == HTTP_E_STATUS_NOT_FOUND ||
             hr1 == HTTP_E_STATUS_UNEXPECTED || hr2 == HTTP_E_STATUS_UNEXPECTED);

        if (offlineErrors)
        {
            Tracer::AddLog(LOG_TYPE::Important, "Got expected offline failures. HTTP_E_STATUS_NOT_FOUND");
            return;
        }
    }
    THROW_IF_FAILED_LOG(hr1);
    THROW_IF_FAILED_LOG(hr2);

    char timeFormatted[256] = {};
    struct tm time;
    localtime_s(&time, &result.time);
    strftime(timeFormatted, sizeof(timeFormatted), "%a %Y-%m-%d %H:%M:%S %Z", &time);
    Tracer::AddLog(LOG_TYPE::Important, "Server time: %s", timeFormatted);

    const time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    localtime_s(&time, &now);
    strftime(timeFormatted, sizeof(timeFormatted), "%a %Y-%m-%d %H:%M:%S %Z", &time);
    Tracer::AddLog(LOG_TYPE::Important, "Local time: %s", timeFormatted);

    m_blueColor += 0.25f;
    SetClearColor(0.0f, 0.0f, m_blueColor);
}

void TestRunner::LoginToPFWithXUser(bool isControlPressed)
{
    if (!isControlPressed)
    {
        PFAuthenticationLoginWithXUserRequest request{};
        request.createAccount = true;
        request.user = m_userHandle1;

        XAsyncBlock async{};
        THROW_IF_FAILED_LOG(PFAuthenticationLoginWithXUserAsync(m_serviceConfigHandle, &request, &async));
        Tracer::AddLog(LOG_TYPE::Important, "PFAuthenticationLoginWithXUserAsync");
        HRESULT hr1 = XAsyncGetStatus(&async, true);

        std::vector<char> loginResultBuffer;
        PFAuthenticationLoginResult const* loginResult;
        size_t bufferSize{ 0 };
        HRESULT hr2 = PFAuthenticationLoginWithXUserGetResultSize(&async, &bufferSize);
        HRESULT hr3 = S_OK;
        if (SUCCEEDED(hr2))
        {
            loginResultBuffer.resize(bufferSize);
            hr3 = PFAuthenticationLoginWithXUserGetResult(&async, &m_entityHandle, loginResultBuffer.size(), loginResultBuffer.data(), &loginResult, nullptr);
            Tracer::AddLog(LOG_TYPE::Important, "PFAuthenticationLoginWithXUserGetResult");
        }

        if (m_offlineTesting)
        {
            bool offlineErrors =
                (hr1 == HTTP_E_STATUS_NOT_FOUND || hr2 == HTTP_E_STATUS_NOT_FOUND ||
                 hr1 == HTTP_E_STATUS_UNEXPECTED || hr2 == HTTP_E_STATUS_UNEXPECTED);
            if (offlineErrors)
            {
                Tracer::AddLog(LOG_TYPE::Important, "Got expected offline failures. HTTP_E_STATUS_NOT_FOUND");
                return;
            }
        }
        THROW_IF_FAILED_LOG(hr1);
        THROW_IF_FAILED_LOG(hr2);
        THROW_IF_FAILED_LOG(hr3);

        size_t size{};
        THROW_IF_FAILED_LOG(PFEntityGetEntityKeySize(m_entityHandle, &size));
        m_entityHandleBuffer.resize(size);
        THROW_IF_FAILED_LOG(PFEntityGetEntityKey(m_entityHandle, m_entityHandleBuffer.size(), m_entityHandleBuffer.data(), &m_pEntityKey, nullptr));

        m_blueColor += 0.25f;
        SetClearColor(0.0f, 0.0f, m_blueColor);
    }
    else
    {
        // Cleanup
        if (m_entityHandle)
        {
            PFEntityCloseHandle(m_entityHandle);
            m_entityHandle = nullptr;
            Tracer::AddLog(LOG_TYPE::Important, "PFEntityCloseHandle");
        }
    }
}

void TestRunner::InitPlayFab(bool isControlPressed)
{
    if (!isControlPressed)
    {
        if (!m_pfInit)
        {
            m_pfInit = true;

            TestTitleData titleData;
            bool loadSuccessful = LoadTitleData(titleData);
            UNREFERENCED_PARAMETER(loadSuccessful);
            assert(loadSuccessful);

            THROW_IF_FAILED_LOG(PFServicesInitialize(nullptr));
            Tracer::AddLog(LOG_TYPE::Important, "PFServicesInitialize");

#if HC_PLATFORM != HC_PLATFORM_GDK
            THROW_IF_FAILED_LOG(PFServiceConfigCreateHandle(
                titleData.connectionString.data(),
                titleData.titleId.data(),
                titleData.secretKey.data(),
                &m_serviceConfigHandle));
            Tracer::AddLog(LOG_TYPE::Important, "PFServiceConfigCreateHandle");

            THROW_IF_FAILED_LOG(PFServicesSetServiceConfig(m_serviceConfigHandle));
            Tracer::AddLog(LOG_TYPE::Important, "PFServicesInitialize");
#else
            THROW_IF_FAILED_LOG(PFServiceConfigCreateHandle(
                titleData.connectionString.data(),
                titleData.titleId.data(),
                &m_serviceConfigHandle));
            Tracer::AddLog(LOG_TYPE::Important, "PFServiceConfigCreateHandle");
#endif
            m_blueColor += 0.25f;
            SetClearColor(0.0f, 0.0f, m_blueColor);

            HRESULT hr = PFEntityRegisterTokenExpiredEventHandler(nullptr, nullptr, [](void* ctx, PFEntityKey const* entityKey)
            {
                Tracer::AddLog(LOG_TYPE::Important, "TokenExpiredEventHandler called");

                UNREFERENCED_PARAMETER(entityKey);
                UNREFERENCED_PARAMETER(ctx);

                PFAuthenticationLoginWithXUserRequest request{};
                request.createAccount = true;
                request.user = g_testRunner->m_userHandle1; // An XUserHandle obtained from XUserAddAsync

                XAsyncBlock async{};
                Tracer::AddLog(LOG_TYPE::Important, "Calling PFAuthenticationReLoginWithXUserAsync");
                HRESULT hr = PFAuthenticationReLoginWithXUserAsync(g_testRunner->m_entityHandle, &request, &async); // This assumes the entity handle was stored in the game's global state
                THROW_IF_FAILED_LOG(hr);
                hr = XAsyncGetStatus(&async, true); // This is doing a blocking wait for completion, but you can use the XAsyncBlock to set a callback instead for async style usage
                THROW_IF_FAILED_LOG(hr);

                // After login we could potentially get back a new player entity with a new entity key
                PFEntityKey const* pEntityKey{};
                std::vector<char> entityKeyBuffer;
                size_t size{};
                hr = PFEntityGetEntityKeySize(g_testRunner->m_entityHandle, &size); // Add your own error handling when FAILED(hr) == true
                THROW_IF_FAILED_LOG(hr);

                entityKeyBuffer.resize(size);
                hr = PFEntityGetEntityKey(g_testRunner->m_entityHandle, entityKeyBuffer.size(), entityKeyBuffer.data(), &pEntityKey, nullptr);
                THROW_IF_FAILED_LOG(hr);

            }, &m_registrationTokenExpired);
            THROW_IF_FAILED_LOG(hr);

            hr = PFEntityRegisterTokenRefreshedEventHandler(nullptr, nullptr, [](void* ctx, PFEntityKey const* entityKey, const PFEntityToken* newToken)
            {
                UNREFERENCED_PARAMETER(ctx);
                UNREFERENCED_PARAMETER(entityKey);
                UNREFERENCED_PARAMETER(newToken);
                Tracer::AddLog(LOG_TYPE::Important, "TokenRefreshedEventHandler called");

                // Perform any logging or other desired actions on token refresh
            }, &m_registrationTokenRefreshed);
            THROW_IF_FAILED_LOG(hr);
        }
    }
    else
    {
        // Cleanup
        if (m_pfInit)
        {
            m_pfInit = false;
            PFServiceConfigCloseHandle(m_serviceConfigHandle);
            m_serviceConfigHandle = nullptr;
            Tracer::AddLog(LOG_TYPE::Important, "PFServiceConfigCloseHandle");

            XAsyncBlock async{};
            THROW_IF_FAILED_LOG(PFServicesUninitializeAsync(&async));
            THROW_IF_FAILED_LOG(XAsyncGetStatus(&async, true));
            Tracer::AddLog(LOG_TYPE::Important, "PFServicesUninitializeAsync");
        }
    }
}

void TestRunner::SigninWithXUser(bool isControlPressed)
{
    if (!isControlPressed)
    {
        if (m_userHandle1 == nullptr)
        {
            AddUser(XUserAddOptions::AddDefaultUserSilently);
            m_blueColor += 0.25f;
            SetClearColor(0.0f, 0.0f, m_blueColor);
            Tracer::AddLog(LOG_TYPE::Important, "AddUser done");
        }
        else
        {
            Tracer::AddLog(LOG_TYPE::Important, "Have user");
        }
    }
    else
    {
        // Cleanup user
        if (m_userHandle1 != nullptr)
        {
            uint64_t xuid = {};
            THROW_IF_FAILED_LOG(XUserGetId(m_userHandle1, &xuid));
            XUserLocalId userLocalId = {};
            THROW_IF_FAILED_LOG(XUserGetLocalId(m_userHandle1, &userLocalId));

            Tracer::AddLog(LOG_TYPE::Important, "Closing XUserHandle 0x%0.8x. userLocalId: %ull, xuid: 0x%0.8x", m_userHandle1, userLocalId, xuid);
            SetUserHandle(nullptr);
        }
        else
        {
            Tracer::AddLog(LOG_TYPE::Important, "cleanup - no user");
        }
    }
}

void TestRunner::InitXBL(bool isControlPressed)
{
    if (!isControlPressed)
    {
        if (!m_xblInit)
        {
            m_xblInit = true;
            uint32_t titleId = 0;
            THROW_IF_FAILED_LOG(XGameGetXboxTitleId(&titleId));
            char scidBuffer[64] = {};
            sprintf_s(scidBuffer, "00000000-0000-0000-0000-0000%08X", titleId);
            XblInitArgs xblInit = { nullptr, scidBuffer };
            THROW_IF_FAILED_LOG(XblInitialize(&xblInit));
            Tracer::AddLog(LOG_TYPE::Important, "XblInitialize done");
        }
    }
    else
    {
        // Cleanup
        if (m_xblInit)
        {
            m_xblInit = false;
            auto async = new XAsyncBlock{};
            THROW_IF_FAILED_LOG(XblCleanupAsync(async));
            Tracer::AddLog(LOG_TYPE::Important, "XblCleanupAsync");
            THROW_IF_FAILED_LOG(XAsyncGetStatus(async, true));
            Tracer::AddLog(LOG_TYPE::Important, "XblCleanupAsync done");
        }
        else
        {
            Tracer::AddLog(LOG_TYPE::Important, "Init first");
        }
    }
}

HRESULT TestRunner::AddUser(XUserAddOptions options)
{
    // Attempt to get the default user, i.e. the user who launched the game
    auto async = new XAsyncBlock{};
    async->callback = [](XAsyncBlock* async)
    {
        XUserHandle user = nullptr;
        HRESULT result = XUserAddResult(async, &user);
        if (SUCCEEDED(result))
        {
            // This failure doesn't come up until you try to actually do something with the user
            uint64_t xuid = {};
            if (FAILED(result = XUserGetId(user, &xuid)))
            {
                Tracer::AddLog(LOG_TYPE::Important, "User ResolveUserIssueWithUI 0x%0.8x", result);
                //ResolveUserIssueWithUI(user);
            }
            else
            {
                XUserLocalId userLocalId = {};
                XUserGetLocalId(user, &userLocalId);

                Tracer::AddLog(LOG_TYPE::Important, "Got XUserHandle 0x%0.8x. userLocalId: %ull, xuid: 0x%0.8x", user, userLocalId.value, xuid);
                g_testRunner->SetUserHandle(user);
            }
        }
        else if (result == E_GAMEUSER_RESOLVE_USER_ISSUE_REQUIRED
            || result == E_GAMEUSER_NO_DEFAULT_USER
            || result == static_cast<HRESULT>(0x8015DC12))
        {
            Tracer::AddLog(LOG_TYPE::Important, "User SwitchUser 0x%0.8x", result);
            g_testRunner->AddUser(XUserAddOptions::AllowGuests);
        }
        else
        {
            Tracer::AddLog(LOG_TYPE::Important, "User HandleError 0x%0.8x", result);
            //HandleError(result);
        }

        delete async;
    };

    HRESULT hr = XUserAddAsync(
        options,
        async
    );
    Tracer::AddLog(LOG_TYPE::Important, "XUserAddAsync 0x%0.8x", hr);

    if (FAILED(hr))
    {
        Tracer::AddLog(LOG_TYPE::Important, "Unable to add user!");

         delete async;
        return hr;
    }

    return S_OK;
}

// Run the remainder of the currently executing test, cleanup the active TestClass and then return
bool TestRunner::Cleanup()
{
    if (m_registrationTokenExpired != 0)
    {
        PFEntityUnregisterTokenExpiredEventHandler(m_registrationTokenExpired);
    }

    if (m_registrationTokenRefreshed != 0)
    {
        PFEntityUnregisterTokenRefreshedEventHandler(m_registrationTokenRefreshed);
    }

    if (m_eventPipelineHandle)
    {
        PFEventPipelineCloseHandle(m_eventPipelineHandle);
        m_eventPipelineHandle = nullptr;
    }

    if (m_entityHandle)
    {
        PFEntityCloseHandle(m_entityHandle);
        m_entityHandle = nullptr;
        Tracer::AddLog(LOG_TYPE::Important, "PFEntityCloseHandle");
    }

    if (m_serviceConfigHandle)
    {
        PFServiceConfigCloseHandle(m_serviceConfigHandle);
        Tracer::AddLog(LOG_TYPE::Important, "PFServiceConfigCloseHandle");

        XAsyncBlock async{};
        THROW_IF_FAILED_LOG(PFServicesUninitializeAsync(&async));
        XAsyncGetStatus(&async, true);
        Tracer::AddLog(LOG_TYPE::Important, "PFServicesUninitializeAsync");
    }

    SetUserHandle(nullptr);

    if (m_xblInit)
    {
        auto async = new XAsyncBlock{};
        HRESULT hr = XblCleanupAsync(async);
        Tracer::AddLog(LOG_TYPE::Important, "XblCleanupAsync 0x%0.8x", hr);
        XAsyncGetStatus(async, true);
        m_xblInit = false;
    }

    return true;
}

Tracer::Tracer()
{
    PFHCSettingsSetTraceLevel(PFHCTraceLevel::Verbose);
    PFHCTraceSetTraceToDebugger(false);
    PFHCTraceSetClientCallback(PFHCTraceCallback);
}

Tracer::~Tracer()
{
    PFHCTraceSetClientCallback(nullptr);
}

std::string FormatStr(_In_z_ _Printf_format_string_ const char* format, ...)
{
    va_list args;
    va_start(args, format);
    std::vector<char> buf(1 + std::vsnprintf(NULL, 0, format, args));
    std::vsnprintf(buf.data(), buf.size(), format, args);
    va_end(args);

    return buf.data();
}

void WriteLogToFile(const char* strIn, const char* strFileName, bool firstLogLine)
{
    HANDLE hFile;
    std::string str = strIn;
    DWORD dwBytesToWrite = (DWORD)str.length();
    DWORD dwBytesWritten = 0;
    BOOL bErrorFlag = FALSE;

    DWORD dwCreationDisposition = OPEN_ALWAYS;
    if (firstLogLine)
    {
        firstLogLine = false;
        dwCreationDisposition = CREATE_ALWAYS; // recreate log upon start
    }

#if defined(_GAMING_DESKTOP)
    char szPath[MAX_PATH];
    SHGetSpecialFolderPathA(HWND_DESKTOP, szPath, CSIDL_DESKTOP, FALSE);
    std::string strFullPath = szPath;
#else
    std::string strFullPath = "D:";
#endif
    strFullPath += "\\";
    strFullPath += strFileName;
    hFile = CreateFileA(strFullPath.c_str(), FILE_APPEND_DATA, 0, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return;
    }

    bErrorFlag = WriteFile(
        hFile,           // open file handle
        str.data(),      // start of data to write
        dwBytesToWrite,  // number of bytes to write
        &dwBytesWritten, // number of bytes that were written
        NULL);            // no overlapped structure

    if (!bErrorFlag && dwBytesWritten != dwBytesToWrite)
    {
        OutputDebugStringA("Log file error: dwBytesWritten != dwBytesToWrite\n");
    }

    CloseHandle(hFile);
}

void Tracer::AddLog(LOG_TYPE logType, _In_z_ _Printf_format_string_ const char* format, ...)
{
    va_list args;
    va_start(args, format);
    std::vector<char> buf(1 + std::vsnprintf(NULL, 0, format, args));
    std::vsnprintf(buf.data(), buf.size(), format, args);
    va_end(args);

    SYSTEMTIME st;
    GetLocalTime(&st);
    char szTime[256];
    sprintf_s(szTime, 255, "[%0.2d/%0.2d %0.2d:%0.2d:%0.2d:%0.4d] ", st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    std::string strBuffer = szTime;
    strBuffer += buf.data();
    strBuffer += "\r\n";

    if (g_tracer)
    {
        std::lock_guard<std::mutex> lock{ g_tracer->mutex };
        if (logType == LOG_TYPE::Important)
        {
            OutputDebugStringA(strBuffer.c_str());
            WriteLogToFile(strBuffer.c_str(), "PFTestAppSummary.txt");

#if HC_PLATFORM != HC_PLATFORM_GDK
            puts(strBuffer.c_str()); // Write to STDOUT for Win32
#endif
        }

        WriteLogToFile(strBuffer.c_str(), "PFTestAppEventsLog.txt");
    }
}

void Tracer::PFHCTraceCallback(_In_z_ const char* areaName, _In_ PFHCTraceLevel level, _In_ uint64_t threadId, _In_ uint64_t timestamp, _In_z_ const char* message)
{
    UNREFERENCED_PARAMETER(areaName);
    UNREFERENCED_PARAMETER(level);
    UNREFERENCED_PARAMETER(threadId);
    UNREFERENCED_PARAMETER(timestamp);

    AddLog(LOG_TYPE::Verbose, message);
}

std::string GetExeDirectory()
{
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    std::string::size_type pos = std::string(path).find_last_of("\\/");
    return std::string(path).substr(0, pos);
}

bool LoadTitleDataJson(std::shared_ptr<char*>& testDataJson, size_t& testDataJsonLen)
{
    // Read the title data from the file into the output character array.
    std::ifstream titleDataFile;
    SetCurrentDirectoryA(GetExeDirectory().c_str());

    std::ios_base::iostate exceptionMask = titleDataFile.exceptions() | std::ios::failbit;
    titleDataFile.exceptions(exceptionMask);

    try
    {
        titleDataFile.open("testTitleData.json", std::ios::binary | std::ios::in);
    }
    catch (std::ios_base::failure& e)
    {
        Tracer::AddLog(LOG_TYPE::Important, "Could not read testTitleData.json: %s", e.what());
        return false;
    }

    if (!titleDataFile)
    {
        return false;
    }

    std::streampos begin = titleDataFile.tellg();
    titleDataFile.seekg(0, std::ios::end);
    std::streampos end = titleDataFile.tellg();
    testDataJsonLen = static_cast<int>(end - begin);
    testDataJson = std::make_shared<char*>(new char[testDataJsonLen + 1]);

    titleDataFile.seekg(0, std::ios::beg);
    titleDataFile.read(*testDataJson, testDataJsonLen);
    (*testDataJson)[testDataJsonLen] = '\0';

    titleDataFile.close();

    return true;
}

std::string GetValueForKey(char* input, std::string searchWord)
{
    char* titleIdPtr = strstr(input, searchWord.c_str());
    titleIdPtr += searchWord.length();
    titleIdPtr = strchr(titleIdPtr, '\"') + 1;

    char szValue[128];
    strncpy_s(szValue, 128, titleIdPtr, 127);
    char* end = strchr(szValue, '\"');
    *end = 0;

    return szValue;
}

bool TestRunner::LoadTitleData(TestTitleData& titleData)
{
    // Load JSON string in a platform-dependent way.
    std::shared_ptr<char*> titleJsonPtr;
    size_t size;
    const bool loadedSuccessfully = LoadTitleDataJson(titleJsonPtr, size);

    if (!loadedSuccessfully)
    {
        return false;
    }

    titleData.titleId = GetValueForKey(*titleJsonPtr, "\"titleId\"");
    titleData.connectionString = GetValueForKey(*titleJsonPtr, "\"connectionString\"");
    titleData.secretKey = GetValueForKey(*titleJsonPtr, "\"secretKey\"");

    return true;
}

void TestRunner::SetUserHandle(XUserHandle user)
{
    if (m_userHandle1 != nullptr)
    {
        XUserCloseHandle(m_userHandle1);
    }

    m_userHandle1 = user;
}

bool TestRunner::WasPressed(DirectX::Keyboard::Keys key)
{
    return ((m_keyboardButtons.IsKeyPressed(key) && !m_keyboardButtonsLast.IsKeyPressed(key)));
}



} // namespace Test
} // namespace PlayFab
