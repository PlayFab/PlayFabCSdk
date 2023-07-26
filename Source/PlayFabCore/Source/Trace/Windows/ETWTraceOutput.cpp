#include "stdafx.h"
#include "ETWTraceOutput.h"

TRACELOGGING_DEFINE_PROVIDER(
    g_hTraceLoggingProvider,
    "PlayFabCore",
    (0x2b6274b2, 0x159f, 0x4e6f, 0xb8, 0x0, 0xd, 0x17, 0x0, 0x9e, 0x14, 0x7a) // {2B6274B2-159F-4E6F-B800-0D17009E147A}
);

namespace PlayFab
{

// Helper method to ensure our ETW Provider never gets registered twice. Registering an ETW provider twice may result
// in a crash or undefined behavior.  See https://learn.microsoft.com/en-us/windows/win32/api/traceloggingprovider/nf-traceloggingprovider-traceloggingregister
// for more details.
void RegisterProvider()
{
    struct ProviderRegistrar
    {
        ProviderRegistrar()
        {
            TraceLoggingRegister(g_hTraceLoggingProvider);
        }
        ~ProviderRegistrar()
        {
            TraceLoggingUnregister(g_hTraceLoggingProvider);
        }
    };

    static ProviderRegistrar s_registrar{};
}

ETWTraceOutput::ETWTraceOutput()
{
    RegisterProvider();
}

void ETWTraceOutput::TraceMessage(const char* message) noexcept
{
    TraceLoggingWrite(
        g_hTraceLoggingProvider,
        "PlayFabCore_TraceMessage",
        TraceLoggingString(message, "Message")
    );
}

}
