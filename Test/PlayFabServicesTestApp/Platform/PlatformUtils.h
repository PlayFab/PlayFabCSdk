#pragma once

#include "TestTypes.h"

namespace PlayFab
{
namespace Test
{
namespace Platform
{

HRESULT GetTestTitleData(
    TestTitleData& testTitleData
) noexcept;

time_t GetSystemTime() noexcept;

String GetTimeString() noexcept;

void TraceMessage(
    PFHCTraceLevel level,
    String const& message
) noexcept;

void Sleep(
    uint64_t durationMs
) noexcept;

}
}
}
