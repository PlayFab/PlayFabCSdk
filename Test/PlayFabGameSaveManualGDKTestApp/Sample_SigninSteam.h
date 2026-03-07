// Copyright (C) Microsoft Corporation. All rights reserved.
#include "pch.h"


#if STEAMWORKS_AVAILABLE
#include "steam/steam_api.h"

class LoginHandler
{
public:
    LoginHandler(XAsyncBlock* async, PFEntityHandle existingEntityHandle, PFLocalUserHandle localUserHandle, PFServiceConfigHandle serviceConfigHandle);

private:
    STEAM_CALLBACK(LoginHandler, OnGetSessionTicket, GetTicketForWebApiResponse_t);

    XAsyncBlock* m_asyncBlock;
    PFEntityHandle m_existingEntityHandle;
    PFLocalUserHandle m_localUserHandle;
    PFServiceConfigHandle m_serviceConfigHandle;
};

struct SteamGameState
{
    bool steamLoggingIn{ false };
    bool steamLinking{ false };
    bool steamWaitOnTicket{ false };
    LoginHandler loginHandler{ nullptr, nullptr, nullptr, nullptr };
    HAuthTicket steamAuthTicket{ k_HAuthTicketInvalid };
};
#endif

