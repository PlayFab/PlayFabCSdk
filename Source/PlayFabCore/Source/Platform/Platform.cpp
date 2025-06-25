#include "stdafx.h"
#include "Platform.h"

// Global new/delete operator overrides
void* operator new[](size_t size)
{
    void* buffer;

    buffer = PlayFab::Alloc(size);
    if (buffer == nullptr)
    {
        throw std::bad_alloc();
    }
    return buffer;
}

void* operator new(size_t size)
{
    void* buffer;

    buffer = PlayFab::Alloc(size);
    if (buffer == nullptr)
    {
        throw std::bad_alloc();
    }
    return buffer;
}

void* operator new(size_t size, const std::nothrow_t&) throw()
{
    return PlayFab::Alloc(size);
}

void* operator new[](size_t size, const std::nothrow_t&) throw()
{
    return PlayFab::Alloc(size);
}

void operator delete[](void* pointer, const std::nothrow_t&) throw()
{
    if (pointer != nullptr)
    {
        PlayFab::Free(pointer);
    }
}

void operator delete[](void* pointer, unsigned __int64) throw()
{
    if (pointer != nullptr)
    {
        PlayFab::Free(pointer);
    }
}

void operator delete(void* pointer, const std::nothrow_t&) throw()
{
    if (pointer != nullptr)
    {
        PlayFab::Free(pointer);
    }
}

void operator delete(void* pointer, unsigned __int64) throw()
{
    if (pointer != nullptr)
    {
        PlayFab::Free(pointer);
    }
}

void operator delete(void* pointer) throw()
{
    if (pointer != nullptr)
    {
        PlayFab::Free(pointer);
    }
}

void operator delete[](void* pointer) throw()
{
    if (pointer != nullptr)
    {
        PlayFab::Free(pointer);
    }
}

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
