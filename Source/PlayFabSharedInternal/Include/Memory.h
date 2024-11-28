#pragma once

#include <type_traits>
#include <memory>
#include <playfab/core/PFPlatform.h>

namespace PlayFab
{

//------------------------------------------------------------------------------
// Memory Hook configuration
//------------------------------------------------------------------------------
PFMemoryHooks& GetMemoryHooks();
HRESULT SetMemoryHooks(PFMemoryHooks& hooks);
bool IsUsingCustomMemoryHooks();

//------------------------------------------------------------------------------
// Raw allocation and free methods
//------------------------------------------------------------------------------
void* Alloc(size_t size);
void Free(void* pointer) noexcept;

//------------------------------------------------------------------------------
// Allocator
//------------------------------------------------------------------------------
template<class T>
struct Allocator
{
    using value_type = T;
    using is_always_equal = std::true_type;

    Allocator() = default;
    template<class U>
    Allocator(Allocator<U> const&) {}

    T* allocate(size_t n);
    void deallocate(_In_opt_ void* p, size_t) noexcept;
};

template<class T>
bool operator==(Allocator<T> const&, Allocator<T> const&);
template<class T>
bool operator!=(Allocator<T> const&, Allocator<T> const&);

//------------------------------------------------------------------------------
// Deleter
//------------------------------------------------------------------------------
template<class T>
struct Deleter
{
    constexpr Deleter() noexcept = default;

    template<class T2, std::enable_if_t<std::is_convertible_v<T2*, T*>, int> = 0>
    Deleter(const Deleter<T2>&) noexcept;

    void operator()(T* ptr) const noexcept;
};

//------------------------------------------------------------------------------
// nlohmann Allocator
//------------------------------------------------------------------------------
struct JsonAllocator
{
public:
    static const bool kNeedFree = true;

    void* Malloc(size_t size);
    void* Realloc(void* originalPtr, size_t originalSize, size_t newSize);
    static void Free(void* ptr);
};

//------------------------------------------------------------------------------
// Smart Pointers
//------------------------------------------------------------------------------
template<class T>
using SharedPtr = std::shared_ptr<T>;

template<class T>
using WeakPtr = std::weak_ptr<T>;

template<class T, class TDeleter = Deleter<T>>
using UniquePtr = std::unique_ptr<T, TDeleter>;

//------------------------------------------------------------------------------
// Factories
//------------------------------------------------------------------------------
template<class T, class... TArgs>
SharedPtr<T> MakeShared(TArgs&&... args);

template<class T, class... TArgs>
UniquePtr<T> MakeUnique(TArgs&&... args);

//------------------------------------------------------------------------------
// Template implementations
//------------------------------------------------------------------------------
template<class T>
T* Allocator<T>::allocate(size_t n)
{
    if (n > (static_cast<size_t>(-1) / sizeof(T)))
    {
        throw std::bad_alloc{};
    }

    auto rawPtr = Alloc(n * sizeof(T)); // Bug 15586224: how to ensure alignment?
    return static_cast<T*>(rawPtr);
}

template<class T>
void Allocator<T>::deallocate(_In_opt_ void* p, size_t) noexcept
{
    Free(p);
}

template<class T>
bool operator==(Allocator<T> const&, Allocator<T> const&)
{
    return true;
}

template<class T>
bool operator!=(Allocator<T> const&, Allocator<T> const&)
{
    return false;
}

template<class T> template<class T2, std::enable_if_t<std::is_convertible_v<T2*, T*>, int>>
Deleter<T>::Deleter(const Deleter<T2>&) noexcept
{
}

template<class T>
void Deleter<T>::operator()(T* ptr) const noexcept
{
    ptr->~T(); // destroy
    Allocator<T> alloc{};
    alloc.deallocate((void*)ptr, 1);
}

template<class T, class... TArgs>
SharedPtr<T> MakeShared(TArgs&&... args)
{
    return std::allocate_shared<T>(Allocator<T>{}, std::forward<TArgs>(args)...);
}

template<class T, class... TArgs>
UniquePtr<T> MakeUnique(TArgs&&... args)
{
    // allocate
    Allocator<T> alloc{};
    auto mem = alloc.allocate(1);

    // construct (failure here only requires deallocation)
    auto obj = new (mem) T(std::forward<TArgs>(args)...);

    // destruction is required from here on
    // pass ownership to correctly typed smart pointer
    UniquePtr<T> ptr{ obj };

    // success!
    return ptr;
}

}
