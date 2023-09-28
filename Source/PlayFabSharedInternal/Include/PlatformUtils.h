#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "Types.h"

namespace PlayFab
{

// The primary purpose of these format strings is to communicate to and from the PlayFab server with consistent accuracy across platforms supported by this SDK
constexpr char TIMESTAMP_READ_FORMAT[] = "%Y-%m-%dT%T";
constexpr char TIMESTAMP_WRITE_FORMAT[] = "%Y-%m-%dT%H:%M:%S.000Z";
constexpr int TIMESTAMP_BUFFER_SIZE = 64; // Arbitrary number sufficiently large enough to contain the timestamp strings sent by PlayFab server

// Time type conversions
inline time_t TimePointToTimeT(const TimePoint& input)
{
    return SystemClock::to_time_t(input);
}

inline TimePoint TimeTToTimePoint(time_t input)
{
    return SystemClock::from_time_t(input);
}

inline tm TimeTToUtcTm(time_t input)
{
    tm timeInfo{ 0 };

#if HC_PLATFORM_IS_PLAYSTATION
    gmtime_s(&input, &timeInfo);
#elif _WIN32
    gmtime_s(&timeInfo, &input);
#else
    gmtime_r(&input, &timeInfo);
#endif
    return timeInfo;
}

time_t UtcTmToTimeT(tm input);

inline tm TimePointToUtcTm(const TimePoint& input)
{
    return TimeTToUtcTm(SystemClock::to_time_t(input));
}

inline TimePoint UtcTmToTimePoint(const tm& input)
{
    return TimeTToTimePoint(UtcTmToTimeT(input));
}

// Get Time now - Platform dependent granularity (granularity: upto 1 second, accuracy within a few seconds)
inline TimePoint GetTimePointNow()
{
    // The conversion is mostly to ensure consistent behavior among all platforms
    return std::chrono::time_point_cast<std::chrono::seconds>(SystemClock::now());
}

inline time_t GetTimeTNow()
{
    return TimePointToTimeT(GetTimePointNow());
}

// Get a tick count that represents now in milliseconds (not useful for absolute time)
inline int64_t GetMilliTicks()
{
    auto msClock = std::chrono::time_point_cast<std::chrono::milliseconds>(Clock::now());
    return msClock.time_since_epoch().count();
}

// Time Serialization
inline String UtcTmToIso8601String(const tm& input)
{
    char buff[TIMESTAMP_BUFFER_SIZE];
    strftime(buff, TIMESTAMP_BUFFER_SIZE, TIMESTAMP_WRITE_FORMAT, &input);
    return buff;
}

inline tm Iso8601StringToTm(const String& utcString)
{
    tm timeInfo{ 0 };
    Stringstream ss(utcString);
    ss >> std::get_time(&timeInfo, TIMESTAMP_READ_FORMAT);
    return timeInfo;
}

inline String TimeTToIso8601String(time_t input)
{
    return UtcTmToIso8601String(TimeTToUtcTm(input));
}

inline time_t Iso8601StringToTimeT(const String& input)
{
    return UtcTmToTimeT(Iso8601StringToTm(input));
}

inline String TimePointToIso8601String(const TimePoint& input)
{
    return UtcTmToIso8601String(TimePointToUtcTm(input));
}

inline TimePoint Iso8601StringToTimePoint(const String& input)
{
    return UtcTmToTimePoint(Iso8601StringToTm(input));
}

inline void StrCpy(char* destination, size_t destinationSize, char const* source)
{
#if _WIN32
    strcpy_s(destination, destinationSize, source);
#else
    std::strncpy(destination, source, destinationSize);
#endif
}

inline char* StrTok(char* str, char const* delimiters, char** context)
{
#if _WIN32
    return strtok_s(str, delimiters, context);
#else
    return strtok_r(str, delimiters, context);
#endif
}

}
