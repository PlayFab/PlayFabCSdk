#include "stdafx.h"
#include "LocalUser.h"
#include <playfab/core/PFPlatformAndroid.h>
#include "PlatformComponents_Android.h"
#include <Generated/Authentication.h>
#include "JsonUtils.h"

namespace PlayFab
{

class AndroidLocalUser : public LocalUser
{
public:
    static Result<SharedPtr<AndroidLocalUser>> Make(
        _In_ SharedPtr<ServiceConfig> serviceConfig,
        _In_ PFLocalUserAndroidContext const& androidContext,
        _In_opt_ void* clientContext
    );

    ~AndroidLocalUser();

    String LocalId() const override;
    HRESULT AndroidContext(PFLocalUserAndroidContext& androidContext) const;
    SharedPtr<Authentication::ILoginHandler> LoginHandler() const noexcept override;

private:
    AndroidLocalUser(
        SharedPtr<ServiceConfig> serviceConfig,
        void* clientContext,
        SharedPtr<PlatformComponents_Android> platformComponents,
        String playerId,
        jobject signInClient,
        String OAuth2WebClientId
    );

    friend class AndroidLocalUserLoginHandler;

    SharedPtr<PlatformComponents_Android> m_platformComponents;
    String m_playerId;
    jobject m_signinClient;
    String m_OAuth2WebClientId;

    SharedPtr<class AndroidLocalUserLoginHandler> m_loginHandler;
};

class AndroidLocalUserLoginHandler : public Authentication::ILoginHandler
{
public:
    AndroidLocalUserLoginHandler(SharedPtr<AndroidLocalUser> user);

    // ILoginHandler
    AsyncOp<Authentication::CombinedLoginResult> Login(
        SharedPtr<PFCoreGlobalState> state,
        SharedPtr<ServiceConfig> serviceConfig,
        bool createAccount,
        RunContext rc
    ) noexcept override;

    AsyncOp<void> ReLogin(
        SharedPtr<PlayFab::Entity> entity,
        RunContext rc
    ) noexcept override;

private:
    WeakPtr<AndroidLocalUser> m_localUser;
};

class GetServerAuthTokenContext
{
public:
    GetServerAuthTokenContext(SharedPtr<AndroidLocalUser> localUser);

    SharedPtr<AndroidLocalUser> const localUser;
    SharedPtr<AsyncOpContext<String>> const asyncContext;
};

GetServerAuthTokenContext::GetServerAuthTokenContext(SharedPtr<AndroidLocalUser> localUser) :
    localUser{ localUser },
    asyncContext{ MakeShared<AsyncOpContext<String>>() }
{
}

Result<SharedPtr<LocalUser>> MakePlatformUser(
    _In_ SharedPtr<ServiceConfig> serviceConfig,
    _In_ PFLocalUserAndroidContext const& androidContext,
    _In_opt_ void* clientContext
)
{
    auto makeUserResult = AndroidLocalUser::Make(serviceConfig, androidContext, clientContext);
    RETURN_IF_FAILED(makeUserResult.hr);
    return Result<SharedPtr<LocalUser>>{ makeUserResult.ExtractPayload() };
}

Result<SharedPtr<AndroidLocalUser>> AndroidLocalUser::Make(
    SharedPtr<PlayFab::ServiceConfig> serviceConfig,
    const PFLocalUserAndroidContext &androidContext,
    void *clientContext
    )
{
    // TODO would be cleaner to provide state as a parameter here
    SharedPtr<PFCoreGlobalState> state;
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    bool isThreadAttached = false;
    JNIEnv* jniEnv = nullptr;
    JavaVM* jvm = state->PlatformComponents()->GetJavaVm();

    if (jvm == nullptr)
    {
        TRACE_ERROR("JVM null");
        return E_UNEXPECTED;
    }

    jint jniResult = jvm->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_6);
    if (jniResult == JNI_EDETACHED)
    {
        TRACE_VERBOSE("Unable to get because thread is detached");
        jint attachThreadResult = jvm->AttachCurrentThread(&jniEnv, nullptr);

        if (attachThreadResult != JNI_OK)
        {
            TRACE_ERROR("Unable to get JNIEnv, jniResult=%u", jniResult);
            return E_FAIL;
        }

        isThreadAttached = true;
    }
    else if (jniResult != JNI_OK || jniEnv == nullptr)
    {
        TRACE_ERROR("Unable to JNIEnv, jniResult=%u", jniResult);
        return E_FAIL;
    }

    jobject signInClientGlobalRef = reinterpret_cast<jclass>(jniEnv->NewGlobalRef(androidContext.signinClient));

    Allocator<AndroidLocalUser> a;
    SharedPtr<AndroidLocalUser> user{ new (a.allocate(1)) AndroidLocalUser(
            serviceConfig,
            clientContext,
            state->PlatformComponents(),
            androidContext.playerId,
            signInClientGlobalRef,
            androidContext.OAuth2WebClientId
        )
    };
    user->m_loginHandler = MakeShared<AndroidLocalUserLoginHandler>(user);

    if (isThreadAttached)
    {
        jvm->DetachCurrentThread();
    }

    return std::move(user);
}

AndroidLocalUser::AndroidLocalUser(
    SharedPtr<ServiceConfig> serviceConfig,
    void* clientContext,
    SharedPtr<PlatformComponents_Android> platformComponents,
    String playerId,
    jobject signInClient,
    String OAuthId
) :
    LocalUser{std::move(serviceConfig), clientContext},
    m_platformComponents{ std::move(platformComponents) },
    m_playerId{ std::move(playerId) },
    m_signinClient{ signInClient },
    m_OAuth2WebClientId{ OAuthId }
{
}

AndroidLocalUser::~AndroidLocalUser()
{
    JavaVM* jvm = m_platformComponents->GetJavaVm();
    JNIEnv* jniEnv = nullptr;
    bool isThreadAttached = false;
    jint getEnvResult = jvm->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_6);

    if (getEnvResult == JNI_EDETACHED)
    {
        jint attachThreadResult = jvm->AttachCurrentThread(&jniEnv, nullptr);
        if (attachThreadResult == JNI_OK)
        {
            isThreadAttached = true;
        }
    }

    if (jniEnv != nullptr)
    {
        jniEnv->DeleteGlobalRef(m_signinClient);
    }

    if (isThreadAttached)
    {
        jvm->DetachCurrentThread();
    }
}

String AndroidLocalUser::LocalId() const
{
    return m_playerId;
}

HRESULT AndroidLocalUser::AndroidContext(PFLocalUserAndroidContext& androidContext) const
{
    androidContext.playerId = m_playerId.data();
    androidContext.signinClient = m_signinClient;
    androidContext.OAuth2WebClientId = m_OAuth2WebClientId.data();
    return S_OK;
}

SharedPtr<Authentication::ILoginHandler> AndroidLocalUser::LoginHandler() const noexcept
{
    return m_loginHandler;
};

AndroidLocalUserLoginHandler::AndroidLocalUserLoginHandler(
    SharedPtr<AndroidLocalUser> user
) :
    m_localUser{ user }
{
}

AsyncOp<Authentication::CombinedLoginResult> AndroidLocalUserLoginHandler::Login(
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<ServiceConfig> serviceConfig,
    bool createAccount,
    RunContext rc
) noexcept
{
    SharedPtr<AndroidLocalUser> localUser{ m_localUser.lock() };
    if (!localUser)
    {
        TRACE_INFORMATION("Unable to login because LocalUser was destroyed");
        return E_FAIL;
    }

    bool isThreadAttached = false;
    JNIEnv* jniEnv = nullptr;
    JavaVM* jvm = state->PlatformComponents()->GetJavaVm();

    jint jniResult = jvm->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_6);

    if (jniResult == JNI_EDETACHED)
    {
        TRACE_ERROR("Unable to get because thread is detached");
        jint attachThreadResult = jvm->AttachCurrentThread(&jniEnv, nullptr);

        if (attachThreadResult != JNI_OK)
        {
            TRACE_ERROR("Unable to get JNIEnv, jniResult=%u", jniResult);
            return E_FAIL;
        }

        isThreadAttached = true;
    }
    else if (jniResult != JNI_OK || jniEnv == nullptr)
    {
        TRACE_ERROR("Unable to JNIEnv, jniResult=%u", jniResult);
        return E_FAIL;
    }

    jmethodID getTokenMethodId = jniEnv->GetStaticMethodID(
        state->PlatformComponents()->GetLocalUserClass(),
        "GetServerAuthTokenAsync",
        "(Lcom/google/android/gms/games/GamesSignInClient;Ljava/lang/String;J)V"
    );

    if (getTokenMethodId == NULL)
    {
        TRACE_ERROR("Unable to get GetServerAuthTokenAsync methodId");
        return E_FAIL;
    }

    UniquePtr<GetServerAuthTokenContext> context{ MakeUnique<GetServerAuthTokenContext>(localUser) };

    jstring oauthToken = jniEnv->NewStringUTF(localUser->m_OAuth2WebClientId.data());
    jniEnv->CallStaticVoidMethod(state->PlatformComponents()->GetLocalUserClass(), getTokenMethodId, localUser->m_signinClient, oauthToken, reinterpret_cast<jlong>(context.get()));

    if (isThreadAttached)
    {
        jvm->DetachCurrentThread();
    }

    auto op = AsyncOp<String>{ context->asyncContext }.Then([serviceConfig, rc](Result<String> result) mutable -> AsyncOp<ServiceResponse>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        TRACE_VERBOSE("Got token %s, making PlayFab Login call", result.Payload().data());
        Authentication::LoginWithGooglePlayGamesServicesRequest request;
        request.SetCreateAccount(true);
        request.SetServerAuthCode(result.ExtractPayload());

        JsonValue requestBody = request.ToJson();
        RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));

        return serviceConfig->HttpClient()->MakePostRequest(
            "/Client/LoginWithGooglePlayGamesServices",
            UnorderedMap<String, String>{},
            std::move(requestBody),
            CacheId::AuthenticationLoginWithGooglePlayGamesServices,
            rc.Derive()
            );

    }).Then([state{ std::move(state) }, serviceConfig, loginHandler{ localUser->LoginHandler() } ](Result<ServiceResponse> result)->Result<Authentication::CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode == 200)
        {
            return Authentication::CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginHandler);
        }
        else
        {
            return Result<Authentication::CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });

    context.release(); // reclaimed in JNI callback
    return op;
}

AsyncOp<void> AndroidLocalUserLoginHandler::ReLogin(
    SharedPtr<PlayFab::Entity> entity,
    RunContext rc
) noexcept
{
    SharedPtr<AndroidLocalUser> localUser{ m_localUser.lock() };
    if (!localUser)
    {
        TRACE_INFORMATION("Unable to login because LocalUser was destroyed");
        return E_FAIL;
    }

    bool isThreadAttached = false;
    JNIEnv* jniEnv = nullptr;
    JavaVM* jvm = localUser->m_platformComponents->GetJavaVm();

    jint jniResult = jvm->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_6);

    if (jniResult == JNI_EDETACHED)
    {
        TRACE_ERROR("Unable to get because thread is detached");
        jint attachThreadResult = jvm->AttachCurrentThread(&jniEnv, nullptr);

        if (attachThreadResult != JNI_OK)
        {
            TRACE_ERROR("Unable to get JNIEnv, jniResult=%u", jniResult);
            return E_FAIL;
        }

        isThreadAttached = true;
    }
    else if (jniResult != JNI_OK || jniEnv == nullptr)
    {
        TRACE_ERROR("Unable to JNIEnv, jniResult=%u", jniResult);
        return E_FAIL;
    }

    jmethodID getTokenMethodId = jniEnv->GetStaticMethodID(
        localUser->m_platformComponents->GetLocalUserClass(),
        "GetServerAuthTokenAsync",
        "(Lcom/google/android/gms/games/GamesSignInClient;Ljava/lang/String;J)V"
    );

    if (getTokenMethodId == NULL)
    {
        TRACE_ERROR("Unable to get GetServerAuthTokenAsync methodId");
        return E_FAIL;
    }

    UniquePtr<GetServerAuthTokenContext> context{ MakeUnique<GetServerAuthTokenContext>(localUser) };

    jstring oauthToken = jniEnv->NewStringUTF(localUser->m_OAuth2WebClientId.data());
    jniEnv->CallStaticVoidMethod(localUser->m_platformComponents->GetLocalUserClass(), getTokenMethodId, localUser->m_signinClient, oauthToken, reinterpret_cast<jlong>(context.get()));

    if (isThreadAttached)
    {
        jvm->DetachCurrentThread();
    }

    auto op = AsyncOp<String>{ context->asyncContext }.Then([entity, rc](Result<String> result) mutable -> AsyncOp<ServiceResponse>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        TRACE_VERBOSE("Got token %s, making PlayFab Login call", result.Payload().data());
        Authentication::LoginWithGooglePlayGamesServicesRequest request;
        request.SetServerAuthCode(result.ExtractPayload());

        JsonValue requestBody = request.ToJson();
        RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

        return entity->ServiceConfig()->HttpClient()->MakePostRequest(
            "/Client/LoginWithGooglePlayGamesServices",
            UnorderedMap<String, String>{},
            std::move(requestBody),
            CacheId::AuthenticationLoginWithGooglePlayGamesServices,
            rc.Derive()
        );

    }).Then([ entity ](Result<ServiceResponse> result)->Result<void>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode == 200)
        {
            Authentication::EntityTokenResponse entityToken;
            RETURN_IF_FAILED(JsonUtils::ObjectGetMember(serviceResponse.Data, "EntityToken", entityToken));
            RETURN_IF_FAILED(entity->OnEntityTokenRefreshed(entityToken));
            return S_OK;
        }
        else
        {
            return Result<void>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });

    context.release(); // reclaimed in JNI callback
    return op;
}

}

extern "C"
{

JNIEXPORT void JNICALL Java_com_microsoft_playfab_core_LocalUser_OnGetServerAuthTokenSuccess(
    JNIEnv* env,
    jclass /* class */,
    jlong context,
    jstring token)
{
    assert(context);

    const char* nativeToken = env->GetStringUTFChars(token, nullptr);   
    TRACE_VERBOSE("Got server auth token: %s", nativeToken);

    // Context will be cleaned up when this callback returns
    PlayFab::UniquePtr<PlayFab::GetServerAuthTokenContext> getServerTokenContext{ reinterpret_cast<PlayFab::GetServerAuthTokenContext*>(context) };
    getServerTokenContext->asyncContext->Complete(PlayFab::Result<PlayFab::String>(nativeToken));

    env->ReleaseStringUTFChars(token, nativeToken);
}

JNIEXPORT void JNICALL Java_com_microsoft_playfab_core_LocalUser_OnGetServerAuthTokenFailed(
    JNIEnv* env,
    jclass/* class */,
    jlong context,
    jstring errorMessage)
{
    assert(context);

    const char* nativeErrorMessage = env->GetStringUTFChars(errorMessage, nullptr);
    TRACE_VERBOSE("Failed to get server auth token, error=%s", nativeErrorMessage);

    // Context will be cleaned up when this callback returns
    PlayFab::UniquePtr<PlayFab::GetServerAuthTokenContext> getServerTokenContext{ reinterpret_cast<PlayFab::GetServerAuthTokenContext*>(context) };
    getServerTokenContext->asyncContext->Complete(PlayFab::Result<PlayFab::String>(E_FAIL,nativeErrorMessage));

    env->ReleaseStringUTFChars(errorMessage, nativeErrorMessage);
}

}
