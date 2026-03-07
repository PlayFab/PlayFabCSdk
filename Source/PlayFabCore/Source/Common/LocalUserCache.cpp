#include "stdafx.h"
#include "LocalUserCache.h"
#include "PFCoreGlobalState.h"
#include "LocalUser.h"
#include <playfab/core/PFLocalUser.h>

namespace PlayFab
{
namespace Core
{

LocalUserCache& LocalUserCache::Instance()
{
    static LocalUserCache instance;
    return instance;
}

void LocalUserCache::ClearAllLocalUsers()
{
    SharedPtr<PFCoreGlobalState> state;
    if (SUCCEEDED(PFCoreGlobalState::Get(state)))
    {
        std::lock_guard<std::mutex> lock(m_cacheMutex);
        
        // Simply clear all cached weak pointers
        // No need to explicitly close handles since LocalUser objects manage their own lifecycle
        state->m_localUserCache.clear();
    }
}

SharedPtr<LocalUser> LocalUserCache::GetLocalUser(const String& localId) const
{
    SharedPtr<PFCoreGlobalState> state;
    if (SUCCEEDED(PFCoreGlobalState::Get(state)))
    {
        auto it = state->m_localUserCache.find(localId);
        if (it != state->m_localUserCache.end())
        {
            // Try to lock the weak pointer to get a shared pointer
            return it->second.lock();
        }
    }

    return nullptr;
}

void LocalUserCache::StoreLocalUser(const String& localId, SharedPtr<LocalUser> localUser)
{
    SharedPtr<PFCoreGlobalState> state;
    if (SUCCEEDED(PFCoreGlobalState::Get(state)))
    {
        state->m_localUserCache.insert_or_assign(localId, localUser);
    }
}

SharedPtr<LocalUser> LocalUserCache::GetOrStoreLocalUser(SharedPtr<LocalUser> newUser)
{
    String localIdForCache = newUser->LocalId();

    if (localIdForCache.empty())
    {
        return nullptr; // Cannot cache without a valid LocalId
    }

    std::lock_guard<std::mutex> lock(m_cacheMutex);

    // Verify if the user exists in cache
    SharedPtr<LocalUser> cachedUser = GetLocalUser(localIdForCache);
    
    if (cachedUser)
    {
        // Prefer the cached version over the newly created one
        return cachedUser;
    }

    // Cache the newly created user
    StoreLocalUser(localIdForCache, newUser);

    return newUser;
}

} // namespace Core
} // namespace PlayFab
