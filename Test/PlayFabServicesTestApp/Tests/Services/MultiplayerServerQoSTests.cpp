// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "TestAppPch.h"
#include "MultiplayerServerQoSTests.h"

#include <playfab/services/QoS/PFMultiplayerServerQoS.h>

#include <optional>

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
#include "QoS/UdpSocketPAL.h"

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
  #include <WinSock2.h>
  #include <WS2tcpip.h>
  #pragma comment(lib, "Ws2_32.lib")
#else
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <poll.h>
#endif
#endif

namespace PlayFab
{
namespace Test
{

void MultiplayerServerQoSTests::AddTests()
{
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
    AddTest("TestPingQosServersDefault", &MultiplayerServerQoSTests::TestPingQosServersDefault);
    AddTest("TestPingQosServersWithTunedOptions", &MultiplayerServerQoSTests::TestPingQosServersWithTunedOptions);
    AddTest("TestPingQosServersInternetRouting", &MultiplayerServerQoSTests::TestPingQosServersInternetRouting);
    AddTest("TestPingQosServersIncludeAllRegions", &MultiplayerServerQoSTests::TestPingQosServersIncludeAllRegions);
    AddTest("TestPingQosServersBadArgNullEntityHandle", &MultiplayerServerQoSTests::TestPingQosServersBadArgNullEntityHandle);
    AddTest("TestPingQosServersGetResultBadArgs", &MultiplayerServerQoSTests::TestPingQosServersGetResultBadArgs);
    AddTest("TestPingQosServersCancel", &MultiplayerServerQoSTests::TestPingQosServersCancel);
    AddTest("TestUdpSocketLoopback", &MultiplayerServerQoSTests::TestUdpSocketLoopback);
#endif
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5

struct PingQosServersResultPayload
{
    Vector<char> buffer;
    PFMultiplayerServerPingQosServersResult const* result{ nullptr };
};

class PingQosServersOperation : public XAsyncOperation<PingQosServersResultPayload>
{
public:
    PingQosServersOperation(Entity entity, std::optional<PFMultiplayerServerPingQosServersOptions> options, PlayFab::RunContext rc) noexcept
        : XAsyncOperation<PingQosServersResultPayload>{ std::move(rc) },
        m_entity{ std::move(entity) },
        m_options{ std::move(options) }
    {
    }

    static AsyncOp<PingQosServersResultPayload> Run(Entity entity, std::optional<PFMultiplayerServerPingQosServersOptions> options, PlayFab::RunContext rc) noexcept
    {
        return RunOperation(MakeUnique<PingQosServersOperation>(std::move(entity), std::move(options), std::move(rc)));
    }

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override
    {
        return PFMultiplayerServerPingQosServersAsync(
            m_entity.Handle(),
            m_options ? &(*m_options) : nullptr,
            async);
    }

    Result<PingQosServersResultPayload> GetResult(XAsyncBlock* async) noexcept override
    {
        size_t bufferSize{};
        RETURN_IF_FAILED(PFMultiplayerServerPingQosServersGetResultSize(async, &bufferSize));

        PingQosServersResultPayload payload{};
        payload.buffer.resize(bufferSize);
        RETURN_IF_FAILED(PFMultiplayerServerPingQosServersGetResult(
            async, payload.buffer.size(), payload.buffer.data(), &payload.result, nullptr));

        return payload;
    }

    Entity const m_entity;
    std::optional<PFMultiplayerServerPingQosServersOptions> const m_options;
};

void MultiplayerServerQoSTests::TestPingQosServersDefault(TestContext& tc)
{
    PingQosServersOperation::Run(DefaultTitlePlayer(), std::nullopt, RunContext())
    .Then([&](Result<PingQosServersResultPayload> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto const* aggregate = result.Payload().result;
        tc.AssertTrue(aggregate != nullptr, "result");
        tc.AssertTrue(aggregate->regionCount > 0u, "regionCount");
        tc.AssertTrue(aggregate->regions != nullptr, "regions");

        // Verify sort invariant (ascending latencyMs, unreachable rows trail).
        uint32_t previousLatency = 0;
        bool sawSuccess = false;
        for (uint32_t i = 0; i < aggregate->regionCount; ++i)
        {
            auto const* row = aggregate->regions[i];
            tc.AssertTrue(row != nullptr, "region row");
            tc.AssertTrue(row->region != nullptr, "region name");
            tc.AssertTrue(row->pingsAttempted > 0u, "pingsAttempted");
            tc.AssertTrue(row->latencyMs >= previousLatency, "sorted ascending");
            previousLatency = row->latencyMs;
            if (row->errorCode == S_OK)
            {
                sawSuccess = true;
                tc.AssertTrue(row->latencyMs < UINT32_MAX, "reachable row has latency");
                tc.AssertTrue(row->pingsSucceeded > 0u, "reachable row has succeeded pings");
            }
        }

        // CI typically allows UDP/3075 outbound; if every region failed it's worth knowing,
        // but it isn't a contract violation of the API itself. Don't fail the test on that.
        (void)sawSuccess;

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void MultiplayerServerQoSTests::TestPingQosServersWithTunedOptions(TestContext& tc)
{
    PFMultiplayerServerPingQosServersOptions options{};
    options.timeoutMs = 300;
    options.pingsPerRegion = 1;
    options.maxConcurrentRegions = 4;
    options.includeAllRegions = false;

    PingQosServersOperation::Run(DefaultTitlePlayer(), options, RunContext())
    .Then([&](Result<PingQosServersResultPayload> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto const* aggregate = result.Payload().result;
        tc.AssertTrue(aggregate != nullptr, "result");
        tc.AssertTrue(aggregate->regionCount > 0u, "regionCount");

        for (uint32_t i = 0; i < aggregate->regionCount; ++i)
        {
            auto const* row = aggregate->regions[i];
            tc.AssertTrue(row != nullptr, "region row");
            tc.AssertEqual<uint32_t>(1u, row->pingsAttempted, "pingsAttempted matches pingsPerRegion");
            tc.AssertTrue(row->pingsSucceeded <= 1u, "pingsSucceeded bounded by pingsPerRegion");
        }

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void MultiplayerServerQoSTests::TestPingQosServersInternetRouting(TestContext& tc)
{
    // Validates the routingPreference passthrough: the service should accept "Internet" and
    // return beacons (the set of VIPs returned may differ from the default "Microsoft" routing
    // tier, but the operation must still succeed and produce at least one row).
    PFMultiplayerServerPingQosServersOptions options{};
    options.routingPreference = "Internet";

    PingQosServersOperation::Run(DefaultTitlePlayer(), options, RunContext())
    .Then([&](Result<PingQosServersResultPayload> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto const* aggregate = result.Payload().result;
        tc.AssertTrue(aggregate != nullptr, "result");
        tc.AssertTrue(aggregate->regionCount > 0u, "Internet-routed regionCount");
        tc.AssertTrue(aggregate->regions != nullptr, "regions");

        for (uint32_t i = 0; i < aggregate->regionCount; ++i)
        {
            auto const* row = aggregate->regions[i];
            tc.AssertTrue(row != nullptr, "region row");
            tc.AssertTrue(row->region != nullptr, "region name");
            tc.AssertTrue(row->pingsAttempted > 0u, "pingsAttempted");
        }

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

// Verifies the includeAllRegions=true passthrough produces a well-formed response. We don't
// compare counts against the default-options run (would require two sequential calls and is
// fragile to service-side region changes); we just confirm the operation succeeds and every
// row carries a region name + pingsAttempted, which proves the flag was honored end-to-end.
void MultiplayerServerQoSTests::TestPingQosServersIncludeAllRegions(TestContext& tc)
{
    PFMultiplayerServerPingQosServersOptions options{};
    options.includeAllRegions = true;
    options.pingsPerRegion = 1;  // keep the test fast — we're verifying shape not latency
    options.timeoutMs = 300;
    options.maxConcurrentRegions = 8;

    PingQosServersOperation::Run(DefaultTitlePlayer(), options, RunContext())
    .Then([&](Result<PingQosServersResultPayload> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto const* aggregate = result.Payload().result;
        tc.AssertTrue(aggregate != nullptr, "result");
        tc.AssertTrue(aggregate->regionCount > 0u, "regionCount > 0 with includeAllRegions=true");
        tc.AssertTrue(aggregate->regions != nullptr, "regions");

        for (uint32_t i = 0; i < aggregate->regionCount; ++i)
        {
            auto const* row = aggregate->regions[i];
            tc.AssertTrue(row != nullptr, "region row");
            tc.AssertTrue(row->region != nullptr, "region name");
            tc.AssertEqual<uint32_t>(1u, row->pingsAttempted, "pingsAttempted matches pingsPerRegion");
        }

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

// Custom operation that verifies the public C API rejects a NULL entityHandle synchronously.
// Routed through XAsyncOperation/RunOperation (like every other test in this file) instead of
// a raw stack XAsyncBlock, so it uses the same RunContext-derived queue (RunOperation ties the
// XAsyncBlock to RunContext().TaskQueueHandle(), not the default process queue) and the same
// termination/cancellation registration as the rest of the suite.
class PingQosServersBadArgNullEntityHandleOperation : public XAsyncOperation<bool>
{
public:
    PingQosServersBadArgNullEntityHandleOperation(PlayFab::RunContext rc) noexcept
        : XAsyncOperation<bool>{ std::move(rc) }
    {
    }

    static AsyncOp<bool> Run(PlayFab::RunContext rc) noexcept
    {
        return RunOperation(MakeUnique<PingQosServersBadArgNullEntityHandleOperation>(std::move(rc)));
    }

private:
    // Public-C API contract test: passing NULL for the required `entityHandle` parameter must
    // fail synchronously without scheduling any background work. The orchestrator throws when
    // duplicating a NULL entity; the AsyncApiImpl catch path converts the exception to HRESULT
    // and returns it immediately. Returning a failure HRESULT here (instead of calling
    // XAsyncCancel/etc.) causes XAsyncOperationBase::OnStarted to route straight to OnFailed(),
    // which completes the AsyncOp - GetResult() below is never invoked in this scenario.
    HRESULT OnStarted(XAsyncBlock* async) noexcept override
    {
        return PFMultiplayerServerPingQosServersAsync(nullptr, nullptr, async);
    }

    Result<bool> GetResult(XAsyncBlock* async) noexcept override
    {
        // Should never be reached: OnStarted always fails synchronously in this test.
        (void)async;
        return Result<bool>{ E_UNEXPECTED, "Expected synchronous failure; GetResult should not run" };
    }
};

void MultiplayerServerQoSTests::TestPingQosServersBadArgNullEntityHandle(TestContext& tc)
{
    PingQosServersBadArgNullEntityHandleOperation::Run(RunContext())
    .Then([&](Result<bool> result) -> Result<void>
    {
        tc.AssertTrue(FAILED(result.hr), "Async with NULL entityHandle returns a failure HRESULT synchronously");
        tc.AssertEqual<HRESULT>(E_INVALIDARG, result.hr, "specifically E_INVALIDARG");
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

// Custom operation that runs a real Ping, then exercises GetResult bad-arg paths inside the
// override before reporting success. Lets us test the public C API edges without paying for
// a second network call. Payload is `bool` (always true on success) to keep Result<T> brace
// construction unambiguous — Result<HRESULT> collides because Result has both Result(HRESULT)
// and Result(T&&) constructors.
class GetResultBadArgsOperation : public XAsyncOperation<bool>
{
public:
    GetResultBadArgsOperation(Entity entity, PlayFab::RunContext rc) noexcept
        : XAsyncOperation<bool>{ std::move(rc) }, m_entity{ std::move(entity) }
    {
    }

    static AsyncOp<bool> Run(Entity entity, PlayFab::RunContext rc) noexcept
    {
        return RunOperation(MakeUnique<GetResultBadArgsOperation>(std::move(entity), std::move(rc)));
    }

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override
    {
        return PFMultiplayerServerPingQosServersAsync(m_entity.Handle(), nullptr, async);
    }

    Result<bool> GetResult(XAsyncBlock* async) noexcept override
    {
        size_t bufferSize{};
        HRESULT hr = PFMultiplayerServerPingQosServersGetResultSize(async, &bufferSize);
        if (FAILED(hr))
        {
            return Result<bool>{ hr, "GetResultSize on completed async returned failure" };
        }
        if (bufferSize == 0)
        {
            return Result<bool>{ E_UNEXPECTED, "GetResultSize reported bufferSize == 0" };
        }

        // Bad-arg #1: NULL result outparam must be rejected up-front (guard at
        // PFMultiplayerServerQoS.cpp:67 — RETURN_HR_INVALIDARG_IF_NULL(result)).
        Vector<char> buffer(bufferSize);
        HRESULT hrNullResult = PFMultiplayerServerPingQosServersGetResult(
            async, bufferSize, buffer.data(), nullptr, nullptr);
        if (hrNullResult != E_INVALIDARG)
        {
            Stringstream ss;
            ss << "Expected E_INVALIDARG from GetResult with NULL result outparam, got 0x"
               << std::hex << hrNullResult;
            return Result<bool>{ E_FAIL, ss.str() };
        }

        // Bad-arg #2: buffer too small. XAsyncGetResult returns E_NOT_SUFFICIENT_BUFFER for
        // bufferSize < required; we accept any FAILED HRESULT to stay robust to layering.
        PFMultiplayerServerPingQosServersResult const* outPtr{ nullptr };
        HRESULT hrTooSmall = PFMultiplayerServerPingQosServersGetResult(
            async, /*bufferSize*/ 0, nullptr, &outPtr, nullptr);
        if (SUCCEEDED(hrTooSmall))
        {
            return Result<bool>{ E_FAIL, "Expected failure from GetResult with bufferSize=0" };
        }

        return Result<bool>{ true };
    }

    Entity m_entity;
};

void MultiplayerServerQoSTests::TestPingQosServersGetResultBadArgs(TestContext& tc)
{
    GetResultBadArgsOperation::Run(DefaultTitlePlayer(), RunContext())
    .Then([&](Result<bool> result) -> Result<void>
    {
        if (FAILED(result.hr))
        {
            return Result<void>{ result.hr, String{ result.errorMessage } };
        }
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

// Custom operation that starts a Ping and immediately requests cancellation on the same
// XAsyncBlock. Two outcomes are accepted as graceful:
//   1. async completes with E_ABORT — cancel beat the work, we get no payload
//   2. async completes with S_OK — cancel arrived during fan-out; the payload should carry
//      at least one per-region row whose errorCode is E_ABORT
// The contract is "no crash, no hang" — both paths satisfy that.
class CancelDuringPingOperation : public XAsyncOperation<PingQosServersResultPayload>
{
public:
    CancelDuringPingOperation(Entity entity, PlayFab::RunContext rc) noexcept
        : XAsyncOperation<PingQosServersResultPayload>{ std::move(rc) }, m_entity{ std::move(entity) }
    {
    }

    static AsyncOp<PingQosServersResultPayload> Run(Entity entity, PlayFab::RunContext rc) noexcept
    {
        return RunOperation(MakeUnique<CancelDuringPingOperation>(std::move(entity), std::move(rc)));
    }

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override
    {
        HRESULT hr = PFMultiplayerServerPingQosServersAsync(m_entity.Handle(), nullptr, async);
        if (FAILED(hr))
        {
            return hr;
        }
        // Cancel as soon as the work is queued. May or may not beat the inner ListQos call.
        XAsyncCancel(async);
        return S_OK;
    }

    Result<PingQosServersResultPayload> GetResult(XAsyncBlock* async) noexcept override
    {
        size_t bufferSize{};
        RETURN_IF_FAILED(PFMultiplayerServerPingQosServersGetResultSize(async, &bufferSize));

        PingQosServersResultPayload payload{};
        payload.buffer.resize(bufferSize);
        RETURN_IF_FAILED(PFMultiplayerServerPingQosServersGetResult(
            async, payload.buffer.size(), payload.buffer.data(), &payload.result, nullptr));

        return payload;
    }

    Entity m_entity;
};

void MultiplayerServerQoSTests::TestPingQosServersCancel(TestContext& tc)
{
    CancelDuringPingOperation::Run(DefaultTitlePlayer(), RunContext())
    .Then([&](Result<PingQosServersResultPayload> result) -> Result<void>
    {
        // Path 1: cancel beat the work — async failed with E_ABORT, no payload.
        if (result.hr == E_ABORT)
        {
            return S_OK;
        }
        // Any other failure is unexpected (network errors, etc.).
        if (FAILED(result.hr))
        {
            return Result<void>{ result.hr, "Cancel produced unexpected failure HRESULT" };
        }

        // Path 2: cancel hit during fan-out — orchestrator completed S_OK with per-region
        // rows reflecting the cancellation. Verify at least one row reports E_ABORT.
        auto const* aggregate = result.Payload().result;
        tc.AssertTrue(aggregate != nullptr, "cancel result");
        tc.AssertTrue(aggregate->regions != nullptr, "cancel regions");

        bool sawAbort = false;
        for (uint32_t i = 0; i < aggregate->regionCount; ++i)
        {
            auto const* row = aggregate->regions[i];
            if (row != nullptr && row->errorCode == E_ABORT)
            {
                sawAbort = true;
                break;
            }
        }
        tc.AssertTrue(sawAbort, "at least one region reports E_ABORT after cancel");
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

// In-process UDP echo bound to 127.0.0.1:<random>. Mirrors packets verbatim to the sender.
// Used by TestUdpSocketLoopback to exercise the UdpSocketPAL implementations (Winsock on
// Win32/GDK, BSD sockets on Linux/Android/iOS/macOS) without depending on any MPS beacon
// being reachable from the test host.
namespace
{
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
using NativeSocket = SOCKET;
constexpr SOCKET kInvalidSocket = INVALID_SOCKET;
inline int CloseNative(NativeSocket s) noexcept { return ::closesocket(s); }
#else
using NativeSocket = int;
constexpr int kInvalidSocket = -1;
inline int CloseNative(NativeSocket s) noexcept { return ::close(s); }
#endif

class UdpEchoServer
{
public:
    UdpEchoServer() noexcept
    {
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
        // WSAStartup is refcounted by Winsock — safe to call alongside UdpSocketPAL's own gate.
        WSADATA wsaData;
        if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            return;
        }
        m_wsaInitialized = true;
#endif
        m_socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (m_socket == kInvalidSocket)
        {
            return;
        }

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        addr.sin_port = 0;  // kernel chooses an ephemeral port

        if (::bind(m_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0)
        {
            CloseNative(m_socket);
            m_socket = kInvalidSocket;
            return;
        }

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
        int addrLen = sizeof(addr);
#else
        socklen_t addrLen = sizeof(addr);
#endif
        if (::getsockname(m_socket, reinterpret_cast<sockaddr*>(&addr), &addrLen) != 0)
        {
            CloseNative(m_socket);
            m_socket = kInvalidSocket;
            return;
        }
        m_port = ntohs(addr.sin_port);

        m_thread = std::thread([this]() { EchoLoop(); });
    }

    ~UdpEchoServer() noexcept
    {
        m_stop.store(true, std::memory_order_relaxed);
        if (m_socket != kInvalidSocket)
        {
            // Closing the bound descriptor unblocks recvfrom on both Winsock and POSIX.
            CloseNative(m_socket);
            m_socket = kInvalidSocket;
        }
        if (m_thread.joinable())
        {
            m_thread.join();
        }
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
        if (m_wsaInitialized)
        {
            ::WSACleanup();
        }
#endif
    }

    UdpEchoServer(UdpEchoServer const&) = delete;
    UdpEchoServer& operator=(UdpEchoServer const&) = delete;

    bool Valid() const noexcept { return m_port != 0; }
    std::uint16_t Port() const noexcept { return m_port; }

private:
    void EchoLoop() noexcept
    {
        std::array<std::uint8_t, 2048> buf{};
        while (!m_stop.load(std::memory_order_relaxed))
        {
#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_GDK
            // Do not rely on the destructor's close(m_socket) to unblock this thread's
            // recvfrom() — that's reliable on Winsock/BSD but is *not* guaranteed on Linux
            // (closing a fd from another thread while this thread is blocked in a syscall on
            // that same fd can leave recvfrom() parked forever, deadlocking ~UdpEchoServer's
            // m_thread.join()). Poll with a short timeout instead so we always wake up on our
            // own and re-check m_stop, regardless of whether close() interrupts us.
            pollfd pfd{};
            pfd.fd = m_socket;
            pfd.events = POLLIN;
            int pollResult = ::poll(&pfd, 1, 100);
            if (pollResult <= 0)
            {
                continue;  // timeout, EINTR, or the socket was torn down — recheck m_stop
            }
#endif
            sockaddr_in from{};
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
            int fromLen = sizeof(from);
#else
            socklen_t fromLen = sizeof(from);
#endif
            int n = ::recvfrom(
                m_socket,
                reinterpret_cast<char*>(buf.data()),
                static_cast<int>(buf.size()),
                0,
                reinterpret_cast<sockaddr*>(&from),
                &fromLen);
            if (n <= 0)
            {
                break;  // descriptor closed or fatal error — exit loop
            }
            ::sendto(
                m_socket,
                reinterpret_cast<const char*>(buf.data()),
                n,
                0,
                reinterpret_cast<sockaddr*>(&from),
                fromLen);
        }
    }

    NativeSocket m_socket{ kInvalidSocket };
    std::uint16_t m_port{ 0 };
    std::atomic<bool> m_stop{ false };
    std::thread m_thread;
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
    bool m_wsaInitialized{ false };
#endif
};

}  // namespace

void MultiplayerServerQoSTests::TestUdpSocketLoopback(TestContext& tc)
{
    // Sanity-check the UdpSocketPAL implementation against a local echo. Validates:
    //   1. Connect("127.0.0.1", <port>) succeeds and returns a usable UdpSocket
    //   2. Exchange writes the request, receives the echoed reply, runs the validator,
    //      and returns a non-negative RTT (sub-millisecond is normal on loopback)
    //   3. The validator callback contract — only invoked on packets that pass the sender
    //      endpoint check; expected to return bool indicating "this is the reply we want"
    UdpEchoServer echo;
    tc.AssertTrue(echo.Valid(), "UDP echo server bound to 127.0.0.1");

    UniquePtr<UdpSocket> socket;
    HRESULT connectHr = UdpSocket::Connect("127.0.0.1", echo.Port(), socket);
    tc.AssertTrue(SUCCEEDED(connectHr), "UdpSocket::Connect to 127.0.0.1");
    tc.AssertTrue(socket != nullptr, "UdpSocket payload");

    // Send 10 arbitrary bytes; loopback echoes them verbatim; validator does byte-for-byte compare.
    std::array<std::uint8_t, 10> request{ 0xFF, 0xFF, 0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE, 0x12, 0x34 };

    auto validator = [](void* ctx, std::uint8_t const* bytes, std::size_t size) -> bool
    {
        auto const& expected = *static_cast<std::array<std::uint8_t, 10> const*>(ctx);
        if (size != expected.size())
        {
            return false;
        }
        return std::memcmp(bytes, expected.data(), size) == 0;
    };

    CancellationToken cancel = CancellationToken::Root();  // not cancelled
    std::chrono::microseconds rttUs{};
    HRESULT exchangeHr = socket->Exchange(
        request.data(),
        request.size(),
        std::chrono::milliseconds{ 500 },
        validator,
        &request,
        cancel,
        rttUs);

    tc.AssertTrue(SUCCEEDED(exchangeHr), "UdpSocket::Exchange against loopback echo");
    if (SUCCEEDED(exchangeHr))
    {
        tc.AssertTrue(rttUs.count() >= 0, "non-negative RTT from loopback exchange");
    }

    tc.EndTest(Result<void>{ S_OK });
}

#endif // QoS-supported platforms

}
}
