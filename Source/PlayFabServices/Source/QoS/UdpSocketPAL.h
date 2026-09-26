// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "Memory.h"
#include "Result.h"
#include "CancellationToken.h"

#include <array>
#include <chrono>
#include <cstdint>

namespace PlayFab
{

// Cross-platform equivalent of Win32's HRESULT_FROM_WIN32(WAIT_TIMEOUT). WAIT_TIMEOUT (0x102)
// is a Win32-only macro unavailable on POSIX platforms, so both UdpSocket::Exchange()
// implementations (Microsoft/UdpSocketPAL_Windows.cpp, UdpSocketPAL_Posix.cpp) and any
// caller that needs to synthesize the same "timed out" HRESULT (e.g. QoS.cpp) share this
// single precomputed literal instead of each re-deriving it.
constexpr HRESULT kUdpSocketTimeoutHr = static_cast<HRESULT>(0x80070102L);

// Connectionless UDP socket abstraction used by the QoS region-ping feature.
// Win32/GDK uses Winsock2; Linux/Android/iOS/macOS use BSD sockets + poll.
// All implementations are blocking on the calling thread for the duration of a single
// Exchange() call but cooperate with PlayFab::CancellationToken — the inner poll loop
// wakes at most every kCancelPollIntervalMs to check the token.
class UdpSocket
{
public:
    // Maximum bytes accepted by Exchange in either direction. Chosen to match the QoS
    // wire format (10 bytes); the socket itself happily handles up to ~65507.
    static constexpr std::size_t kMaxDatagramSize = 1500;

    // How often the inner poll loop unblocks to check cancellation. Trades cancel-latency
    // against syscall overhead. 50 ms is a reasonable default given typical ping timeouts
    // of 250-1000 ms.
    static constexpr std::chrono::milliseconds kCancelPollInterval{ 50 };

    // Resolves serverHost via getaddrinfo (blocking) and opens a connected UDP socket
    // toward serverHost:port. Caller may pre-resolve to amortize DNS cost across iterations.
    static HRESULT Connect(
        const char* serverHost, 
        std::uint16_t port,
        UniquePtr<UdpSocket>& result
    ) noexcept;

    virtual ~UdpSocket() noexcept = default;

    // Sends `request` (requestSize bytes, <= kMaxDatagramSize) and waits up to `timeout`
    // for a reply. For each inbound datagram, the socket verifies that the sender endpoint
    // matches the connected destination, then invokes the validator callback. If the
    // validator returns true, Exchange returns the wall-clock RTT in microseconds.
    // Otherwise the packet is silently dropped and Exchange keeps waiting.
    //
    // Errors:
    //   kUdpSocketTimeoutHr                no acceptable packet arrived in time
    //   E_ABORT                            cancel was raised on the supplied CancellationToken
    //   other                              underlying socket failure
    //                                      (HRESULT_FROM_WIN32(WSAGetLastError()) on Windows;
    //                                       HRESULT_FROM_WIN32(errno) on POSIX)
    using ValidatorFn = bool (*)(void* ctx, const std::uint8_t* bytes, std::size_t size);
    virtual HRESULT Exchange(
        const std::uint8_t* request,
        std::size_t requestSize,
        std::chrono::milliseconds timeout,
        ValidatorFn validator,
        void* validatorContext,
        CancellationToken const& cancel,
        std::chrono::microseconds& result
    ) noexcept = 0;

protected:
    UdpSocket() noexcept = default;
    UdpSocket(UdpSocket const&) = delete;
    UdpSocket& operator=(UdpSocket const&) = delete;
};

} // namespace PlayFab
