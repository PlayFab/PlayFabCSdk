#pragma once

namespace PlayFab
{
namespace Test
{
namespace Platform
{

// Simple XPlat MemoryManager
class MemoryManager
{
public:
    static MemoryManager& Instance();

    MemoryManager(MemoryManager const&) = delete;
    MemoryManager& operator=(MemoryManager) = delete;
    ~MemoryManager();

    HRESULT SetHooks();
    String Summary(); // For logging purposes

private:
    MemoryManager() = default;

    // PlayFab SDK Callbacks
    static void* STDAPIVCALLTYPE AllocHook(size_t size);
    static void  STDAPIVCALLTYPE FreeHook(void* pointer);

    std::mutex m_mutex;
    uint64_t m_nextId{ 0 };
    Map<void*, uint64_t> m_allocations;
};

} // namespace Platform
} // namespace Test
} // namespace PlayFab