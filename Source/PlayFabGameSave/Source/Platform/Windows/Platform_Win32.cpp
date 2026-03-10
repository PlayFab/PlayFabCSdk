#include "stdafx.h"
#include "Platform/Platform.h"
#include "PFGameSaveFilesAPIProvider_GRTS.h"
#include "PFGameSaveFilesAPIProvider_Win32.h"

namespace PlayFab
{

namespace GameSave
{

UniquePtr<GameSaveAPIProvider> PlatformGetAPIProvider(bool forceInproc) noexcept
{
    UNREFERENCED_PARAMETER(forceInproc);
    return UniquePtr<GameSaveAPIProvider>(MakeUnique<GameSaveAPIProviderWin32>().release());
}

} // namespace GameSave
} // namespace PlayFab