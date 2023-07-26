#pragma once

#include <playfab/services/PFServices.h>
#include "HttpClient.h"

// Forward declaration of HCInitArgs
struct HCInitArgs;

namespace PlayFab
{

class GlobalState : public ITerminationListener
{
public:
    virtual ~GlobalState() noexcept;

    static HRESULT Create(_In_opt_ HCInitArgs* args) noexcept;
    static HRESULT Get(SharedPtr<GlobalState>& state) noexcept;
    static HRESULT CleanupAsync(XAsyncBlock* async) noexcept;

public:
    RunContext RunContext() const noexcept;

private:
    GlobalState(bool uninitPlayFabCore) noexcept;

    void OnTerminated(void* context) noexcept override;
    static HRESULT CALLBACK CleanupAsyncProvider(XAsyncOp op, XAsyncProviderData const* data);

    PlayFab::RunContext m_runContext;
    bool m_uninitPlayFabCore{ false };
};

}
