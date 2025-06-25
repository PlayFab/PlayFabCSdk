#include "stdafx.h"
#include "Platform.h"
#include "Memory.h"
#include <playfab/core/PFPlatform.h>

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

void operator delete( void* pointer, const std::nothrow_t&) throw()
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

HRESULT InitializePlatformHooks()
{
    PFMemoryHooks hooks{};
    RETURN_IF_FAILED(PFMemGetFunctions(&hooks));
    return SetMemoryHooks(hooks);
}

}
