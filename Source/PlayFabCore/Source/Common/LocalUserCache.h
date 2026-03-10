#pragma once

#include <playfab/core/PFTypes.h>
#include <playfab/core/PFLocalUser.h>
#include <mutex>
#include "LocalUser.h"

namespace PlayFab
{

class PFCoreGlobalState; // Forward declaration

namespace Core
{

/// <summary>
/// Caching layer for PlayFab local users.
/// This class provides a convenience interface to the login cache functionality
/// that is now managed by PFCoreGlobalState.
/// </summary>
class LocalUserCache
{
public:
    /// <summary>
    /// Get the singleton instance of the local user cache
    /// </summary>
    static LocalUserCache& Instance();

    /// <summary>
    /// Clear all cached local users
    /// </summary>
    void ClearAllLocalUsers();

    /// <summary>
    /// Get a cached user by LocalId, or store and return the provided user if not cached.
    /// Uses "cache first" pattern: checks cache first using the user's LocalId,
    /// returns cached user if found, otherwise stores the provided user and returns it.
    /// </summary>
    /// <param name="newUser">The user object to store if not found in cache</param>
    /// <returns>The LocalUser (from cache or the provided user)</returns>
    SharedPtr<LocalUser> GetOrStoreLocalUser(SharedPtr<LocalUser> newUser);

private:
    LocalUserCache() = default;
    ~LocalUserCache() = default;

    // Prevent copying and assignment
    LocalUserCache(const LocalUserCache&) = delete;
    LocalUserCache& operator=(const LocalUserCache&) = delete;
    
    // Private helper methods for use within locked sections
    SharedPtr<LocalUser> GetLocalUser(const String& localId) const;
    void StoreLocalUser(const String& localId, SharedPtr<LocalUser> localUser);

    // Cache mutex for thread-safe access
    mutable std::mutex m_cacheMutex;
};

} // namespace Core
} // namespace PlayFab
