#include "TestAppPch.h"
#include "Platform/Generic/MemoryManager.h"
#include "XAsyncProviderBase.h"

namespace PlayFab
{
namespace Test
{
namespace Platform
{

MemoryManager::~MemoryManager()
{
    // Unfreed allocation! an allocation made by the SDK leaked. This could be because the title forgot to close a handle
    // or because the SDK failed to delete something it internally allocated
    assert(m_allocations.empty());
}

MemoryManager& MemoryManager::Instance()
{
    static MemoryManager s_memoryManager;
    return s_memoryManager;
}

HRESULT MemoryManager::SetHooks()
{
    // Init PlayFab hooks. These will be used by allocations with the PlayFab SDK
    PFMemoryHooks playFabHooks{ AllocHook, FreeHook };
    return PFMemSetFunctions(&playFabHooks);
}

String MemoryManager::Summary()
{
    std::unique_lock<std::mutex> lock{ m_mutex };

    Stringstream ss;
    ss << "Total allocations: " << m_nextId << " Un-freed allocations: " << m_allocations.size();
    return ss.str();
}

void* STDAPIVCALLTYPE MemoryManager::AllocHook(size_t size)
{
    auto& memoryManager = MemoryManager::Instance();

    std::unique_lock<std::mutex> lock{ memoryManager.m_mutex };

    void* ptr = malloc(size);
    memoryManager.m_allocations[ptr] = memoryManager.m_nextId++;
    return ptr;
}

void STDAPIVCALLTYPE MemoryManager::FreeHook(void* pointer)
{
    auto& memoryManager = MemoryManager::Instance();

    std::unique_lock<std::mutex> lock{ memoryManager.m_mutex };

    auto iter = memoryManager.m_allocations.find(pointer);
    if (iter == memoryManager.m_allocations.end())
    {
        throw Exception{ "PlayFab Free with no matching Alloc!" };
    }
    memoryManager.m_allocations.erase(iter);

    free(pointer);
}

} // namespace Platform
} // namespace Test
} // namespace PlayFab
