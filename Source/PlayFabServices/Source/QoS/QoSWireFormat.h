// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <cstddef>

namespace PlayFab
{
namespace QoS
{

// Single QoS UDP exchange is exactly 10 bytes in each direction:
//   request:  0xFF 0xFF  <8-byte echo token>
//   response: 0x00 0x00  <same 8-byte echo token>
//
// The token is opaque to the server — it is echoed back unchanged. We use
// .NET DateTime.UtcNow.Ticks in little-endian byte order to match the
// canonical Thunderhead C# reference client (Thunderhead\src\Sandbox\QosTest\Program.cs).
// "Opaque to the server" is the current behavior; using UTC ticks is the
// forward-compatible choice in case the server ever adds validation.

constexpr std::size_t kQoSPacketSize = 10;
constexpr std::uint8_t kRequestMagic0 = 0xFF;
constexpr std::uint8_t kRequestMagic1 = 0xFF;
constexpr std::uint8_t kResponseMagic0 = 0x00;
constexpr std::uint8_t kResponseMagic1 = 0x00;

// Default UDP port for MPS QoS beacons. Today this is 3075 in every region.
constexpr std::uint16_t kQoSBeaconPort = 3075;

using QoSPacket = std::array<std::uint8_t, kQoSPacketSize>;

namespace detail
{

// .NET DateTime epoch (0001-01-01 UTC) is offset from the Unix epoch by exactly
// 621355968000000000 ticks of 100 ns. Adding this offset to a system_clock value
// in 100-ns ticks yields a .NET-compatible UTC tick count.
constexpr std::int64_t kDotNetEpochOffsetTicks = 621355968000000000LL;

inline std::uint64_t Load64LE(const std::uint8_t* p) noexcept
{
    return static_cast<std::uint64_t>(p[0])
         | (static_cast<std::uint64_t>(p[1]) << 8)
         | (static_cast<std::uint64_t>(p[2]) << 16)
         | (static_cast<std::uint64_t>(p[3]) << 24)
         | (static_cast<std::uint64_t>(p[4]) << 32)
         | (static_cast<std::uint64_t>(p[5]) << 40)
         | (static_cast<std::uint64_t>(p[6]) << 48)
         | (static_cast<std::uint64_t>(p[7]) << 56);
}

inline void Store64LE(std::uint64_t v, std::uint8_t* p) noexcept
{
    p[0] = static_cast<std::uint8_t>(v);
    p[1] = static_cast<std::uint8_t>(v >> 8);
    p[2] = static_cast<std::uint8_t>(v >> 16);
    p[3] = static_cast<std::uint8_t>(v >> 24);
    p[4] = static_cast<std::uint8_t>(v >> 32);
    p[5] = static_cast<std::uint8_t>(v >> 40);
    p[6] = static_cast<std::uint8_t>(v >> 48);
    p[7] = static_cast<std::uint8_t>(v >> 56);
}

} // namespace detail

// Generates a fresh 64-bit token derived from system_clock UTC ticks.
// Ticks are 100-nanosecond intervals since .NET epoch 0001-01-01 UTC.
inline std::uint64_t MakeEchoToken() noexcept
{
    using namespace std::chrono;

    // system_clock resolution varies by platform; cast through nanoseconds to be precise,
    // then divide by 100 to get .NET ticks.
    auto unixNs = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
    std::int64_t unixTicks = unixNs / 100;
    return static_cast<std::uint64_t>(unixTicks + detail::kDotNetEpochOffsetTicks);
}

// Encode an outbound ping packet. `out` is filled with exactly kQoSPacketSize bytes.
inline void EncodeRequest(std::uint64_t token, QoSPacket& out) noexcept
{
    out[0] = kRequestMagic0;
    out[1] = kRequestMagic1;
    detail::Store64LE(token, out.data() + 2);
}

// Validate an inbound packet:
//   - byteCount must equal kQoSPacketSize
//   - bytes[0..1] must be kResponseMagic0, kResponseMagic1
//   - bytes[2..9] must equal the expected token (little-endian decode)
//
// Returns true if the packet is a valid echo of `expectedToken`. The caller
// is responsible for verifying that the sender endpoint matches the destination —
// validating that requires the recvfrom result and lives in the socket layer.
inline bool ValidateResponse(const std::uint8_t* bytes, std::size_t byteCount, std::uint64_t expectedToken) noexcept
{
    if (byteCount != kQoSPacketSize || bytes == nullptr)
    {
        return false;
    }
    if (bytes[0] != kResponseMagic0 || bytes[1] != kResponseMagic1)
    {
        return false;
    }
    return detail::Load64LE(bytes + 2) == expectedToken;
}

} // namespace QoS
} // namespace PlayFab
