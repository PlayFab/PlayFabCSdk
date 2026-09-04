#include "pch.h"

#include "XblStringHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

static std::vector<uint8_t> s_stringVerifyBuffer;

CommandResultPayload HandleXblStringVerifyStringAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string stringToVerify, error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "stringToVerify", stringToVerify, error))
            {
                stringToVerify = "TestString";
            }

            HRESULT hr = XblStringVerifyStringAsync(state->xblContext, stringToVerify.c_str(), &async);
            LogToWindowFormat("XblStringVerifyStringAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultSize{};
            HRESULT hr = XblStringVerifyStringResultSize(&async, &resultSize);
            if (SUCCEEDED(hr))
            {
                s_stringVerifyBuffer.resize(resultSize);
                XblVerifyStringResult* result{};
                hr = XblStringVerifyStringResult(&async, resultSize, s_stringVerifyBuffer.data(), &result, nullptr);
                if (SUCCEEDED(hr) && result)
                {
                    payload.result["resultCode"] = static_cast<int>(result->resultCode);
                    if (result->firstOffendingSubstring)
                    {
                        payload.result["firstOffendingSubstring"] = result->firstOffendingSubstring;
                    }
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblStringVerifyStringsAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::vector<std::string> strings;
            std::vector<const char*> stringPtrs;
            if (parameters.contains("stringsToVerify") && parameters["stringsToVerify"].is_array())
            {
                for (const auto& s : parameters["stringsToVerify"])
                {
                    if (s.is_string()) strings.push_back(s.get<std::string>());
                }
                for (const auto& s : strings) stringPtrs.push_back(s.c_str());
            }

            if (stringPtrs.empty())
            {
                strings = { "TestString1", "GoodString", "TestString2" };
                stringPtrs.clear();
                for (const auto& s : strings) stringPtrs.push_back(s.c_str());
            }

            HRESULT hr = XblStringVerifyStringsAsync(state->xblContext, stringPtrs.data(), stringPtrs.size(), &async);
            LogToWindowFormat("XblStringVerifyStringsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultSize{};
            HRESULT hr = XblStringVerifyStringsResultSize(&async, &resultSize);
            if (SUCCEEDED(hr))
            {
                s_stringVerifyBuffer.resize(resultSize);
                XblVerifyStringResult* results{};
                size_t resultCount{};
                hr = XblStringVerifyStringsResult(&async, resultSize, s_stringVerifyBuffer.data(), &results, &resultCount, nullptr);
                if (SUCCEEDED(hr))
                {
                    payload.result["resultCount"] = resultCount;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblStringVerifyStringResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblStringVerifyStringResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblStringVerifyStringResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblStringVerifyStringResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblStringVerifyStringsResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblStringVerifyStringsResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblStringVerifyStringsResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblStringVerifyStringsResult: called inline by Async handler");
            return S_OK;
        });
}


// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblStringVerifyStringAsync", HandleXblStringVerifyStringAsync },
    { "XblStringVerifyStringResult", HandleXblStringVerifyStringResult },
    { "XblStringVerifyStringResultSize", HandleXblStringVerifyStringResultSize },
    { "XblStringVerifyStringsAsync", HandleXblStringVerifyStringsAsync },
    { "XblStringVerifyStringsResult", HandleXblStringVerifyStringsResult },
    { "XblStringVerifyStringsResultSize", HandleXblStringVerifyStringsResultSize }
});
