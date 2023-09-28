#include "stdafx.h"
#include "Platform/Platform.h"
#include "LocalStorage_Generic.h"

namespace PlayFab
{

HRESULT PlatformInitialize() noexcept
{
    // Setup up generic LocalStorage handlers if custom ones weren't set
    PFLocalStorageHooks& storageHooks = GetLocalStorageHandlers();
    if (!storageHooks.read)
    {
        storageHooks.read = Detail::GenericLocalStorageReadAsync;
        storageHooks.write = Detail::GenericLocalStorageWriteAsync;
        storageHooks.clear = Detail::GenericLocalStorageClearAsync;
    }
    return S_OK;
}

}
