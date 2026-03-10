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

    // Check if the filePath contains any of the invalid characters
    return std::none_of(filePath.begin(), filePath.end(),
        [&invalidChars](char c)
        {
            return invalidChars.find(c) != std::string::npos;
        });
}

Result<String> FilePAL::JoinPath(const String& pathHead, const String& pathTail) noexcept
{
    RETURN_HR_IF(E_INVALIDARG, !IsValidPath(pathHead) || !IsValidPath(pathTail));

    if (pathTail.empty())
    {
        // If there's nothing to append, just leave it as is
        return String{ pathHead.c_str() };
    }

    char sep = GetPathSeparatorChar();
    bool headHasSepAtEnd = (pathHead.size() > 0 && pathHead[pathHead.length() - 1] == sep);
    bool tailHasSepAtStart = (pathTail.size() > 0 && pathTail[0] == sep);
    if (!headHasSepAtEnd && !tailHasSepAtStart)
    {
        // Need to add a path separator
        return FormatString("%s%c%s", pathHead.c_str(), sep, pathTail.c_str());
    }
    else if (headHasSepAtEnd && tailHasSepAtStart)
    {
        // Both have separators, so skip the tail's separator
        return FormatString("%s%s", pathHead.c_str(), &pathTail.c_str()[1]);
    }
    else
    {
        // One or the other has one, so no need to add
        return FormatString("%s%s", pathHead.c_str(), pathTail.c_str());
    }
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

} // namespace PlayFab