#pragma once

#include <playfab/services/cpp/AccountManagementTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

#if 0
class ClientAddOrUpdateContactEmailOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementAddOrUpdateContactEmailRequestWrapper<Allocator>;

    ClientAddOrUpdateContactEmailOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientAddUsernamePasswordOperation : public XAsyncOperation<Wrappers::PFAccountManagementAddUsernamePasswordResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementAddUsernamePasswordRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementAddUsernamePasswordResultWrapper<Allocator>;

    ClientAddUsernamePasswordOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementAddUsernamePasswordResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class ClientGetAccountInfoOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetAccountInfoResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetAccountInfoRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetAccountInfoResultWrapper<Allocator>;

    ClientGetAccountInfoOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetAccountInfoResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ClientGetPlayerCombinedInfoOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayerCombinedInfoResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayerCombinedInfoRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayerCombinedInfoResultWrapper<Allocator>;

    ClientGetPlayerCombinedInfoOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayerCombinedInfoResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ClientGetPlayerProfileOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayerProfileResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayerProfileRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayerProfileResultWrapper<Allocator>;

    ClientGetPlayerProfileOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayerProfileResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if 0
class ClientGetPlayFabIDsFromFacebookIDsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper<Allocator>;

    ClientGetPlayFabIDsFromFacebookIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientGetPlayFabIDsFromFacebookInstantGamesIdsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper<Allocator>;

    ClientGetPlayFabIDsFromFacebookInstantGamesIdsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientGetPlayFabIDsFromGameCenterIDsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper<Allocator>;

    ClientGetPlayFabIDsFromGameCenterIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientGetPlayFabIDsFromGoogleIDsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromGoogleIDsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper<Allocator>;

    ClientGetPlayFabIDsFromGoogleIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper<Allocator>;

    ClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientGetPlayFabIDsFromKongregateIDsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromKongregateIDsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper<Allocator>;

    ClientGetPlayFabIDsFromKongregateIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH
class ClientGetPlayFabIDsFromNintendoServiceAccountIdsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper<Allocator>;

    ClientGetPlayFabIDsFromNintendoServiceAccountIdsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper<Allocator>;

    ClientGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
class ClientGetPlayFabIDsFromPSNAccountIDsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper<Allocator>;

    ClientGetPlayFabIDsFromPSNAccountIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
class ClientGetPlayFabIDsFromSteamIDsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper<Allocator>;

    ClientGetPlayFabIDsFromSteamIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientGetPlayFabIDsFromTwitchIDsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper<Allocator>;

    ClientGetPlayFabIDsFromTwitchIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
class ClientGetPlayFabIDsFromXboxLiveIDsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper<Allocator>;

    ClientGetPlayFabIDsFromXboxLiveIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientLinkAndroidDeviceIDOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementLinkAndroidDeviceIDRequestWrapper<Allocator>;

    ClientLinkAndroidDeviceIDOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientLinkAppleOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementLinkAppleRequestWrapper<Allocator>;

    ClientLinkAppleOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class ClientLinkCustomIDOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementLinkCustomIDRequestWrapper<Allocator>;

    ClientLinkCustomIDOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if 0
class ClientLinkFacebookAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementLinkFacebookAccountRequestWrapper<Allocator>;

    ClientLinkFacebookAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientLinkFacebookInstantGamesIdOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementLinkFacebookInstantGamesIdRequestWrapper<Allocator>;

    ClientLinkFacebookInstantGamesIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientLinkGameCenterAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementLinkGameCenterAccountRequestWrapper<Allocator>;

    ClientLinkGameCenterAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientLinkGoogleAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementLinkGoogleAccountRequestWrapper<Allocator>;

    ClientLinkGoogleAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientLinkGooglePlayGamesServicesAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementLinkGooglePlayGamesServicesAccountRequestWrapper<Allocator>;

    ClientLinkGooglePlayGamesServicesAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientLinkIOSDeviceIDOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementLinkIOSDeviceIDRequestWrapper<Allocator>;

    ClientLinkIOSDeviceIDOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientLinkKongregateOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementLinkKongregateAccountRequestWrapper<Allocator>;

    ClientLinkKongregateOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH
class ClientLinkNintendoServiceAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementClientLinkNintendoServiceAccountRequestWrapper<Allocator>;

    ClientLinkNintendoServiceAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientLinkNintendoSwitchDeviceIdOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementClientLinkNintendoSwitchDeviceIdRequestWrapper<Allocator>;

    ClientLinkNintendoSwitchDeviceIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class ClientLinkOpenIdConnectOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementLinkOpenIdConnectRequestWrapper<Allocator>;

    ClientLinkOpenIdConnectOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
class ClientLinkPSNAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementClientLinkPSNAccountRequestWrapper<Allocator>;

    ClientLinkPSNAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
class ClientLinkSteamAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementLinkSteamAccountRequestWrapper<Allocator>;

    ClientLinkSteamAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientLinkTwitchOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementLinkTwitchAccountRequestWrapper<Allocator>;

    ClientLinkTwitchOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
class ClientLinkXboxAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementClientLinkXboxAccountRequestWrapper<Allocator>;

    ClientLinkXboxAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientRemoveContactEmailOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementRemoveContactEmailRequestWrapper<Allocator>;

    ClientRemoveContactEmailOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class ClientReportPlayerOperation : public XAsyncOperation<Wrappers::PFAccountManagementReportPlayerClientResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementReportPlayerClientRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementReportPlayerClientResultWrapper<Allocator>;

    ClientReportPlayerOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementReportPlayerClientResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if 0
class ClientSendAccountRecoveryEmailOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementSendAccountRecoveryEmailRequestWrapper<Allocator>;

    ClientSendAccountRecoveryEmailOperation(ServiceConfig serviceConfig, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(ServiceConfig serviceConfig, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    ServiceConfig m_serviceConfig;
    RequestType m_request;
};
#endif

#if 0
class ClientUnlinkAndroidDeviceIDOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementUnlinkAndroidDeviceIDRequestWrapper<Allocator>;

    ClientUnlinkAndroidDeviceIDOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientUnlinkAppleOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementUnlinkAppleRequestWrapper<Allocator>;

    ClientUnlinkAppleOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class ClientUnlinkCustomIDOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementUnlinkCustomIDRequestWrapper<Allocator>;

    ClientUnlinkCustomIDOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if 0
class ClientUnlinkFacebookAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementUnlinkFacebookAccountRequestWrapper<Allocator>;

    ClientUnlinkFacebookAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientUnlinkFacebookInstantGamesIdOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementUnlinkFacebookInstantGamesIdRequestWrapper<Allocator>;

    ClientUnlinkFacebookInstantGamesIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientUnlinkGameCenterAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementUnlinkGameCenterAccountRequestWrapper<Allocator>;

    ClientUnlinkGameCenterAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientUnlinkGoogleAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementUnlinkGoogleAccountRequestWrapper<Allocator>;

    ClientUnlinkGoogleAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientUnlinkGooglePlayGamesServicesAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequestWrapper<Allocator>;

    ClientUnlinkGooglePlayGamesServicesAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientUnlinkIOSDeviceIDOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementUnlinkIOSDeviceIDRequestWrapper<Allocator>;

    ClientUnlinkIOSDeviceIDOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientUnlinkKongregateOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementUnlinkKongregateAccountRequestWrapper<Allocator>;

    ClientUnlinkKongregateOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH
class ClientUnlinkNintendoServiceAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementClientUnlinkNintendoServiceAccountRequestWrapper<Allocator>;

    ClientUnlinkNintendoServiceAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientUnlinkNintendoSwitchDeviceIdOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequestWrapper<Allocator>;

    ClientUnlinkNintendoSwitchDeviceIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class ClientUnlinkOpenIdConnectOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementUnlinkOpenIdConnectRequestWrapper<Allocator>;

    ClientUnlinkOpenIdConnectOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
class ClientUnlinkPSNAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementClientUnlinkPSNAccountRequestWrapper<Allocator>;

    ClientUnlinkPSNAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
class ClientUnlinkSteamAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementUnlinkSteamAccountRequestWrapper<Allocator>;

    ClientUnlinkSteamAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientUnlinkTwitchOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementUnlinkTwitchAccountRequestWrapper<Allocator>;

    ClientUnlinkTwitchOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
class ClientUnlinkXboxAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementClientUnlinkXboxAccountRequestWrapper<Allocator>;

    ClientUnlinkXboxAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class ClientUpdateAvatarUrlOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementClientUpdateAvatarUrlRequestWrapper<Allocator>;

    ClientUpdateAvatarUrlOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if 0
class ClientUpdateUserTitleDisplayNameOperation : public XAsyncOperation<Wrappers::PFAccountManagementUpdateUserTitleDisplayNameResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementUpdateUserTitleDisplayNameRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementUpdateUserTitleDisplayNameResultWrapper<Allocator>;

    ClientUpdateUserTitleDisplayNameOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementUpdateUserTitleDisplayNameResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerBanUsersOperation : public XAsyncOperation<Wrappers::PFAccountManagementBanUsersResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementBanUsersRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementBanUsersResultWrapper<Allocator>;

    ServerBanUsersOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementBanUsersResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerDeletePlayerOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementDeletePlayerRequestWrapper<Allocator>;

    ServerDeletePlayerOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetPlayerCombinedInfoOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayerCombinedInfoResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayerCombinedInfoRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayerCombinedInfoResultWrapper<Allocator>;

    ServerGetPlayerCombinedInfoOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayerCombinedInfoResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetPlayerProfileOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayerProfileResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayerProfileRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayerProfileResultWrapper<Allocator>;

    ServerGetPlayerProfileOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayerProfileResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetPlayFabIDsFromFacebookIDsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper<Allocator>;

    ServerGetPlayFabIDsFromFacebookIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetPlayFabIDsFromFacebookInstantGamesIdsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper<Allocator>;

    ServerGetPlayFabIDsFromFacebookInstantGamesIdsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetPlayFabIDsFromNintendoServiceAccountIdsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper<Allocator>;

    ServerGetPlayFabIDsFromNintendoServiceAccountIdsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper<Allocator>;

    ServerGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetPlayFabIDsFromPSNAccountIDsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper<Allocator>;

    ServerGetPlayFabIDsFromPSNAccountIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetPlayFabIDsFromSteamIDsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper<Allocator>;

    ServerGetPlayFabIDsFromSteamIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetPlayFabIDsFromTwitchIDsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper<Allocator>;

    ServerGetPlayFabIDsFromTwitchIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetPlayFabIDsFromXboxLiveIDsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper<Allocator>;

    ServerGetPlayFabIDsFromXboxLiveIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetServerCustomIDsFromPlayFabIDsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper<Allocator>;

    ServerGetServerCustomIDsFromPlayFabIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetUserAccountInfoOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetUserAccountInfoResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetUserAccountInfoRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetUserAccountInfoResultWrapper<Allocator>;

    ServerGetUserAccountInfoOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetUserAccountInfoResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetUserBansOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetUserBansResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetUserBansRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetUserBansResultWrapper<Allocator>;

    ServerGetUserBansOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetUserBansResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerLinkNintendoServiceAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementServerLinkNintendoServiceAccountRequestWrapper<Allocator>;

    ServerLinkNintendoServiceAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerLinkNintendoSwitchDeviceIdOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementServerLinkNintendoSwitchDeviceIdRequestWrapper<Allocator>;

    ServerLinkNintendoSwitchDeviceIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerLinkPSNAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementServerLinkPSNAccountRequestWrapper<Allocator>;

    ServerLinkPSNAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerLinkPSNIdOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementLinkPSNIdRequestWrapper<Allocator>;

    ServerLinkPSNIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerLinkServerCustomIdOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementLinkServerCustomIdRequestWrapper<Allocator>;

    ServerLinkServerCustomIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerLinkSteamIdOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementLinkSteamIdRequestWrapper<Allocator>;

    ServerLinkSteamIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerLinkXboxAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementServerLinkXboxAccountRequestWrapper<Allocator>;

    ServerLinkXboxAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerRevokeAllBansForUserOperation : public XAsyncOperation<Wrappers::PFAccountManagementRevokeAllBansForUserResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementRevokeAllBansForUserRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementRevokeAllBansForUserResultWrapper<Allocator>;

    ServerRevokeAllBansForUserOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementRevokeAllBansForUserResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerRevokeBansOperation : public XAsyncOperation<Wrappers::PFAccountManagementRevokeBansResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementRevokeBansRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementRevokeBansResultWrapper<Allocator>;

    ServerRevokeBansOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementRevokeBansResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerSendCustomAccountRecoveryEmailOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementSendCustomAccountRecoveryEmailRequestWrapper<Allocator>;

    ServerSendCustomAccountRecoveryEmailOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerSendEmailFromTemplateOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementSendEmailFromTemplateRequestWrapper<Allocator>;

    ServerSendEmailFromTemplateOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerUnlinkNintendoServiceAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementServerUnlinkNintendoServiceAccountRequestWrapper<Allocator>;

    ServerUnlinkNintendoServiceAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerUnlinkNintendoSwitchDeviceIdOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequestWrapper<Allocator>;

    ServerUnlinkNintendoSwitchDeviceIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerUnlinkPSNAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementServerUnlinkPSNAccountRequestWrapper<Allocator>;

    ServerUnlinkPSNAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerUnlinkServerCustomIdOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementUnlinkServerCustomIdRequestWrapper<Allocator>;

    ServerUnlinkServerCustomIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerUnlinkSteamIdOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementUnlinkSteamIdRequestWrapper<Allocator>;

    ServerUnlinkSteamIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerUnlinkXboxAccountOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementServerUnlinkXboxAccountRequestWrapper<Allocator>;

    ServerUnlinkXboxAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerUpdateAvatarUrlOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAccountManagementServerUpdateAvatarUrlRequestWrapper<Allocator>;

    ServerUpdateAvatarUrlOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerUpdateBansOperation : public XAsyncOperation<Wrappers::PFAccountManagementUpdateBansResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementUpdateBansRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementUpdateBansResultWrapper<Allocator>;

    ServerUpdateBansOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementUpdateBansResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class GetTitlePlayersFromXboxLiveIDsOperation : public XAsyncOperation<Wrappers::PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper<Allocator>;

    GetTitlePlayersFromXboxLiveIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

}
}