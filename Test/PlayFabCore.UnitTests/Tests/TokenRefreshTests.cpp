#include "stdafx.h"
#include "TestIncludes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PlayFab
{
namespace UnitTests
{

TEST_CLASS(TokenRefreshTests)
{
public:
    TEST_METHOD(TokenRefreshSimple)
    {
        Wrappers::TestSession session;
        Wrappers::ServiceConfig<> serviceConfig{ "mockAPIEndpoint", "mockTitleId" };

        // Mock LoginWithCustomID
        PlayFabServiceMock loginMock{ serviceConfig, "LoginWithCustomID" };

        // Token to assign after refresh
        const char refreshedToken[] = "RefreshedToken";

        loginMock.SetCallback([&](PlayFabServiceMock& mock, std::string, std::string, uint32_t hitCount)
        {
            if (hitCount == 1)
            {
                // On the initial Login, set token to expire in 1 minute to trigger a refresh
                time_t tokenExpiration = time(nullptr) + 60;
                auto tokenExpirationString = TimeTToIso8601String(tokenExpiration);
                rapidjson::Value tokenExpirationJson{ tokenExpirationString.data(), PlayFabServiceMock::JsonAllocator() };
                loginMock.ResponseBodyPayload()["EntityToken"].AddMember("TokenExpiration", tokenExpirationJson.Move(), PlayFabServiceMock::JsonAllocator());
            }
            else if (hitCount == 2)
            {
                // On subsequent Login, supply a new, non-expiring token
                auto& entityTokenJson = mock.ResponseBodyPayload()["EntityToken"];
                entityTokenJson.RemoveMember("TokenExpiration");
                entityTokenJson["EntityToken"] = rapidjson::Value{ refreshedToken };
            }
            else
            {
                // Login should only be called twice
                assert(false);
            }
        });

        struct TokenRefreshedHandlerContext
        {
            Event event;
            std::string entityId;
            std::string newToken;
        } tokenRefreshContext;

        PFRegistrationToken registrationToken{};
        VERIFY_SUCCEEDED(PFEntityRegisterTokenRefreshedEventHandler(nullptr, &tokenRefreshContext, [](void* ctx, const PFEntityKey* entityKey, const PFEntityToken* newToken)
        {
            auto handlerContext = static_cast<TokenRefreshedHandlerContext*>(ctx);
            handlerContext->entityId = entityKey->id;
            handlerContext->newToken = newToken->token;
            handlerContext->event.Set();
        }, &registrationToken));

        // Synchronously authenticate
        PFEntityHandle entityHandle{ nullptr };
        {
            XAsyncBlock async{};
            PFAuthenticationLoginWithCustomIDRequest request{};
            VERIFY_SUCCEEDED(PFAuthenticationLoginWithCustomIDAsync(serviceConfig.Handle(), &request, &async));
            VERIFY_SUCCEEDED(XAsyncGetStatus(&async, true));
            VERIFY_SUCCEEDED(PFAuthenticationLoginWithCustomIDGetResult(&async, &entityHandle, 0, nullptr, nullptr, nullptr));
        }
        auto entity = Wrappers::Entity<>::Wrap(entityHandle);


        // Wait for token refreshed event
        tokenRefreshContext.event.Wait();
        Assert::AreEqual(tokenRefreshContext.entityId.data(), entity.EntityKey().Model().id);
        Assert::AreEqual(tokenRefreshContext.newToken.data(), refreshedToken);

        // Check the new EntityToken cached in the Entity object
        {
            XAsyncBlock async{};
            VERIFY_SUCCEEDED(PFEntityGetEntityTokenAsync(entity.Handle(), &async));

            size_t tokenSize;
            VERIFY_SUCCEEDED(PFEntityGetEntityTokenResultSize(&async, &tokenSize));

            std::vector<char> tokenBuffer(tokenSize);
            PFEntityToken const* token{};
            VERIFY_SUCCEEDED(PFEntityGetEntityTokenResult(&async, tokenBuffer.size(), tokenBuffer.data(), &token, nullptr));

            Assert::AreEqual(refreshedToken, token->token);
        }
    }

    TEST_METHOD(TestTokenExpiredCallback)
    {
        Wrappers::TestSession session;
        Wrappers::ServiceConfig<> serviceConfig{ "mockAPIEndpoint", "mockTitleId" };

        // Mock LoginWithCustomID
        PlayFabServiceMock loginMock{ serviceConfig, "LoginWithCustomID" };

        // Token to assign after ReLogin
        const char newToken[] = "NewEntityToken";

        loginMock.SetCallback([&](PlayFabServiceMock& mock, std::string, std::string, uint32_t hitCount)
        {
            if (hitCount == 1)
            {
                // On the initial Login, set token to expire in 1 minute to trigger a refresh
                time_t tokenExpiration = time(nullptr) + 60;
                auto tokenExpirationString = TimeTToIso8601String(tokenExpiration);
                rapidjson::Value tokenExpirationJson{ tokenExpirationString.data(), PlayFabServiceMock::JsonAllocator() };
                loginMock.ResponseBodyPayload()["EntityToken"].AddMember("TokenExpiration", tokenExpirationJson.Move(), PlayFabServiceMock::JsonAllocator());
            }
            else if (hitCount == 2)
            {
                // On second Login attempt, respond with error
                mock.ResponseBody().CopyFrom(PlayFabServiceMock::ServiceErrors()["ExpiredAuthToken"], PlayFabServiceMock::JsonAllocator());
            }
            else if (hitCount == 3)
            {
                // Reset to default response with the new token
                mock.ResponseBody().CopyFrom(mock.ServiceResponses()["default"], mock.JsonAllocator());
                mock.ResponseBodyPayload()["EntityToken"]["EntityToken"] = rapidjson::Value{ newToken };
            }
            else
            {
                // Login should only be called 3 times
                assert(false);
            }
        });

        struct TokenExpiredHandlerContext
        {
            Event event;
            std::string entityId;
        } tokenExpiredContext;

        PFRegistrationToken registrationToken{};
        VERIFY_SUCCEEDED(PFEntityRegisterTokenExpiredEventHandler(nullptr, &tokenExpiredContext, [](void* ctx, PFEntityKey const* entityKey)
        {
            auto handlerContext = static_cast<TokenExpiredHandlerContext*>(ctx);
            handlerContext->entityId = entityKey->id;
            handlerContext->event.Set();
        }, &registrationToken));

        // Synchronously authenticate
        PFEntityHandle entityHandle{ nullptr };
        {
            XAsyncBlock async{};
            PFAuthenticationLoginWithCustomIDRequest request{};
            VERIFY_SUCCEEDED(PFAuthenticationLoginWithCustomIDAsync(serviceConfig.Handle(), &request, &async));
            VERIFY_SUCCEEDED(XAsyncGetStatus(&async, true));
            VERIFY_SUCCEEDED(PFAuthenticationLoginWithCustomIDGetResult(&async, &entityHandle, 0, nullptr, nullptr, nullptr));
        }
        auto entity = Wrappers::Entity<>::Wrap(entityHandle);

        // Wait for token expired event
        tokenExpiredContext.event.Wait();
        Assert::AreEqual(tokenExpiredContext.entityId.data(), entity.EntityKey().Model().id);

        // Synchronously reauthenticate
        {
            XAsyncBlock async{};
            PFAuthenticationLoginWithCustomIDRequest request{};
            VERIFY_SUCCEEDED(PFAuthenticationReLoginWithCustomIDAsync(entity.Handle(), &request, &async));
            VERIFY_SUCCEEDED(XAsyncGetStatus(&async, true));
        }

        // Check the new EntityToken
        {
            XAsyncBlock async{};
            VERIFY_SUCCEEDED(PFEntityGetEntityTokenAsync(entity.Handle(), &async));

            size_t tokenSize;
            VERIFY_SUCCEEDED(PFEntityGetEntityTokenResultSize(&async, &tokenSize));

            std::vector<char> tokenBuffer(tokenSize);
            PFEntityToken const* token{};
            VERIFY_SUCCEEDED(PFEntityGetEntityTokenResult(&async, tokenBuffer.size(), tokenBuffer.data(), &token, nullptr));

            Assert::AreEqual(newToken, token->token);
        }
    }

};

}
}
