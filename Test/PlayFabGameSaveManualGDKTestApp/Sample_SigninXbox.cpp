// Copyright (C) Microsoft Corporation. All rights reserved.
#include "pch.h"

Sample_GameSaveState* GetSampleGameSaveState();

HRESULT Sample_Xbox_AttemptSignIn()
{
    HRESULT hr;
    auto async = std::make_unique<XAsyncBlock>();
    bool ready = false;
    async->context = &ready;
    async->queue = nullptr; // use the default queue
    async->callback = [](XAsyncBlock* async)
    {
        std::unique_ptr<XAsyncBlock> asyncBlockPtr{ async }; // take ownership of XAsyncBlock
        Sample_GameSaveState* state = GetSampleGameSaveState();
        state->xuserResult = XUserAddResult(async, &state->xuser);
        state->currentState = SampleGameSaveState::SignIn;
    };
    hr = XUserAddAsync(XUserAddOptions::AddDefaultUserAllowingUI, async.get());
    LogToWindow("XUserAddAsync - sandbox should be XDKS.1 and sign in with test account via Xbox App");
    if (SUCCEEDED(hr))
    {
        async.release(); // at this point, the callback will be called so release the unique ptr
    }

    return S_OK;
}
