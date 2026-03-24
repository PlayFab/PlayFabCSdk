#include "pch.h"
#include "HttpMockHandlers.h"
#include "DeviceGameSaveState.h"
#include "CommandHandlerShared.h"
#include "HttpMock.h"
#include <memory>

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
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

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
            MarkFailure(payload.result, E_INVALIDARG, "URL parameter is required");
            return payload;
        }

        // Extract optional response body
        std::string responseBody = "";
        if (parameters.contains("responseBody"))
        {
            responseBody = parameters["responseBody"].is_string() ? parameters["responseBody"].get<std::string>() : "";
        }

        // Create and configure the HTTP mock
        auto mock = std::make_shared<HttpMock>(method.c_str(), url.c_str());
        mock->SetResponseHttpStatus(statusCode);
        
        // Set response body if provided
        if (!responseBody.empty())
        {
            mock->SetResponseBody(responseBody.c_str());
        }

        // Store the mock in the state to keep it alive
        state->httpMocks.push_back(mock);

        LogToWindow("ConfigureHttpMock: [" + deviceId + "] " + method + " " + url + " -> " + std::to_string(statusCode));
        MarkSuccess(payload.result);
    }
    catch (const std::exception& ex)
    {
        MarkFailure(payload.result, E_FAIL, std::string("Exception in ConfigureHttpMock: ") + ex.what());
    }

    payload.elapsedMs = 0;
    return payload;
}

CommandResultPayload HandleClearHttpMocks(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    try
    {
        // Clear all HTTP mocks (they will be destroyed and unregistered)
        size_t count = state->httpMocks.size();
        state->httpMocks.clear();

        LogToWindow("ClearHttpMocks: [" + deviceId + "] Cleared " + std::to_string(count) + " mock(s)");
        MarkSuccess(payload.result);
    }
    catch (const std::exception& ex)
    {
        MarkFailure(payload.result, E_FAIL, std::string("Exception in ClearHttpMocks: ") + ex.what());
    }

    payload.elapsedMs = 0;
    return payload;
}
