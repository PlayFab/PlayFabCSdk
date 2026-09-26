#include "pch.h"

#include "HoldFileOpen.h"
#include "CommandHandlerShared.h"
#include "DeviceLogging.h"
#include "ResolveSaveRoot.h"

#include <filesystem>
#include <mutex>
#include <sstream>
#include <vector>

#include "CommandRegistry.h"

namespace
{
    namespace fs = std::filesystem;

    std::mutex g_heldFilesMutex;
    std::vector<std::pair<std::string, HANDLE>> g_heldFiles;

    std::wstring ToWide(const std::string& utf8)
    {
        if (utf8.empty())
        {
            return std::wstring();
        }

        int length = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), nullptr, 0);
        if (length <= 0)
        {
            return std::wstring();
        }

        std::wstring wide(static_cast<size_t>(length), L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), &wide[0], length);
        return wide;
    }
}

void ReleaseAllHeldFiles()
{
    std::lock_guard<std::mutex> lock(g_heldFilesMutex);
    for (auto& held : g_heldFiles)
    {
        if (held.second != INVALID_HANDLE_VALUE)
        {
            CloseHandle(held.second);
        }
    }
    g_heldFiles.clear();
}

CommandResultPayload HandleHoldFileOpen(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            std::string parseError;

            // Either an explicit absolute path, or a path relative to the device's save root.
            std::string relativePath;
            std::string absolutePath;
            const bool hasRelative = CommandHandlerShared::TryGetStringParameter(parameters, "relativePath", relativePath, parseError);
            const bool hasAbsolute = CommandHandlerShared::TryGetStringParameter(parameters, "filePath", absolutePath, parseError);

            if (!hasRelative && !hasAbsolute)
            {
                CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG,
                    "HoldFileOpen requires either 'relativePath' or 'filePath'");
                return E_INVALIDARG;
            }

            fs::path target;
            if (hasAbsolute)
            {
                target = fs::path(absolutePath);
            }
            else
            {
                fs::path rootPath;
                std::string resolveError;
                HRESULT resolveHr = ResolveSaveRoot(state, rootPath, resolveError);
                if (FAILED(resolveHr))
                {
                    CommandHandlerShared::MarkFailure(payload.result, resolveHr, "HoldFileOpen: " + resolveError);
                    return resolveHr;
                }
                target = rootPath / fs::path(relativePath);
            }

            // Default to denying all sharing, which is what reproduces the reported failure.
            // 'allowRead' relaxes it to FILE_SHARE_READ so a scenario can prove that a merely
            // read-shared file does NOT break the upload.
            bool allowRead = false;
            std::string ignoredError;
            CommandHandlerShared::TryParseBoolParameter(parameters, "allowRead", allowRead, ignoredError);
            const DWORD shareMode = allowRead ? FILE_SHARE_READ : 0;

            HANDLE handle = CreateFileW(
                ToWide(target.string()).c_str(),
                GENERIC_READ | GENERIC_WRITE,
                shareMode,
                nullptr,
                OPEN_EXISTING,
                0,
                nullptr);

            if (handle == INVALID_HANDLE_VALUE)
            {
                HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
                std::ostringstream oss;
                oss << "HoldFileOpen: could not open '" << target.string() << "' to hold it (hr=0x"
                    << std::hex << hr << ")";
                CommandHandlerShared::MarkFailure(payload.result, hr, oss.str());
                return hr;
            }

            {
                std::lock_guard<std::mutex> lock(g_heldFilesMutex);
                g_heldFiles.emplace_back(target.string(), handle);
            }

            LogToWindowFormat("HoldFileOpen holding '%s' (shareMode=%lu)", target.string().c_str(), shareMode);

            payload.result["filePath"] = target.string();
            payload.result["allowRead"] = allowRead;
            return S_OK;
        });
}

CommandResultPayload HandleReleaseHeldFiles(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(state);
    UNREFERENCED_PARAMETER(parameters);

    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            size_t released = 0;
            {
                std::lock_guard<std::mutex> lock(g_heldFilesMutex);
                released = g_heldFiles.size();
            }

            ReleaseAllHeldFiles();
            LogToWindowFormat("ReleaseHeldFiles released %zu handle(s)", released);

            payload.result["released"] = static_cast<int64_t>(released);
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "HoldFileOpen", HandleHoldFileOpen },
    { "ReleaseHeldFiles", HandleReleaseHeldFiles }
});
