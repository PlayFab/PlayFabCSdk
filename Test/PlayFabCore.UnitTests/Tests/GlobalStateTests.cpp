#include "stdafx.h"
#include "TestIncludes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PlayFab
{
namespace UnitTests
{

class MemoryManager
{
public:
    MemoryManager() noexcept
    {
        s_allocCalls = 0;
        s_freeCalls = 0;

        PFMemoryHooks hooks{ MemAllocHook, MemFreeHook };
        VERIFY_SUCCEEDED(PFMemSetFunctions(&hooks));
    }

    ~MemoryManager() noexcept
    {
        PFMemoryHooks hooks{ nullptr, nullptr };
        VERIFY_SUCCEEDED(PFMemSetFunctions(&hooks));
        Assert::AreEqual(s_allocCalls.load(), s_freeCalls.load());
    }

private:
    static _Ret_maybenull_ _Post_writable_byte_size_(dwSize) void* STDAPIVCALLTYPE MemAllocHook(
        _In_ size_t dwSize,
        _In_ uint32_t memoryTypeId
    )
    {
		UNREFERENCED_PARAMETER(memoryTypeId);
        std::lock_guard<std::mutex> lock{ s_mutex };
        auto ret = new char[dwSize];
        s_allocMap[ret] = s_allocCalls++;
        return ret;
    }

    static void STDAPIVCALLTYPE MemFreeHook(
        _In_ void* pAddress,
        _In_ uint32_t memoryTypeId
    )
    {
		UNREFERENCED_PARAMETER(memoryTypeId);
        std::lock_guard<std::mutex> lock{ s_mutex };
        s_freeCalls++;
        Assert::IsTrue(s_allocMap.erase(pAddress) > 0);
        delete[] pAddress;
    }

    static std::mutex s_mutex;
    static std::atomic<size_t> s_allocCalls;
    static std::atomic<size_t> s_freeCalls;
    static std::map<void*, size_t> s_allocMap;
};

std::mutex MemoryManager::s_mutex{};
std::atomic<size_t> MemoryManager::s_allocCalls{ 0 };
std::atomic<size_t> MemoryManager::s_freeCalls{ 0 };
std::map<void*, size_t> MemoryManager::s_allocMap{};

TEST_CLASS(GlobalStateTests)
{
public:
    TEST_METHOD(CustomMemHooks)
    {
        MemoryManager manager;
        VERIFY_SUCCEEDED(PFInitialize(nullptr));

        XAsyncBlock async{};
        VERIFY_SUCCEEDED(PFUninitializeAsync(&async));
        VERIFY_SUCCEEDED(XAsyncGetStatus(&async, true));
    }

    TEST_METHOD(SetHooksAfterInit)
    {
        VERIFY_SUCCEEDED(PFInitialize(nullptr));

        PFMemoryHooks hooks{ nullptr, nullptr };
        Assert::AreEqual(E_PF_CORE_ALREADY_INITIALIZED, PFMemSetFunctions(&hooks));

        XAsyncBlock async{};
        VERIFY_SUCCEEDED(PFUninitializeAsync(&async));
        VERIFY_SUCCEEDED(XAsyncGetStatus(&async, true));
    }

    TEST_METHOD(RepeatedInitialization)
    {
        for (uint32_t i = 0; i < 10000; ++i)
        {
            VERIFY_SUCCEEDED(PFInitialize(nullptr));

            XAsyncBlock async{};
            VERIFY_SUCCEEDED(PFUninitializeAsync(&async));
            VERIFY_SUCCEEDED(XAsyncGetStatus(&async, true));
        }
    }
};

} // UnitTests
} // PlayFab