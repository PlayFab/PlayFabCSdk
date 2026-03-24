#include "pch.h"

#include "ChaosModeHandler.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "WriteGameSaveData.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>

using CommandHandlerShared::ComputeElapsedMs;
using CommandHandlerShared::CreateBaseResult;
using CommandHandlerShared::MarkFailure;
using CommandHandlerShared::MarkSuccess;
using CommandHandlerShared::SetHResult;
using CommandHandlerShared::ToLowerCopy;

namespace
{
    constexpr uint32_t kSmallMinBytes = 1024;
    constexpr uint32_t kSmallMaxBytes = 32768;
    constexpr uint32_t kLargeMinBytes = 524288;
    constexpr uint32_t kLargeMaxBytes = 2097152;
    constexpr uint32_t kMaxUploads = 1000;
    constexpr uint32_t kMaxOperations = 1000;

    nlohmann::json SerializeConfiguration(const ChaosModeConfiguration& config)
    {
        nlohmann::json json = nlohmann::json::object();
        json["fileCreate"] = config.fileCreate;
        json["fileModify"] = config.fileModify;
        json["fileDelete"] = config.fileDelete;
        json["folderCreate"] = config.folderCreate;
        json["folderDelete"] = config.folderDelete;
        json["binaryFiles"] = config.binaryFiles;
        json["textFiles"] = config.textFiles;
        json["unicodeFiles"] = config.unicodeFiles;
        json["unicodeFolders"] = config.unicodeFolders;
        json["largeFiles"] = config.largeFiles;
        json["operationsPerUpload"] = config.operationsPerUpload;
        json["numUploads"] = config.numUploads;
        return json;
    }

    std::vector<std::string> BuildChaosVerbs(const ChaosModeConfiguration& config)
    {
        std::vector<std::string> verbs;
        if (config.fileCreate)
        {
            if (config.binaryFiles)
            {
                verbs.emplace_back("CreateRandomBinaryFile");
                verbs.emplace_back("CreateBinaryFile");
            }
            if (config.textFiles)
            {
                verbs.emplace_back("CreateRandomText");
            }
        }
        if (config.fileModify)
        {
            verbs.emplace_back("CopyFile");
            verbs.emplace_back("RenameFile");
        }
        if (config.fileDelete)
        {
            verbs.emplace_back("DeleteFile");
        }
        if (config.folderCreate)
        {
            verbs.emplace_back("CreateFolder");
        }
        if (config.folderDelete)
        {
            verbs.emplace_back("DeleteFolder");
            verbs.emplace_back("RenameFolder");
        }

        std::sort(verbs.begin(), verbs.end());
        verbs.erase(std::unique(verbs.begin(), verbs.end()), verbs.end());
        return verbs;
    }

    uint32_t ClampToUInt32(int64_t value, uint32_t fallback)
    {
        if (value < 0)
        {
            return fallback;
        }

        if (value > static_cast<int64_t>(std::numeric_limits<uint32_t>::max()))
        {
            return std::numeric_limits<uint32_t>::max();
        }

        return static_cast<uint32_t>(value);
    }

    bool TryParseUploadMode(const nlohmann::json& parameters, PFGameSaveFilesUploadOption& option, std::string& error)
    {
        auto it = parameters.find("uploadMode");
        if (it == parameters.end())
        {
            return true;
        }

        if (!it->is_string())
        {
            error = "uploadMode must be a string";
            return false;
        }

        const std::string lower = ToLowerCopy(it->get<std::string>());
        if (lower == "keepdeviceactive" || lower == "keepactive" || lower == "default" || lower == "offlinedeferred")
        {
            option = PFGameSaveFilesUploadOption::KeepDeviceActive;
            return true;
        }

        if (lower == "releasedeviceasactive" || lower == "release")
        {
            option = PFGameSaveFilesUploadOption::ReleaseDeviceAsActive;
            return true;
        }

        error = "Unsupported uploadMode value";
        return false;
    }

    std::string UnicodeFolderStem(uint32_t runIndex)
    {
        std::ostringstream oss;
        oss << "unicode-\xE3\x83\xA6\xE3\x83\x8B-" << runIndex;
        return oss.str();
    }

    std::string UnicodeFileStem(uint32_t runIndex)
    {
        std::ostringstream oss;
        oss << "file-\xCF\x80-" << runIndex;
        return oss.str();
    }

    std::vector<nlohmann::json> BuildUnicodeOperations(const ChaosModeConfiguration& config, uint32_t runIndex, uint32_t fileBytes)
    {
        std::vector<nlohmann::json> operations;
        if (!config.unicodeFiles && !config.unicodeFolders)
        {
            return operations;
        }

        const std::string folderStem = UnicodeFolderStem(runIndex);
        std::string basePath;
        if (config.unicodeFolders)
        {
            nlohmann::json folderOp = nlohmann::json::object();
            folderOp["verb"] = "CreateFolder";
            folderOp["relativePath"] = folderStem;
            operations.emplace_back(std::move(folderOp));
            basePath = folderStem;
        }
        else
        {
            basePath = std::string("unicode-") + std::to_string(runIndex);
        }

        if (config.unicodeFiles && config.textFiles)
        {
            nlohmann::json textOp = nlohmann::json::object();
            textOp["verb"] = "CreateRandomText";
            textOp["relativePath"] = basePath + "/" + UnicodeFileStem(runIndex) + ".txt";
            textOp["bytes"] = static_cast<int64_t>(fileBytes);
            operations.emplace_back(std::move(textOp));
        }

        if (config.unicodeFiles && config.binaryFiles)
        {
            nlohmann::json binOp = nlohmann::json::object();
            binOp["verb"] = "CreateRandomBinaryFile";
            binOp["relativePath"] = basePath + "/" + UnicodeFileStem(runIndex) + ".bin";
            binOp["bytes"] = static_cast<int64_t>(fileBytes);
            operations.emplace_back(std::move(binOp));
        }

        return operations;
    }

    uint32_t GenerateChaosSeed(uint32_t runIndex, uint32_t operationsPerUpload)
    {
        (void)runIndex;
        (void)operationsPerUpload;
        const auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
        const uint64_t ticks = static_cast<uint64_t>(now.count());
        uint32_t base = static_cast<uint32_t>(ticks) ^ static_cast<uint32_t>(ticks >> 32);
        uint32_t counter = static_cast<uint32_t>(ticks);

        uint32_t mix = base ^ (counter * 0x9E3779B9u);
        return mix;
    }

    nlohmann::json BuildChaosObject(
        uint32_t runIndex,
        uint32_t operationsPerUpload,
        const std::vector<std::string>& verbs,
        uint32_t minFileBytes,
        uint32_t maxFileBytes,
        const nlohmann::json& overrides)
    {
        nlohmann::json chaos = nlohmann::json::object();
        chaos["operationCount"] = static_cast<int64_t>(operationsPerUpload);
        chaos["minFileBytes"] = static_cast<int64_t>(minFileBytes);
        chaos["maxFileBytes"] = static_cast<int64_t>(maxFileBytes);

        nlohmann::json verbsJson = nlohmann::json::array();
        for (const auto& verb : verbs)
        {
            verbsJson.push_back(verb);
        }
        chaos["verbs"] = std::move(verbsJson);

        uint32_t seed = GenerateChaosSeed(runIndex, operationsPerUpload);
        std::srand(static_cast<unsigned int>(seed));
        chaos["seed"] = seed;

        for (auto it = overrides.begin(); it != overrides.end(); ++it)
        {
            chaos[it.key()] = it.value();
        }

        return chaos;
    }

    HRESULT PerformUpload(
        DeviceGameSaveState* state,
        PFGameSaveFilesUploadOption option,
        int& elapsedMs,
        std::string& error)
    {
        if (!state->localUserHandle)
        {
            error = "Local user handle not created";
            elapsedMs = 0;
            return E_POINTER;
        }

        XAsyncBlock async{};
        async.queue = state->taskQueueOwnedByCommand ? state->taskQueue : nullptr;

        auto start = std::chrono::steady_clock::now();
        HRESULT hr = PFGameSaveFilesUploadWithUiAsync(state->localUserHandle, option, &async);
        HRESULT waitHr = S_OK;
        HRESULT resultHr = S_OK;
        if (SUCCEEDED(hr))
        {
            waitHr = XAsyncGetStatus(&async, true);
            if (SUCCEEDED(waitHr))
            {
                resultHr = PFGameSaveFilesUploadWithUiResult(&async);
            }
        }

        elapsedMs = ComputeElapsedMs(start);
        if (FAILED(hr))
        {
            error = "PFGameSaveFilesUploadWithUiAsync failed";
            return hr;
        }

        if (FAILED(waitHr))
        {
            error = "PFGameSaveFilesUploadWithUiAsync wait failed";
            return waitHr;
        }

        if (FAILED(resultHr))
        {
            error = "PFGameSaveFilesUploadWithUiResult failed";
            return resultHr;
        }

        return S_OK;
    }

}

CommandResultPayload HandleDoChaosMode(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!state)
    {
        MarkFailure(payload.result, E_POINTER, "Device state not available");
        return payload;
    }

    if (!parameters.is_object())
    {
        MarkFailure(payload.result, E_INVALIDARG, "Parameters payload is not an object");
        return payload;
    }

    ChaosModeConfiguration config = state->chaosConfig;
    std::string error;

    auto requireBool = [&](const char* key, bool& target) -> bool
    {
        auto it = parameters.find(key);
        if (it == parameters.end())
        {
            error = std::string("Missing parameter '") + key + "'";
            return false;
        }

        bool value = target;
        if (!CommandHandlerShared::TryParseBoolParameter(parameters, key, value, error))
        {
            return false;
        }

        target = value;
        return true;
    };

    if (!requireBool("fileCreate", config.fileCreate) ||
        !requireBool("fileModify", config.fileModify) ||
        !requireBool("fileDelete", config.fileDelete) ||
        !requireBool("folderCreate", config.folderCreate) ||
        !requireBool("folderDelete", config.folderDelete) ||
        !requireBool("binaryFiles", config.binaryFiles) ||
        !requireBool("textFiles", config.textFiles) ||
        !requireBool("largeFiles", config.largeFiles) ||
        !requireBool("unicodeFiles", config.unicodeFiles) ||
        !requireBool("unicodeFolders", config.unicodeFolders))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    int64_t operationsPerUploadValue = config.operationsPerUpload;
    if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "operationsPerUpload", operationsPerUploadValue, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    if (operationsPerUploadValue < 1 || operationsPerUploadValue > kMaxOperations)
    {
        MarkFailure(payload.result, E_INVALIDARG, "Parameter 'operationsPerUpload' must be between 1 and 1000");
        return payload;
    }

    int64_t numUploadsValue = config.numUploads;
    if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "numUploads", numUploadsValue, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    if (numUploadsValue < 1 || numUploadsValue > kMaxUploads)
    {
        MarkFailure(payload.result, E_INVALIDARG, "Parameter 'numUploads' must be between 1 and 1000");
        return payload;
    }

    config.operationsPerUpload = static_cast<uint32_t>(operationsPerUploadValue);
    config.numUploads = static_cast<uint32_t>(numUploadsValue);

    state->chaosConfig = config;

    bool configureOnly = false;
    if (!CommandHandlerShared::TryParseBoolParameter(parameters, "configureOnly", configureOnly, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    LogToWindowFormat("DoChaosMode configuration: fileCreate=%d fileModify=%d fileDelete=%d folderCreate=%d folderDelete=%d binary=%d text=%d large=%d unicodeFiles=%d unicodeFolders=%d opsPerUpload=%u numUploads=%u",
        config.fileCreate ? 1 : 0,
        config.fileModify ? 1 : 0,
        config.fileDelete ? 1 : 0,
        config.folderCreate ? 1 : 0,
        config.folderDelete ? 1 : 0,
        config.binaryFiles ? 1 : 0,
        config.textFiles ? 1 : 0,
        config.largeFiles ? 1 : 0,
        config.unicodeFiles ? 1 : 0,
        config.unicodeFolders ? 1 : 0,
        config.operationsPerUpload,
    config.numUploads);

    if (configureOnly)
    {
        payload.result["configuration"] = SerializeConfiguration(config);
        payload.elapsedMs = 0;
        MarkSuccess(payload.result);
        SetHResult(payload.result, S_OK);
        return payload;
    }

    auto verbs = BuildChaosVerbs(config);
    if (verbs.empty())
    {
        MarkFailure(payload.result, E_INVALIDARG, "No chaos verbs enabled. Enable at least one file or folder option.");
        return payload;
    }

    bool skipUpload = false;
    if (!CommandHandlerShared::TryParseBoolParameter(parameters, "skipUpload", skipUpload, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    int64_t minBytesOverride = -1;
    if (parameters.contains("minFileBytes"))
    {
        if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "minFileBytes", minBytesOverride, error))
        {
            MarkFailure(payload.result, E_INVALIDARG, error);
            return payload;
        }
    }

    int64_t maxBytesOverride = -1;
    if (parameters.contains("maxFileBytes"))
    {
        if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "maxFileBytes", maxBytesOverride, error))
        {
            MarkFailure(payload.result, E_INVALIDARG, error);
            return payload;
        }
    }

    if ((minBytesOverride >= 0 && maxBytesOverride >= 0) && minBytesOverride > maxBytesOverride)
    {
        std::swap(minBytesOverride, maxBytesOverride);
    }

    uint32_t minBytes = config.largeFiles ? kLargeMinBytes : kSmallMinBytes;
    uint32_t maxBytes = config.largeFiles ? kLargeMaxBytes : kSmallMaxBytes;

    if (minBytesOverride >= 0)
    {
        minBytes = ClampToUInt32(minBytesOverride, minBytes);
    }

    if (maxBytesOverride >= 0)
    {
        maxBytes = std::max(minBytes, ClampToUInt32(maxBytesOverride, maxBytes));
    }

    nlohmann::json chaosOverrides = nlohmann::json::object();
    if (auto chaosOverridesIt = parameters.find("chaosOverrides"); chaosOverridesIt != parameters.end())
    {
        if (!chaosOverridesIt->is_object())
        {
            MarkFailure(payload.result, E_INVALIDARG, "chaosOverrides must be an object");
            return payload;
        }
        chaosOverrides = *chaosOverridesIt;
    }

    nlohmann::json userOperations = nlohmann::json::array();
    if (auto operationsIt = parameters.find("operations"); operationsIt != parameters.end())
    {
        if (!operationsIt->is_array())
        {
            MarkFailure(payload.result, E_INVALIDARG, "operations must be an array");
            return payload;
        }
        userOperations = *operationsIt;
    }

    PFGameSaveFilesUploadOption uploadOption = PFGameSaveFilesUploadOption::KeepDeviceActive;
    if (!TryParseUploadMode(parameters, uploadOption, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    std::vector<nlohmann::json> runSummaries;
    const uint32_t numUploads = config.numUploads;
    runSummaries.reserve(numUploads);
    uint32_t uploadsCompleted = 0;
    auto overallStart = std::chrono::steady_clock::now();

    for (uint32_t run = 0; run < numUploads; ++run)
    {
        const uint32_t runIndex = run + 1;
        LogToWindowFormat("Chaos run %u/%u starting", runIndex, numUploads);

        nlohmann::json runResult = nlohmann::json::object();
        runResult["run"] = static_cast<int64_t>(runIndex);

        nlohmann::json operations = nlohmann::json::array();
        for (const auto& entry : userOperations)
        {
            operations.push_back(entry);
        }

        const auto unicodeOps = BuildUnicodeOperations(config, runIndex, std::min<uint32_t>(maxBytes, 8192));
        for (const auto& op : unicodeOps)
        {
            operations.push_back(op);
        }

        nlohmann::json writeParameters = nlohmann::json::object();
        writeParameters["chaos"] = BuildChaosObject(
            runIndex,
            config.operationsPerUpload,
            verbs,
            minBytes,
            maxBytes,
            chaosOverrides);

        if (!operations.empty())
        {
            writeParameters["operations"] = std::move(operations);
        }

        const auto runStart = std::chrono::steady_clock::now();
        WriteGameSaveDataResult writeResult = ExecuteWriteGameSaveData(state, commandId + "-run" + std::to_string(runIndex), writeParameters);
        runResult["writeElapsedMs"] = ComputeElapsedMs(runStart);

        if (FAILED(writeResult.hr))
        {
            LogToWindowFormat("Chaos run %u failed: %s", runIndex, writeResult.errorMessage.c_str());
            MarkFailure(payload.result, writeResult.hr, writeResult.errorMessage);
            payload.result["runs"] = runSummaries;
            payload.result["failedRun"] = static_cast<int64_t>(runIndex);
            payload.elapsedMs = ComputeElapsedMs(overallStart);
            return payload;
        }

        runResult["mutationsApplied"] = static_cast<int64_t>(writeResult.mutations.size());
        runResult["chaosMutations"] = writeResult.chaosMutationsApplied;
        runResult["scriptMutations"] = writeResult.scriptMutationsApplied;
        runResult["saveFolder"] = writeResult.saveFolder;

        LogToWindowFormat(
            "Chaos run %u completed mutations (chaos=%d script=%d)",
            runIndex,
            writeResult.chaosMutationsApplied,
            writeResult.scriptMutationsApplied);

        if (!skipUpload)
        {
            int uploadElapsed = 0;
            std::string uploadError;
            HRESULT uploadHr = PerformUpload(state, uploadOption, uploadElapsed, uploadError);
            runResult["uploadElapsedMs"] = uploadElapsed;
            if (FAILED(uploadHr))
            {
                LogToWindowFormat("Chaos run %u upload failed: %s (0x%08X)", runIndex, uploadError.c_str(), static_cast<uint32_t>(uploadHr));
                MarkFailure(payload.result, uploadHr, uploadError);
                payload.result["runs"] = runSummaries;
                payload.result["failedRun"] = static_cast<int64_t>(runIndex);
                payload.elapsedMs = ComputeElapsedMs(overallStart);
                return payload;
            }

            ++uploadsCompleted;
            LogToWindowFormat("Chaos run %u upload succeeded (%d ms)", runIndex, uploadElapsed);

        }

        runResult["elapsedMs"] = ComputeElapsedMs(runStart);
        runSummaries.emplace_back(std::move(runResult));
    }

    payload.result["runs"] = runSummaries;
    payload.result["uploadsCompleted"] = static_cast<int64_t>(uploadsCompleted);
    payload.result["configuration"] = SerializeConfiguration(config);
    payload.result["numUploads"] = static_cast<int64_t>(numUploads);
    payload.result["skipUpload"] = skipUpload;
    payload.result["minFileBytes"] = static_cast<int64_t>(minBytes);
    payload.result["maxFileBytes"] = static_cast<int64_t>(maxBytes);

    payload.elapsedMs = ComputeElapsedMs(overallStart);
    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}
