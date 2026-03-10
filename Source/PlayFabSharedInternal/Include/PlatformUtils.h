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
    destination[destinationSize - 1] = '\0';  // Ensure null-termination
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

inline String FormatString(_In_z_ _Printf_format_string_ const char* format, ...)
{
    va_list args1;
    va_start(args1, format);

    va_list args2;
    va_copy(args2, args1);

    Vector<char> buffer(1 + std::vsnprintf(NULL, 0, format, args1));
    va_end(args1);

    std::vsnprintf(buffer.data(), buffer.size(), format, args2);
    va_end(args2);

    String strBuffer(buffer.data(), buffer.size() - 1);

    return strBuffer;
}

// Base64 encoding
inline String Base64Encode(const String& in)
{
    static const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    String out;
    out.reserve(((in.size() + 2) / 3) * 4);

    size_t i = 0;
    while (i < in.size())
    {
        // Track how many bytes we read in this group
        size_t bytes_in_group = 0;
        uint32_t octet_a = 0, octet_b = 0, octet_c = 0;
        
        if (i < in.size()) { octet_a = static_cast<uint8_t>(in[i++]); bytes_in_group++; }
        if (i < in.size()) { octet_b = static_cast<uint8_t>(in[i++]); bytes_in_group++; }
        if (i < in.size()) { octet_c = static_cast<uint8_t>(in[i++]); bytes_in_group++; }

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        out.push_back(base64_chars[(triple >> 3 * 6) & 0x3F]);
        out.push_back(base64_chars[(triple >> 2 * 6) & 0x3F]);
        out.push_back(bytes_in_group < 2 ? '=' : base64_chars[(triple >> 1 * 6) & 0x3F]);
        out.push_back(bytes_in_group < 3 ? '=' : base64_chars[(triple >> 0 * 6) & 0x3F]);
    }

    return out;
}

// Base64 decoding - returns empty string if input is not valid base64
inline String Base64Decode(const String& input)
{
    static const unsigned char decode_table[128] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 0-15
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 16-31
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 62,   0xff, 0xff, 0xff, 63,   // 32-47 (+, /)
        52,   53,   54,   55,   56,   57,   58,   59,   60,   61,   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 48-63 (0-9)
        0xff, 0,    1,    2,    3,    4,    5,    6,    7,    8,    9,    10,   11,   12,   13,   14,   // 64-79 (A-O)
        15,   16,   17,   18,   19,   20,   21,   22,   23,   24,   25,   0xff, 0xff, 0xff, 0xff, 0xff, // 80-95 (P-Z)
        0xff, 26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,   40,   // 96-111 (a-o)
        41,   42,   43,   44,   45,   46,   47,   48,   49,   50,   51,   0xff, 0xff, 0xff, 0xff, 0xff  // 112-127 (p-z)
    };

    if (input.empty())
    {
        return String();
    }

    // Validate input: must only contain valid base64 characters (plus whitespace)
    // '=' padding is only valid at the end
    size_t padding_count = 0;
    bool padding_started = false;
    for (size_t i = 0; i < input.size(); i++)
    {
        unsigned char c = static_cast<unsigned char>(input[i]);
        if (c == '=')
        {
            padding_count++;
            padding_started = true;
        }
        else if (padding_started && (c != '\r' && c != '\n' && c != ' ' && c != '\t'))
        {
            // Invalid character after padding - not valid base64
            return String();
        }
        else if (c < 128 && decode_table[c] != 0xff)
        {
            // Valid base64 character
        }
        else if (c == '\r' || c == '\n' || c == ' ' || c == '\t')
        {
            // Whitespace is tolerated
        }
        else
        {
            // Invalid character - not valid base64
            return String();
        }
    }
    if (padding_count > 2)
    {
        // More than two padding characters - not valid base64
        return String();
    }

    String output;
    output.reserve(input.size() * 3 / 4);

    const unsigned char* src = reinterpret_cast<const unsigned char*>(input.data());
    size_t len = input.size();

    while (len > 0)
    {
        // Collect the next group of (up to) four characters
        int v = 0;
        int group_size = 0;
        while (group_size < 4 && len > 0)
        {
            unsigned char c = *src++;
            len--;

            // Skip whitespace
            if (c == '\r' || c == '\n' || c == ' ' || c == '\t')
            {
                continue;
            }
            // '=' padding indicates final group
            if (c == '=')
            {
                // Check if there are non-whitespace characters after '='
                while (len > 0)
                {
                    c = *src++;
                    len--;
                    if (c != '\r' && c != '\n' && c != ' ' && c != '\t' && c != '=')
                    {
                        return String(); // Invalid base64 due to trailing data after padding
                    }
                }
                break;
            }
            // Skip invalid characters
            if (c > 127 || decode_table[c] == 0xff)
            {
                continue;
            }
            v <<= 6;
            v |= decode_table[c];
            group_size++;
        }
        // Align a short group properly
        v <<= 6 * (4 - group_size);
        // Unpack the group we just collected
        switch (group_size)
        {
        case 4:
            output.push_back(static_cast<char>((v >> 16) & 0xff));
            output.push_back(static_cast<char>((v >> 8) & 0xff));
            output.push_back(static_cast<char>(v & 0xff));
            break;
        case 3:
            output.push_back(static_cast<char>((v >> 16) & 0xff));
            output.push_back(static_cast<char>((v >> 8) & 0xff));
            break;
        case 2:
            output.push_back(static_cast<char>((v >> 16) & 0xff));
            break;
        default:
            break;
        }
    }

    return output;
}

}
