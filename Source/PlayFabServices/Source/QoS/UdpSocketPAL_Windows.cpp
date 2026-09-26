// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//
// Winsock2-based UDP socket implementation for Win32 and GDK.
// The POSIX implementation lives in Linux/UdpSocketPAL_Linux.cpp and Apple/UdpSocketPAL_Apple.cpp.

#include "stdafx.h"
#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && !HC_PLATFORM_IS_PLAYSTATION
#include "UdpSocketPAL.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <atomic>
#include <cstring>
#include <mutex>

#pragma comment(lib, "Ws2_32.lib")

namespace PlayFab
{

namespace
{

// Reference-counted WSAStartup gate. On Windows games the host process may already have
// initialized Winsock; we still call WSAStartup so our refcount is correct. WSACleanup
// decrements the internal Winsock refcount, so this is safe.
class WinsockBringup
{
public:
    HRESULT Acquire() noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if (m_refCount == 0)
        {
            WSADATA wsaData{};
            int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (err != 0)
            {
                return HRESULT_FROM_WIN32(err);
            }
        }
        ++m_refCount;
        return S_OK;
    }

    void Release() noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if (m_refCount > 0)
        {
            --m_refCount;
            if (m_refCount == 0)
            {
                WSACleanup();
            }
        }
    }

private:
    std::mutex m_mutex;
    int m_refCount{ 0 };
};

WinsockBringup& GetWinsockBringup() noexcept
{
    static WinsockBringup s_instance;
    return s_instance;
}

// getaddrinfo on Winsock returns EAI_* constants which alias to WSA error codes
// (e.g. EAI_NONAME == WSAHOST_NOT_FOUND). Map them explicitly so callers receive
// canonical Winsock-based HRESULTs and so the intent is not buried in the alias.
HRESULT HResultFromGaiError(int gai) noexcept
{
    switch (gai)
    {
    case EAI_AGAIN:    return HRESULT_FROM_WIN32(WSATRY_AGAIN);
    case EAI_BADFLAGS: return E_INVALIDARG;
    case EAI_FAIL:     return HRESULT_FROM_WIN32(WSANO_RECOVERY);
    case EAI_FAMILY:   return HRESULT_FROM_WIN32(WSAEAFNOSUPPORT);
    case EAI_MEMORY:   return E_OUTOFMEMORY;
    case EAI_NONAME:   return HRESULT_FROM_WIN32(WSAHOST_NOT_FOUND);
    case EAI_SERVICE:  return HRESULT_FROM_WIN32(WSATYPE_NOT_FOUND);
    case EAI_SOCKTYPE: return HRESULT_FROM_WIN32(WSAESOCKTNOSUPPORT);
    default:           return HRESULT_FROM_WIN32(gai);
    }
}

class WinsockUdpSocket : public UdpSocket
{
public:
    WinsockUdpSocket(SOCKET sock, sockaddr_storage const& destination, int destinationLen) noexcept
        : m_socket{ sock }
        , m_destination{ destination }
        , m_destinationLen{ destinationLen }
    {
    }

    ~WinsockUdpSocket() noexcept override
    {
        if (m_socket != INVALID_SOCKET)
        {
            closesocket(m_socket);
            m_socket = INVALID_SOCKET;
        }
        GetWinsockBringup().Release();
    }

    HRESULT Exchange(
        const std::uint8_t* request,
        std::size_t requestSize,
        std::chrono::milliseconds timeout,
        ValidatorFn validator,
        void* validatorContext,
        CancellationToken const& cancel,
        std::chrono::microseconds& result
    ) noexcept override
    {
        if (request == nullptr || requestSize == 0 || requestSize > kMaxDatagramSize || validator == nullptr)
        {
            return E_INVALIDARG;
        }
        if (cancel.IsCancelled())
        {
            return E_ABORT;
        }

        auto sentAt = std::chrono::steady_clock::now();
        int const requestSizeInt = static_cast<int>(requestSize);
        int sent = sendto(
            m_socket,
            reinterpret_cast<const char*>(request),
            requestSizeInt,
            0,
            reinterpret_cast<const sockaddr*>(&m_destination),
            m_destinationLen);
        if (sent == SOCKET_ERROR)
        {
            return HRESULT_FROM_WIN32(WSAGetLastError());
        }
        if (sent != requestSizeInt)
        {
            // UDP sendto is expected to send the full datagram or fail.
            return HRESULT_FROM_WIN32(ERROR_BAD_LENGTH);
        }

        auto deadline = sentAt + timeout;
        std::uint8_t buffer[kMaxDatagramSize];

        for (;;)
        {
            auto now = std::chrono::steady_clock::now();
            if (now >= deadline)
            {
                return kUdpSocketTimeoutHr;
            }
            if (cancel.IsCancelled())
            {
                return E_ABORT;
            }

            auto remaining = std::chrono::duration_cast<std::chrono::milliseconds>(deadline - now);
            auto pollMs = std::min(remaining, kCancelPollInterval);

            WSAPOLLFD pfd{};
            pfd.fd = m_socket;
            pfd.events = POLLRDNORM;
            int pollResult = WSAPoll(&pfd, 1, static_cast<INT>(pollMs.count()));
            if (pollResult == SOCKET_ERROR)
            {
                return HRESULT_FROM_WIN32(WSAGetLastError());
            }
            if (pollResult == 0)
            {
                // Cooperative wake interval expired with no data — loop and re-check cancel/deadline.
                continue;
            }

            sockaddr_storage from{};
            int fromLen = static_cast<int>(sizeof(from));
            int recvd = recvfrom(
                m_socket,
                reinterpret_cast<char*>(buffer),
                static_cast<int>(sizeof(buffer)),
                0,
                reinterpret_cast<sockaddr*>(&from),
                &fromLen);
            if (recvd == SOCKET_ERROR)
            {
                int wsaErr = WSAGetLastError();
                // Spurious wake or harmless ICMP unreachable on a prior datagram — keep waiting.
                if (wsaErr == WSAEWOULDBLOCK || wsaErr == WSAECONNRESET || wsaErr == WSAEMSGSIZE)
                {
                    continue;
                }
                return HRESULT_FROM_WIN32(wsaErr);
            }

            if (!SameEndpoint(from, fromLen, m_destination, m_destinationLen))
            {
                continue;
            }
            if (!validator(validatorContext, buffer, static_cast<std::size_t>(recvd)))
            {
                continue;
            }

            auto rtt = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - sentAt
            );
            result = std::move(rtt);
            return S_OK;
        }
    }

private:
    static bool SameEndpoint(sockaddr_storage const& a, int aLen,
                             sockaddr_storage const& b, int bLen) noexcept
    {
        if (aLen != bLen || a.ss_family != b.ss_family)
        {
            return false;
        }
        if (a.ss_family == AF_INET)
        {
            auto const* sa = reinterpret_cast<sockaddr_in const*>(&a);
            auto const* sb = reinterpret_cast<sockaddr_in const*>(&b);
            return sa->sin_port == sb->sin_port
                && sa->sin_addr.s_addr == sb->sin_addr.s_addr;
        }
        if (a.ss_family == AF_INET6)
        {
            auto const* sa = reinterpret_cast<sockaddr_in6 const*>(&a);
            auto const* sb = reinterpret_cast<sockaddr_in6 const*>(&b);
            return sa->sin6_port == sb->sin6_port
                && std::memcmp(&sa->sin6_addr, &sb->sin6_addr, sizeof(sa->sin6_addr)) == 0;
        }
        return false;
    }

    SOCKET m_socket{ INVALID_SOCKET };
    sockaddr_storage m_destination{};
    int m_destinationLen{ 0 };
};

} // anonymous namespace

static HRESULT HresultFromGetaddrinfo(int eai)
{
    // getaddrinfo returns EAI_* codes, not Win32 last-error codes. Map to a reasonable WinSock error.
    DWORD wsaError = WSAEINVAL;
    switch (eai)
    {
    case EAI_AGAIN:
       wsaError = WSATRY_AGAIN;
       break;
    case EAI_FAIL:
        wsaError = WSANO_RECOVERY;
        break;
    case EAI_FAMILY:
        wsaError = WSAEAFNOSUPPORT;
        break;
    case EAI_MEMORY:
        wsaError = WSA_NOT_ENOUGH_MEMORY;
        break;
    case EAI_NONAME:
        wsaError = WSAHOST_NOT_FOUND;
        break;
#if defined(EAI_NODATA) && (EAI_NODATA != EAI_NONAME)
    case EAI_NODATA:
        wsaError = WSAHOST_NOT_FOUND;
        break;
#endif
#if defined(EAI_SERVICE)
    case EAI_SERVICE:
        wsaError = WSATYPE_NOT_FOUND;
        break;
#endif
#if defined(EAI_BADFLAGS)
    case EAI_BADFLAGS:
        wsaError = WSAEINVAL;
        break;
#endif
    default:
        wsaError = WSAEINVAL;
        break;
    }
    return HRESULT_FROM_WIN32(wsaError);
}

HRESULT UdpSocket::Connect(
    const char* serverHost, 
    std::uint16_t port,
    UniquePtr<UdpSocket>& result
) noexcept
{
    if (serverHost == nullptr || serverHost[0] == '\0')
    {
        return E_INVALIDARG;
    }

    HRESULT hr = GetWinsockBringup().Acquire();
    if (FAILED(hr))
    {
        return hr;
    }

    // Release Winsock if any subsequent step fails.
    struct WinsockGuard
    {
        bool committed{ false };
        ~WinsockGuard() noexcept { if (!committed) { GetWinsockBringup().Release(); } }
    } guard;

    char portBuf[8];
    std::snprintf(portBuf, sizeof(portBuf), "%u", static_cast<unsigned>(port));

    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;     // accept v4 or v6
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    addrinfo* resolved = nullptr;
    int gaiResult = getaddrinfo(serverHost, portBuf, &hints, &resolved);
    if (gaiResult != 0)
    {
        return HresultFromGetaddrinfo(gaiResult);
    }

    struct AddrInfoGuard
    {
        addrinfo* ptr;
        ~AddrInfoGuard() noexcept { if (ptr) { freeaddrinfo(ptr); } }
    } addrGuard{ resolved };

    if (resolved == nullptr)
    {
        return HRESULT_FROM_WIN32(WSAHOST_NOT_FOUND);
    }

    SOCKET sock = socket(resolved->ai_family, resolved->ai_socktype, resolved->ai_protocol);
    if (sock == INVALID_SOCKET)
    {
        return HRESULT_FROM_WIN32(WSAGetLastError());
    }

    // Close the socket if any subsequent step fails before ownership is transferred to
    // WinsockUdpSocket. Cleared once MakeUnique succeeds.
    struct SocketGuard
    {
        SOCKET sock{ INVALID_SOCKET };
        ~SocketGuard() noexcept { if (sock != INVALID_SOCKET) { closesocket(sock); } }
    } socketGuard{ sock };

    u_long nonBlocking = 1;
    if (ioctlsocket(sock, FIONBIO, &nonBlocking) == SOCKET_ERROR)
    {
        return HRESULT_FROM_WIN32(WSAGetLastError());
    }

    sockaddr_storage dest{};
    int destLen = static_cast<int>(resolved->ai_addrlen);
    std::memcpy(&dest, resolved->ai_addr, resolved->ai_addrlen);

    UniquePtr<WinsockUdpSocket> concrete = MakeUnique<WinsockUdpSocket>(sock, dest, destLen);
    if (!concrete)
    {
        closesocket(sock);
        return E_OUTOFMEMORY;
    }

    // WinsockUdpSocket now owns both the socket descriptor and the WSAStartup refcount;
    // release the local guards so its destructor — not ours — performs cleanup.
    socketGuard.sock = INVALID_SOCKET;
    guard.committed = true;

    result = std::move(concrete);
    return S_OK;
}

} // namespace PlayFab
#endif // HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && !HC_PLATFORM_IS_PLAYSTATION
