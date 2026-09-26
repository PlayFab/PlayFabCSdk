// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "TestAppPch.h"
#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && !HC_PLATFORM_IS_PLAYSTATION
#include "CppUnitTest.h"
#include "QoS\QoSWireFormat.h"

#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace PlayFab::QoS;

namespace PlayFab
{
namespace UnitTests
{

TEST_CLASS(QoSWireFormatTests)
{
public:
    TEST_METHOD(EncodeRequest_FillsMagicBytes)
    {
        QoSPacket pkt{};
        EncodeRequest(0, pkt);
        Assert::AreEqual<uint8_t>(kRequestMagic0, pkt[0]);
        Assert::AreEqual<uint8_t>(kRequestMagic1, pkt[1]);
    }

    TEST_METHOD(EncodeRequest_TokenIsLittleEndian)
    {
        // 0x0123456789ABCDEF in LE = EF CD AB 89 67 45 23 01
        QoSPacket pkt{};
        EncodeRequest(0x0123456789ABCDEFull, pkt);
        Assert::AreEqual<uint8_t>(0xEF, pkt[2]);
        Assert::AreEqual<uint8_t>(0xCD, pkt[3]);
        Assert::AreEqual<uint8_t>(0xAB, pkt[4]);
        Assert::AreEqual<uint8_t>(0x89, pkt[5]);
        Assert::AreEqual<uint8_t>(0x67, pkt[6]);
        Assert::AreEqual<uint8_t>(0x45, pkt[7]);
        Assert::AreEqual<uint8_t>(0x23, pkt[8]);
        Assert::AreEqual<uint8_t>(0x01, pkt[9]);
    }

    TEST_METHOD(Validate_AcceptsCorrectResponse)
    {
        // Build a valid response packet for token T: 0x00 0x00 <T LE>
        std::uint64_t const token = 0xFEEDFACEDEADBEEFull;
        std::array<std::uint8_t, kQoSPacketSize> resp{
            kResponseMagic0, kResponseMagic1,
            0xEF, 0xBE, 0xAD, 0xDE, 0xCE, 0xFA, 0xED, 0xFE
        };
        Assert::IsTrue(ValidateResponse(resp.data(), resp.size(), token));
    }

    TEST_METHOD(Validate_RejectsShortPacket)
    {
        // 9 bytes - one byte short of a valid packet, must not over-read.
        std::array<std::uint8_t, 9> shortPkt{ 0, 0, 1, 2, 3, 4, 5, 6, 7 };
        Assert::IsFalse(ValidateResponse(shortPkt.data(), shortPkt.size(), 0));
    }

    TEST_METHOD(Validate_RejectsLongPacket)
    {
        // 11 bytes - one byte longer than expected; reject.
        std::array<std::uint8_t, 11> longPkt{ 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        Assert::IsFalse(ValidateResponse(longPkt.data(), longPkt.size(), 0));
    }

    TEST_METHOD(Validate_RejectsZeroLength)
    {
        std::uint8_t dummy = 0;
        Assert::IsFalse(ValidateResponse(&dummy, 0, 0));
    }

    TEST_METHOD(Validate_RejectsNullBuffer)
    {
        // byteCount valid but pointer null - must not deref.
        Assert::IsFalse(ValidateResponse(nullptr, kQoSPacketSize, 0));
    }

    TEST_METHOD(Validate_RejectsWrongMagic)
    {
        // Wrong header bytes (0xFF 0xFF = request magic, not response).
        std::array<std::uint8_t, kQoSPacketSize> bad{
            0xFF, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0
        };
        Assert::IsFalse(ValidateResponse(bad.data(), bad.size(), 0));
    }

    TEST_METHOD(Validate_RejectsPartialMagic)
    {
        // Only first magic byte correct
        std::array<std::uint8_t, kQoSPacketSize> bad{
            kResponseMagic0, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0
        };
        Assert::IsFalse(ValidateResponse(bad.data(), bad.size(), 0));
    }

    TEST_METHOD(Validate_RejectsWrongToken)
    {
        // Response is well-formed but echoes a token the caller did not send.
        // Critical security check: defends against spoofed echoes from other ports/hosts.
        std::array<std::uint8_t, kQoSPacketSize> resp{
            kResponseMagic0, kResponseMagic1,
            0xEF, 0xBE, 0xAD, 0xDE, 0xCE, 0xFA, 0xED, 0xFE
        };
        // Expected token differs in low byte only.
        Assert::IsFalse(ValidateResponse(resp.data(), resp.size(), 0xFEEDFACEDEADBEEEull));
    }

    TEST_METHOD(Validate_RejectsAllZeroTokenWhenNonZeroExpected)
    {
        // Easy spoof attempt: send a properly-shaped response with token=0.
        std::array<std::uint8_t, kQoSPacketSize> resp{
            kResponseMagic0, kResponseMagic1, 0, 0, 0, 0, 0, 0, 0, 0
        };
        Assert::IsFalse(ValidateResponse(resp.data(), resp.size(), 1));
        Assert::IsTrue(ValidateResponse(resp.data(), resp.size(), 0));
    }

    TEST_METHOD(EncodeRequest_RoundTrip_ValidatesAsResponse)
    {
        // Encode request with token T, then build the corresponding response
        // (same token, response magic) and confirm Validate accepts.
        std::uint64_t const token = MakeEchoToken();

        QoSPacket req{};
        EncodeRequest(token, req);

        // Server logic: flip magic bytes from request to response, leave token bytes alone.
        QoSPacket resp = req;
        resp[0] = kResponseMagic0;
        resp[1] = kResponseMagic1;

        Assert::IsTrue(ValidateResponse(resp.data(), resp.size(), token));
    }

    TEST_METHOD(MakeEchoToken_IsAboveDotNetEpochOffset)
    {
        // The token is unix-time-since-epoch ticks plus the .NET epoch offset.
        // Any value after 0001-01-01 UTC must exceed the offset.
        constexpr std::uint64_t kDotNetEpochOffsetTicks = 621355968000000000ull;
        Assert::IsTrue(MakeEchoToken() > kDotNetEpochOffsetTicks);
    }

    TEST_METHOD(MakeEchoToken_MonotonicAcrossCalls)
    {
        // Two calls separated by a sleep must produce non-decreasing tokens.
        // (Not strictly increasing - same tick is possible on platforms with
        // coarse clocks if called back-to-back. The sleep is the safety margin.)
        std::uint64_t t0 = MakeEchoToken();
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        std::uint64_t t1 = MakeEchoToken();
        Assert::IsTrue(t1 > t0);
    }

    TEST_METHOD(PacketSize_IsTenBytes)
    {
        // The wire format is fixed at 10 bytes. Lock it down so a refactor
        // can't accidentally drift away from the Thunderhead reference client.
        Assert::AreEqual<size_t>(10, kQoSPacketSize);
        Assert::AreEqual<size_t>(10, sizeof(QoSPacket));
    }

    TEST_METHOD(MagicBytes_MatchProtocol)
    {
        // Lock down magic byte values - request 0xFF 0xFF, response 0x00 0x00.
        Assert::AreEqual<uint8_t>(0xFF, kRequestMagic0);
        Assert::AreEqual<uint8_t>(0xFF, kRequestMagic1);
        Assert::AreEqual<uint8_t>(0x00, kResponseMagic0);
        Assert::AreEqual<uint8_t>(0x00, kResponseMagic1);
    }
};

}
}
#endif // HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && !HC_PLATFORM_IS_PLAYSTATION
