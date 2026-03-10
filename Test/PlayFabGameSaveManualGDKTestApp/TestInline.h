// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

class TestInlineClass
{
public:
    TestInlineClass() noexcept(false);
    ~TestInlineClass();

    static HRESULT TestGameSaveAPIAddUser(XUserHandle xuser);
    static HRESULT TestLinkGDKGameSaveAPI();
};
