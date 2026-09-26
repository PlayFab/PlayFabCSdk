// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//
// Hand-written test class for the QoS region-ping public API
// (PFMultiplayerServerPingQosServersAsync). Layer-2 integration test that exercises the full
// chain: ListQosServersForTitle, UDP/3075 ping, echo validation, aggregation, buffer packing.
//
// NOT generated. Lives in TestApp Common file list and is registered directly in TestRunner.

#pragma once

#include "ServicesTestClass.h"

namespace PlayFab
{
namespace Test
{

class MultiplayerServerQoSTests : public ServicesTestClass
{
public:
    using ServicesTestClass::ServicesTestClass;

private:
    void AddTests() override;

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
    void TestPingQosServersDefault(TestContext& testContext);
    void TestPingQosServersWithTunedOptions(TestContext& testContext);
    void TestPingQosServersInternetRouting(TestContext& testContext);
    void TestPingQosServersIncludeAllRegions(TestContext& testContext);
    void TestPingQosServersBadArgNullEntityHandle(TestContext& testContext);
    void TestPingQosServersGetResultBadArgs(TestContext& testContext);
    void TestPingQosServersCancel(TestContext& testContext);
    void TestUdpSocketLoopback(TestContext& testContext);
#endif
};

}
}
