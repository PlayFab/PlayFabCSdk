// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

namespace PlayFab
{
namespace GameSave
{

// XAsyncProvider for PlayFab API calls
template<typename CallT>
class GameSaveAsyncProvider : public XAsyncProviderBase
{
public:
    template<size_t n>
    GameSaveAsyncProvider(RunContext&& rc, XAsyncBlock* async, const char(&identityName)[n]) :
        XAsyncProviderBase{ std::move(rc), async, identityName }
    {
    }

protected:
    HRESULT Begin(RunContext runContext) override
    {
        UNREFERENCED_PARAMETER(runContext);

        return Schedule(0);
    }

    std::optional<Result<CallT>> m_result;
};

} // namespace GameSave
} // namespace PlayFab