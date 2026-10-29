#include "pch.h"
#include "FilePAL_Generic.h"
#include <algorithm>
#include <sys/stat.h>

namespace PlayFab
{

bool FilePAL::IsValidPath(const String& filePath) noexcept
{
    // List of characters that are not allowed in file paths
    const String invalidChars = FilePAL::GetPathInvalidChars();
    return filePath.find_first_of(invalidChars) == String::npos;
}

Result<String> FilePAL::JoinPath(const String& pathHead, const String& pathTail) noexcept
{
    // Hot during save-folder scans (called per file), so validate both inputs against one copy of
    // the invalid-character set and build the result directly rather than via FormatString.
    const String invalidChars = FilePAL::GetPathInvalidChars();
    RETURN_HR_IF(E_INVALIDARG, pathHead.find_first_of(invalidChars) != String::npos || pathTail.find_first_of(invalidChars) != String::npos);

    if (pathTail.empty())
    {
        // If there's nothing to append, just leave it as is
        return String{ pathHead.c_str() };
    }

    // c_str() keeps the old "%s" behavior of stopping at an embedded NUL.
    const char* head = pathHead.c_str();
    const char* tail = pathTail.c_str();
    char sep = GetPathSeparatorChar();
    bool headHasSepAtEnd = (pathHead.size() > 0 && pathHead[pathHead.length() - 1] == sep);
    bool tailHasSepAtStart = (pathTail.size() > 0 && pathTail[0] == sep);

    String result;
    result.reserve(pathHead.size() + pathTail.size() + 1);
    result.append(head);
    if (!headHasSepAtEnd && !tailHasSepAtStart)
    {
        // Need to add a path separator
        result.push_back(sep);
        result.append(tail);
    }
    else if (headHasSepAtEnd && tailHasSepAtStart)
    {
        // Both have separators, so skip the tail's separator
        result.append(tail + 1);
    }
    else
    {
        // One or the other has one, so no need to add
        result.append(tail);
    }
    return result;
}

Result<String> FilePAL::GetParentPath(const String& path) noexcept
{
    RETURN_HR_IF(E_INVALIDARG, !IsValidPath(path));

    // Regardless of the system's path separator, we still deal with / sometimes
    char sep = GetPathSeparatorChar();
    char universalSep = '/';
    size_t lastSep = path.find_last_of(sep);
    size_t lastUniversalSep = path.find_last_of(universalSep);
    if (lastUniversalSep != String::npos && (lastSep == String::npos || lastUniversalSep > lastSep))
    {
        lastSep = lastUniversalSep;
    }

    return (lastSep == String::npos || lastSep == 0) ? String{} : path.substr(0, lastSep);
}

// FilePAL.cpp provides the std::filesystem implementations. Other platforms provide their own
// FilePAL and use these compatibility members where save storage has no links. A platform with
// native directory-only pruning can own both DeletePathIfEmpty overloads, including root/link
// validation, by setting PF_FILEPAL_HAS_NATIVE_EMPTY_DIRECTORY_PRUNE=1 for this translation unit.
#ifndef PF_FILEPAL_NEEDS_LINK_FALLBACKS
#if HC_PLATFORM_IS_MICROSOFT || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM_IS_APPLE
#define PF_FILEPAL_NEEDS_LINK_FALLBACKS 0
#else
#define PF_FILEPAL_NEEDS_LINK_FALLBACKS 1
#endif
#endif

#if PF_FILEPAL_NEEDS_LINK_FALLBACKS

#ifndef PF_FILEPAL_HAS_NATIVE_EMPTY_DIRECTORY_PRUNE
#define PF_FILEPAL_HAS_NATIVE_EMPTY_DIRECTORY_PRUNE 0
#endif

#if !PF_FILEPAL_HAS_NATIVE_EMPTY_DIRECTORY_PRUNE
namespace
{

// Removes directoryPath bottom-up if no file is left beneath it; true when it was removed. A
// directory that still holds a file (or can't be read) is kept, along with its ancestors.
bool RemoveDirectoryIfEmptyRecursiveFallback(const String& directoryPath) noexcept
{
    Result<Vector<String>> subdirectories = FilePAL::EnumDirectories(directoryPath);
    if (FAILED(subdirectories.hr))
    {
        return false;
    }
    bool allRemoved = true;
    for (const String& subdirectory : subdirectories.Payload())
    {
        Result<String> subdirectoryPath = FilePAL::JoinPath(directoryPath, subdirectory);
        if (FAILED(subdirectoryPath.hr) || !RemoveDirectoryIfEmptyRecursiveFallback(subdirectoryPath.Payload()))
        {
            allRemoved = false;
        }
    }
    if (!allRemoved)
    {
        return false;
    }
    Result<Vector<String>> files = FilePAL::EnumFiles(directoryPath);
    if (FAILED(files.hr) || !files.Payload().empty())
    {
        return false;
    }
    return SUCCEEDED(FilePAL::DeletePath(directoryPath));
}

} // anonymous namespace
#endif

bool FilePAL::DoesEntryExist(const String& path) noexcept
{
    return DoesFileExist(path) || DoesDirectoryExist(path);
}

Result<Vector<String>> FilePAL::EnumDirectories(const String& directoryPath, Vector<String>* skippedLinks) noexcept
{
    if (skippedLinks != nullptr)
    {
        skippedLinks->clear();
    }
    return EnumDirectories(directoryPath);
}

#if !PF_FILEPAL_HAS_NATIVE_EMPTY_DIRECTORY_PRUNE
HRESULT FilePAL::DeletePathIfEmpty(const String& directoryPath, bool& fullyDeleted, const String& containingRoot) noexcept
{
    fullyDeleted = false;
    TRACE_INFORMATION("[FILEPAL] DeletePathIfEmpty %s", directoryPath.c_str());
    RETURN_HR_IF_FALSE(E_INVALIDARG, IsValidPath(directoryPath));

    if (!containingRoot.empty())
    {
        auto isSeparator = [](char c) { return c == '/' || c == '\\'; };
        String root = containingRoot;
        while (root.size() > 1 && isSeparator(root.back()))
        {
            root.pop_back();
        }
        const bool beneath = directoryPath.size() > root.size() &&
            directoryPath.compare(0, root.size(), root) == 0 &&
            (isSeparator(root.back()) || isSeparator(directoryPath[root.size()]));
        if (!beneath)
        {
            TRACE_WARNING("[FILEPAL] DeletePathIfEmpty: refusing to prune %s because it isn't beneath %s", directoryPath.c_str(), containingRoot.c_str());
            return E_INVALIDARG;
        }
    }

    // Checked first: the enumerators create a missing directory.
    if (!DoesDirectoryExist(directoryPath))
    {
        fullyDeleted = true;
        return S_OK;
    }
    fullyDeleted = RemoveDirectoryIfEmptyRecursiveFallback(directoryPath);
    TRACE_INFORMATION("[FILEPAL] DeletePathIfEmpty: fullyDeleted:%d", fullyDeleted);
    return S_OK;
}
#endif

#endif // PF_FILEPAL_NEEDS_LINK_FALLBACKS

} // namespace PlayFab