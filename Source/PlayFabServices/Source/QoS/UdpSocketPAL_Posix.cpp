// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//
// BSD-sockets UDP implementation shared by Linux, Android, iOS, and macOS.
// The Microsoft (Winsock2) implementation lives in Microsoft/UdpSocketPAL_Windows.cpp.

#include "stdafx.h"
#include "UdpSocketPAL.h"

#if !HC_PLATFORM_IS_MICROSOFT

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
#include <cstring>

namespace PlayFab
{

namespace
{

// libHttpClient defines __HRESULT_FROM_WIN32 for every platform but does not pull in
// winerror.h, so the single-underscore HRESULT_FROM_WIN32 name is unavailable on POSIX.
// We embed errno values (low 16 bits) into the FACILITY_WIN32 space the same way the
// Microsoft implementation does so callers can compare HRESULTs across platforms.
constexpr HRESULT HResultFromErrno(int err) noexcept
{
    return __HRESULT_FROM_WIN32(err);
}

class PosixUdpSocket : public UdpSocket
{
public:
    PosixUdpSocket(int fd, sockaddr_storage const& destination, socklen_t destinationLen) noexcept
        : m_fd{ fd }
        , m_destination{ destination }
        , m_destinationLen{ destinationLen }
    {
    }

    ~PosixUdpSocket() noexcept override
    {
        if (m_fd >= 0)
        {
            close(m_fd);
            m_fd = -1;
        }
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
        ssize_t sent = sendto(
            m_fd,
            request,
            requestSize,
            0,
            reinterpret_cast<const sockaddr*>(&m_destination),
            m_destinationLen);
        if (sent < 0)
        {
            return HResultFromErrno(errno);
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

            pollfd pfd{};
            pfd.fd = m_fd;
            pfd.events = POLLIN;
            int pollResult = poll(&pfd, 1, static_cast<int>(pollMs.count()));
            if (pollResult < 0)
            {
                if (errno == EINTR)
                {
                    continue;
                }
                return HResultFromErrno(errno);
            }
            if (pollResult == 0)
            {
                // Cooperative wake interval expired with no data — loop and re-check cancel/deadline.
                continue;
            }

            sockaddr_storage from{};
            socklen_t fromLen = static_cast<socklen_t>(sizeof(from));
            ssize_t recvd = recvfrom(
                m_fd,
                buffer,
                sizeof(buffer),
                0,
                reinterpret_cast<sockaddr*>(&from),
                &fromLen);
            if (recvd < 0)
            {
                // Spurious wake or asynchronous ICMP unreachable on a prior datagram — keep waiting.
                if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR || errno == ECONNREFUSED)
                {
                    continue;
                }
                return HResultFromErrno(errno);
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
    static bool SameEndpoint(sockaddr_storage const& a, socklen_t aLen,
                             sockaddr_storage const& b, socklen_t bLen) noexcept
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

    int m_fd{ -1 };
    sockaddr_storage m_destination{};
    socklen_t m_destinationLen{ 0 };
};

} // anonymous namespace

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
        // EAI_* codes are negative on glibc but the HRESULT shape still encodes them
        // unambiguously via the low 16 bits; callers treat any non-S_OK as "DNS failed".
        return HResultFromErrno(EHOSTUNREACH);
    }

    struct AddrInfoGuard
    {
        addrinfo* ptr;
        ~AddrInfoGuard() noexcept { if (ptr) { freeaddrinfo(ptr); } }
    } addrGuard{ resolved };

    if (resolved == nullptr)
    {
        return HResultFromErrno(EHOSTUNREACH);
    }

    int fd = socket(resolved->ai_family, resolved->ai_socktype, resolved->ai_protocol);
    if (fd < 0)
    {
        return HResultFromErrno(errno);
    }

    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0 || fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        int err = errno;
        close(fd);
        return HResultFromErrno(err);
    }

    sockaddr_storage dest{};
    socklen_t destLen = static_cast<socklen_t>(resolved->ai_addrlen);
    std::memcpy(&dest, resolved->ai_addr, resolved->ai_addrlen);

    UniquePtr<PosixUdpSocket> concrete = MakeUnique<PosixUdpSocket>(fd, dest, destLen);
    result = std::move(concrete);
    return S_OK;
}

} // namespace PlayFab

#endif // !HC_PLATFORM_IS_MICROSOFT
