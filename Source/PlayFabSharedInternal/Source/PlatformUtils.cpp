#include "pch.h"
#include "PlatformUtils.h"

namespace PlayFab
{

#if !HC_PLATFORM_IS_PLAYSTATION
time_t UtcTmToTimeT(tm input)
{
#if _WIN32
    return _mkgmtime(&input);
#else
    return timegm(&input);
#endif
}
#endif

}