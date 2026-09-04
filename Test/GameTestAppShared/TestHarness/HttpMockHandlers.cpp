#include "pch.h"
#include "HttpMockHandlers.h"
#include "DeviceGameSaveState.h"
#include "CommandHandlerShared.h"
#include "HttpMock.h"
#include <memory>
#include "CommandRegistry.h"

using CommandHandlerShared::CreateBaseResult;
using CommandHandlerShared::MarkSuccess;
using CommandHandlerShared::MarkFailure;

CommandResultPayload HandleConfigureHttpMock(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            try
            {
                // Extract parameters - handle both string and numeric types
                std::string method = "POST";
                if (parameters.contains("method"))
                {
                    method = parameters["method"].is_string() ? parameters["method"].get<std::string>() : "POST";
                }
                
                std::string url = "";
                if (parameters.contains("url"))
                {
                    url = parameters["url"].is_string() ? parameters["url"].get<std::string>() : "";
                }
                
                uint32_t statusCode = 503;
                if (parameters.contains("statusCode"))
                {
                    if (parameters["statusCode"].is_number())
                    {
                        statusCode = parameters["statusCode"].get<uint32_t>();
                    }
                    else if (parameters["statusCode"].is_string())
                    {
                        statusCode = static_cast<uint32_t>(std::stoul(parameters["statusCode"].get<std::string>()));
                    }
                }

                if (url.empty())
                {
                    return E_INVALIDARG;
                }

                // Extract optional response body
                std::string responseBody = "";
                if (parameters.contains("responseBody"))
                {
                    responseBody = parameters["responseBody"].is_string() ? parameters["responseBody"].get<std::string>() : "";
                }

                // Optional narrowing: libHttpClient matches mocks by URL PREFIX only and ignores
                // the HTTP method (DoesMockCallMatch in lhc_mock.cpp), so a mock aimed at one
                // request on a host inevitably captures every other request to that host too.
                // When "urlContains" is set, only requests whose URL contains that substring get
                // the configured failure; everything else the mock swallows is answered with
                // "otherStatusCode" (default 200) and an empty body, so unrelated traffic to the
                // same host behaves as if it had succeeded.
                std::string urlContains = "";
                if (parameters.contains("urlContains") && parameters["urlContains"].is_string())
                {
                    urlContains = parameters["urlContains"].get<std::string>();
                }

                uint32_t otherStatusCode = 200;
                if (parameters.contains("otherStatusCode") && parameters["otherStatusCode"].is_number())
                {
                    otherStatusCode = parameters["otherStatusCode"].get<uint32_t>();
                }

                // Create and configure the HTTP mock
                auto mock = std::make_shared<HttpMock>(method.c_str(), url.c_str());
                mock->SetResponseHttpStatus(statusCode);
                
                // Set response body if provided
                if (!responseBody.empty())
                {
                    mock->SetResponseBody(responseBody.c_str());
                }

                if (!urlContains.empty())
                {
                    // The matched callback runs before libHttpClient reads the mock's status and
                    // body (lhc_mock.cpp:108-135), so deciding here is what the caller observes.
                    mock->SetCallback(
                        [urlContains, statusCode, otherStatusCode, responseBody, deviceId]
                        (HttpMock const& matched, std::string matchedUrl, std::string, uint32_t)
                        {
                            if (matchedUrl.find(urlContains) != std::string::npos)
                            {
                                matched.SetResponseHttpStatus(statusCode);
                                if (!responseBody.empty())
                                {
                                    matched.SetResponseBody(responseBody.c_str());
                                }
                                LogToWindow("HttpMock: [" + deviceId + "] FAIL " + std::to_string(statusCode) + " " + matchedUrl);
                            }
                            else
                            {
                                matched.SetResponseHttpStatus(otherStatusCode);
                                matched.ClearReponseBody();
                                LogToWindow("HttpMock: [" + deviceId + "] pass " + std::to_string(otherStatusCode) + " " + matchedUrl);
                            }
                        });
                }

                // Store the mock in the state to keep it alive
                state->httpMocks.push_back(mock);

                std::string detail = method + " " + url + " -> " + std::to_string(statusCode);
                if (!urlContains.empty())
                {
                    detail += " (only when url contains '" + urlContains + "', otherwise " + std::to_string(otherStatusCode) + ")";
                }
                LogToWindow("ConfigureHttpMock: [" + deviceId + "] " + detail);
                return S_OK;
            }
            catch (const std::exception&)
            {
                return E_FAIL;
            }
        });
}

CommandResultPayload HandleClearHttpMocks(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            try
            {
                // Clear all HTTP mocks (they will be destroyed and unregistered)
                size_t count = state->httpMocks.size();
                state->httpMocks.clear();

                LogToWindow("ClearHttpMocks: [" + deviceId + "] Cleared " + std::to_string(count) + " mock(s)");
                return S_OK;
            }
            catch (const std::exception&)
            {
                return E_FAIL;
            }
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "ClearHttpMocks", HandleClearHttpMocks },
    { "ConfigureHttpMock", HandleConfigureHttpMock }
});
