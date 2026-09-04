#include "pch.h"

#include "ResolveSaveRoot.h"
#include "DeviceGameSaveState.h"
#include "DeviceFileSystem.h"
#include "DeviceLogging.h"

namespace fs = std::filesystem;

HRESULT ConvertFilesystemError(const std::error_code& ec)
{
    if (!ec)
    {
        return S_OK;
    }

    if (ec.category() == std::system_category())
    {
        return HRESULT_FROM_WIN32(static_cast<uint32_t>(ec.value()));
    }

    return E_FAIL;
}

HRESULT FetchSaveFolder(DeviceGameSaveState* state, std::string& folder)
{
    if (!state || !state->localUserHandle)
    {
        return E_POINTER;
    }

    if (!state->saveFolder.empty())
    {
        folder = state->saveFolder;
        LogToWindowFormat("FetchSaveFolder: using cached saveFolder='%s' (length=%zu)",
            folder.c_str(), folder.size());
        return S_OK;
    }

    size_t folderSize = 0;
    HRESULT hr = PFGameSaveFilesGetFolderSize(state->localUserHandle, &folderSize);
    if (FAILED(hr))
    {
        LogToWindowFormat("FetchSaveFolder: PFGameSaveFilesGetFolderSize FAILED (hr=0x%08X)", hr);
        return hr;
    }

    LogToWindowFormat("FetchSaveFolder: PFGameSaveFilesGetFolderSize returned folderSize=%zu", folderSize);

    std::vector<char> buffer(folderSize > 0 ? folderSize : 1);
    size_t used = 0;
    hr = PFGameSaveFilesGetFolder(state->localUserHandle, buffer.size(), buffer.data(), &used);
    if (FAILED(hr))
    {
        LogToWindowFormat("FetchSaveFolder: PFGameSaveFilesGetFolder FAILED (hr=0x%08X)", hr);
        return hr;
    }

    LogToWindowFormat("FetchSaveFolder: PFGameSaveFilesGetFolder returned used=%zu, bufferSize=%zu",
        used, buffer.size());

    size_t stringLength = (used > 0 && used <= buffer.size()) ? used - 1 : 0;
    folder.assign(buffer.data(), buffer.data() + stringLength);

    LogToWindowFormat("FetchSaveFolder: result folder='%s' (stringLength=%zu)", folder.c_str(), stringLength);
    return S_OK;
}

HRESULT ResolveSaveRoot(DeviceGameSaveState* state, fs::path& rootPath, std::string& error)
{
    std::string folder;
    HRESULT hr = FetchSaveFolder(state, folder);
    if (FAILED(hr))
    {
        error = "Failed to resolve save root via PFGameSaveFilesGetFolder";
        return hr;
    }

    LogToWindowFormat("ResolveSaveRoot: FetchSaveFolder returned folder='%s' (length=%zu)",
        folder.c_str(), folder.size());

    if (folder.empty())
    {
        error = "PFGameSaveFilesGetFolder returned an empty path";
        return E_FAIL;
    }

    // Log hex dump of first bytes to detect hidden characters (quotes, nulls, etc.)
    {
        std::string hexDump;
        for (size_t i = 0; i < folder.size() && i < 64; ++i)
        {
            char buf[8];
            snprintf(buf, sizeof(buf), "%02X ", static_cast<unsigned char>(folder[i]));
            hexDump += buf;
        }
        LogToWindowFormat("ResolveSaveRoot: folder hex: %s", hexDump.c_str());
    }

    auto mountGuard = DeviceFileSystemMount(folder);

    std::string folderPath = DeviceFileSystemGetMountedPath();

    LogToWindowFormat("ResolveSaveRoot: DeviceFileSystemGetMountedPath returned '%s' (length=%zu)",
        folderPath.c_str(), folderPath.size());

    rootPath = fs::path(folderPath);

    LogToWindowFormat("ResolveSaveRoot: fs::path constructed, native='%s'",
        GetStringFromU8String(rootPath.u8string()).c_str());

#ifdef _GAMING_XBOX
    // On Xbox, the save folder is an NT device path (e.g. \\?\GLOBALROOT\Device\...).
    // Win32 and std::filesystem APIs require a trailing separator on these paths
    // for directory operations (GetFileAttributesW, FindFirstFileW, directory_iterator)
    // to work correctly. Ensure the path ends with a separator.
    rootPath = rootPath / "";
    LogToWindowFormat("ResolveSaveRoot: Xbox path (with trailing sep)='%s'",
        GetStringFromU8String(rootPath.u8string()).c_str());
#else
    // On non-Xbox platforms, ensure the save root directory exists
    {
        LogToWindowFormat("ResolveSaveRoot: creating directories if needed");
        std::error_code ec;
        fs::create_directories(rootPath, ec);
        if (ec)
        {
            error = std::string("Failed to ensure save root directory exists: ") + ec.message();
            return ConvertFilesystemError(ec);
        }
    }

    // Validate the resolved path
    {
        std::error_code existsEc;
        bool exists = fs::exists(rootPath, existsEc);
        if (existsEc)
        {
            LogToWindowFormat("ResolveSaveRoot: fs::exists check failed (error: %s, code=%d)",
                existsEc.message().c_str(), existsEc.value());
        }
        else
        {
            LogToWindowFormat("ResolveSaveRoot: fs::exists='%s'", exists ? "true" : "false");
        }

        bool isDir = fs::is_directory(rootPath, existsEc);
        if (existsEc)
        {
            LogToWindowFormat("ResolveSaveRoot: fs::is_directory check failed (error: %s, code=%d)",
                existsEc.message().c_str(), existsEc.value());
        }
        else
        {
            LogToWindowFormat("ResolveSaveRoot: fs::is_directory='%s'", isDir ? "true" : "false");
        }
    }
#endif

    return S_OK;
}
