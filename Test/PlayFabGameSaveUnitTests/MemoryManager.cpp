#include "pch.h"
#include "MemoryManager.h"
#include "XAsyncProviderBase.h"
#include <string>
#include <sstream>

MemoryManager::~MemoryManager()
{
    // Unfreed allocation! an allocation made by the SDK leaked. This could be because the title forgot to close a handle
    // or because the SDK failed to delete something it internally allocated
    //assert(m_allocations.empty());
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

std::string MemoryManager::Summary()
{
    std::unique_lock<std::mutex> lock{ m_mutex };

    std::stringstream ss;
    ss << "Total allocations: " << m_nextId << " Un-freed allocations: " << m_allocations.size();
    return ss.str();
}

bool MemoryManager::HasUnfreedAllocations()
{
    std::unique_lock<std::mutex> lock{ m_mutex };
    return (m_allocations.size() > 0);
}

void* STDAPIVCALLTYPE MemoryManager::AllocHook(size_t size, uint32_t memoryTypeId)
{
    UNREFERENCED_PARAMETER(memoryTypeId);
    auto& memoryManager = MemoryManager::Instance();

    std::unique_lock<std::mutex> lock{ memoryManager.m_mutex };

    void* ptr = malloc(size);
    memoryManager.m_allocations[ptr] = memoryManager.m_nextId++;
    return ptr;
}

void STDAPIVCALLTYPE MemoryManager::FreeHook(void* pointer, uint32_t memoryTypeId)
{
    UNREFERENCED_PARAMETER(memoryTypeId);
    auto& memoryManager = MemoryManager::Instance();

    std::unique_lock<std::mutex> lock{ memoryManager.m_mutex };

    auto iter = memoryManager.m_allocations.find(pointer);
    if (iter == memoryManager.m_allocations.end())
    {
        throw std::exception{ "PlayFab Free with no matching Alloc!" };
    }
    memoryManager.m_allocations.erase(iter);

    free(pointer);
}