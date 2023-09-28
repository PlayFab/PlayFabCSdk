#include "stdafx.h"
#include "Platform.h"

#ifdef _DEBUG
// The SDK should never rely on the new or delete operators - all allocations should be hooked and
// route through the configured memory hooks (or the default memory functions, which use std::malloc and std::free).
// If these operators are ever used, it indicates we have an allocation that is unhooked and needs to be.

#if !HC_PLATFORM_IS_PLAYSTATION
void* operator new(size_t size)
{
#if HC_PLATFORM == HC_PLATFORM_GDK
    assert(false);
#endif
    return malloc(size);
}

void operator delete(void* p)
{
    if (p)
    {
        // rapidjson sometimes calls delete on a nullptr, which by C++ standard is a no-op. Don't flag this as
        // an unhooked allocation
#if HC_PLATFORM == HC_PLATFORM_GDK
        assert(false);
#endif
        free(p);
    }
}
#endif //!HC_PLATFORM_IS_PLAYSTATION
#endif

namespace PlayFab
{

PFLocalStorageHooks& GetLocalStorageHandlers()
{
    static PFLocalStorageHooks s_handlers{ nullptr, nullptr, nullptr, nullptr, nullptr };
    return s_handlers;
}

HRESULT SetLocalStorageHandlers(PFLocalStorageHooks& newHandlers)
{
    auto& handlers = GetLocalStorageHandlers();

    if (newHandlers.write && newHandlers.read && newHandlers.clear)
    {
        handlers = newHandlers;
    }
    else
    {
        return E_INVALIDARG;
    }
    return S_OK;
}

}
