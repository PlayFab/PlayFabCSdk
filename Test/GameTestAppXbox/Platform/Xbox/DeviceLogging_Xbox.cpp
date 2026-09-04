// Copyright (C) Microsoft Corporation. All rights reserved.
// DeviceLogging_Xbox.cpp - Xbox/GDK logging implementation using DX::TextConsole
#include "pch.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "TextConsole.h"

#include <httpClient/trace.h>

#include <atomic>
#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <vector>

namespace
{
    constexpr size_t MAX_FILE_LOG_LINES = 500;

    std::vector<std::string> g_fileLogHistory;
    std::mutex g_logLinesMutex;
    std::unique_ptr<std::ofstream> g_logFile;
    std::unique_ptr<std::ofstream> g_summaryLogFile;

    // Force OS-level flush to disk (std::flush alone may not commit to disk on Xbox)
    void FlushFileToDisk(const std::filesystem::path& filePath)
    {
        HANDLE hFile = CreateFileW(
            filePath.c_str(),
            GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);
        if (hFile != INVALID_HANDLE_VALUE)
        {
            FlushFileBuffers(hFile);
            CloseHandle(hFile);
        }
    }
    bool g_fileLoggingActive = false;
    std::string g_currentLogDeviceName;
    std::filesystem::path g_logFilePath;
    std::filesystem::path g_summaryLogFilePath;
    std::atomic<DX::TextConsole*> g_textConsole(nullptr);

    std::filesystem::path GetExecutableDirectory()
    {
        char buffer[MAX_PATH] = {};
        DWORD length = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
        if (length == 0 || length == MAX_PATH)
        {
            return std::filesystem::current_path();
        }

        std::filesystem::path exePath(buffer);
        return exePath.parent_path();
    }

    std::filesystem::path GetLogsDirectory()
    {
        std::filesystem::path exeDir = GetExecutableDirectory();
        std::filesystem::path current = exeDir;
        std::error_code ec;

        while (!current.empty())
        {
            ec.clear();
            std::filesystem::path outDir = current / "Out";
            if (std::filesystem::exists(outDir, ec) && !ec)
            {
                std::filesystem::path logDir = (outDir / "logs").lexically_normal();
                std::filesystem::create_directories(logDir, ec);
                return logDir;
            }

            if (!current.has_parent_path() || current.parent_path() == current)
            {
                break;
            }

            current = current.parent_path();
        }

        return exeDir;
    }

    std::string SanitizeDeviceName(const std::string& deviceName)
    {
        if (deviceName.empty())
        {
            return std::string("device");
        }

        std::string sanitized;
        sanitized.reserve(deviceName.size());
        for (char ch : deviceName)
        {
            unsigned char uch = static_cast<unsigned char>(ch);
            if (std::iscntrl(uch))
            {
                continue;
            }

            switch (ch)
            {
            case '<': case '>': case ':': case '"':
            case '/': case '\\': case '|': case '?': case '*':
                sanitized.push_back('_');
                break;
            default:
                sanitized.push_back(ch);
                break;
            }
        }

        if (sanitized.empty())
        {
            return std::string("device");
        }

        while (!sanitized.empty() && (sanitized.back() == '.' || sanitized.back() == ' '))
        {
            sanitized.pop_back();
        }

        if (sanitized.empty())
        {
            return std::string("device");
        }

        return sanitized;
    }

    /// Generates a unique suffix for log filenames: "HHMMSS-mmm-PID"
    /// where mmm = milliseconds. Called once per EnableFileLoggingForDevice
    /// so log and summary files share the same suffix.
    std::string GenerateLogFileSuffix()
    {
        SYSTEMTIME lt;
        GetLocalTime(&lt);
        DWORD pid = GetCurrentProcessId();

        std::ostringstream oss;
        oss << std::setfill('0')
            << std::setw(2) << lt.wHour
            << std::setw(2) << lt.wMinute
            << std::setw(2) << lt.wSecond
            << '-' << std::setw(3) << lt.wMilliseconds
            << '-' << pid;
        return oss.str();
    }

    std::string PrependTimestamp(const std::string& message)
    {
        SYSTEMTIME localTime{};
        GetLocalTime(&localTime);

        char timeBuffer[16] = {};
        std::snprintf(timeBuffer, sizeof(timeBuffer), "%02u:%02u:%02u",
            static_cast<unsigned>(localTime.wHour),
            static_cast<unsigned>(localTime.wMinute),
            static_cast<unsigned>(localTime.wSecond));

        std::string result;
        result.reserve(message.size() + 12);
        result.append("[");
        result.append(timeBuffer);
        result.append("] ");
        result.append(message);
        return result;
    }

    void TrimLogVector(std::vector<std::string>& logVector, size_t maxEntries)
    {
        if (logVector.size() > maxEntries)
        {
            size_t removeCount = logVector.size() - maxEntries;
            logVector.erase(logVector.begin(), logVector.begin() + removeCount);
        }
    }

    void LogInternal(const std::string& text, bool showOnScreen)
    {
        std::string timestamped = PrependTimestamp(text);

        {
            std::lock_guard<std::mutex> lock(g_logLinesMutex);

            g_fileLogHistory.push_back(timestamped);
            TrimLogVector(g_fileLogHistory, MAX_FILE_LOG_LINES);

            if (g_fileLoggingActive && g_logFile && g_logFile->is_open())
            {
                (*g_logFile) << timestamped << std::endl;
                g_logFile->flush();
                FlushFileToDisk(g_logFilePath);
            }

            if (showOnScreen && g_fileLoggingActive && g_summaryLogFile && g_summaryLogFile->is_open())
            {
                (*g_summaryLogFile) << timestamped << std::endl;
                g_summaryLogFile->flush();
                FlushFileToDisk(g_summaryLogFilePath);
            }
        }

        if (showOnScreen)
        {
            DX::TextConsole* console = g_textConsole.load();
            if (console)
            {
                std::string line = timestamped + "\n";
                std::wstring wline(line.begin(), line.end());
                console->Write(wline.c_str());
            }
        }

        OutputDebugStringA(timestamped.c_str());
        OutputDebugStringA("\n");
    }
}

void EnableFileLoggingForDevice(const std::string& deviceName)
{
    if (deviceName.empty())
    {
        return;
    }

    std::lock_guard<std::mutex> lock(g_logLinesMutex);
    if (g_fileLoggingActive)
    {
        if (g_currentLogDeviceName == deviceName)
        {
            return;
        }

        if (g_logFile)
        {
            g_logFile->flush();
            g_logFile.reset();
        }

        g_fileLoggingActive = false;
        g_currentLogDeviceName.clear();
    }

    std::filesystem::path logDir("D:\\");
    std::string sanitizedDeviceName = SanitizeDeviceName(deviceName);
    std::string suffix = GenerateLogFileSuffix();
    std::filesystem::path logPath = logDir / ("device-" + sanitizedDeviceName + "-" + suffix + "-log.txt");
    std::filesystem::path summaryLogPath = logDir / ("device-" + sanitizedDeviceName + "-" + suffix + "-summary.txt");

    auto file = std::make_unique<std::ofstream>(logPath, std::ios::out | std::ios::trunc);
    if (!file->is_open())
    {
        return;
    }

    auto summaryFile = std::make_unique<std::ofstream>(summaryLogPath, std::ios::out | std::ios::trunc);
    if (!summaryFile->is_open())
    {
        return;
    }

    for (const auto& line : g_fileLogHistory)
    {
        (*file) << line << std::endl;
    }

    file->flush();
    summaryFile->flush();

    g_logFile = std::move(file);
    g_summaryLogFile = std::move(summaryFile);
    g_fileLoggingActive = true;
    g_currentLogDeviceName = deviceName;
    g_logFilePath = logPath;
    g_summaryLogFilePath = summaryLogPath;
}

namespace
{
    void DeviceHCTraceCallback(
        _In_z_ const char* areaName,
        _In_ HCTraceLevel level,
        _In_ uint64_t threadId,
        _In_ uint64_t timestamp,
        _In_z_ const char* message)
    {
        UNREFERENCED_PARAMETER(level);
        UNREFERENCED_PARAMETER(threadId);
        UNREFERENCED_PARAMETER(timestamp);
        LogToWindowFormatVerbose(true, "     [%s] %s", areaName, message);
    }
}

void InitializeHCTraceToVerboseLog()
{
    HCSettingsSetTraceLevel(HCTraceLevel::Verbose);
    HCTraceSetTraceToDebugger(true);
    HCTraceSetClientCallback(DeviceHCTraceCallback);
}

void CloseLogFile()
{
    std::lock_guard<std::mutex> lock(g_logLinesMutex);
    if (g_logFile)
    {
        g_logFile->flush();
        g_logFile.reset();
    }

    if (g_summaryLogFile)
    {
        g_summaryLogFile->flush();
        g_summaryLogFile.reset();
    }

    g_fileLoggingActive = false;
    g_currentLogDeviceName.clear();
    g_logFilePath.clear();
    g_summaryLogFilePath.clear();
}

void FlushDeviceLogFile()
{
    std::lock_guard<std::mutex> lock(g_logLinesMutex);
    if (g_logFile && g_logFile->is_open())
    {
        g_logFile->flush();
        FlushFileToDisk(g_logFilePath);
    }

    if (g_summaryLogFile && g_summaryLogFile->is_open())
    {
        g_summaryLogFile->flush();
        FlushFileToDisk(g_summaryLogFilePath);
    }
}

bool TryGetCurrentLogFilePath(std::string& pathOut)
{
    std::lock_guard<std::mutex> lock(g_logLinesMutex);
    if (!g_fileLoggingActive || !g_logFile || !g_logFile->is_open())
    {
        pathOut.clear();
        return false;
    }

    pathOut = g_logFilePath.u8string();
    return !pathOut.empty();
}

std::string ResolveLogFilePathForDevice(const std::string& deviceName)
{
    std::filesystem::path logDir("D:\\");
    std::string sanitizedDeviceName = SanitizeDeviceName(deviceName);
    std::string prefix = "device-" + sanitizedDeviceName + "-";

    // Log filenames now include a unique HHMMSS-mmm-PID suffix.
    // Find the most recently written file matching our prefix.
    std::filesystem::path best;
    std::filesystem::file_time_type bestTime{};
    std::error_code ec;
    for (auto& entry : std::filesystem::directory_iterator(logDir, ec))
    {
        if (!entry.is_regular_file()) continue;
        auto name = entry.path().filename().string();
        if (name.rfind(prefix, 0) == 0 && name.find("-log.txt") != std::string::npos)
        {
            auto t = entry.last_write_time(ec);
            if (!ec && (best.empty() || t > bestTime))
            {
                best = entry.path();
                bestTime = t;
            }
        }
    }
    if (!best.empty())
        return best.lexically_normal().u8string();

    // Fallback: return pattern-based path (may not exist)
    std::filesystem::path resolved = logDir / (prefix + "log.txt");
    return resolved.lexically_normal().u8string();
}

void LogToWindow(const std::string& text)
{
    LogInternal(text, true);
}

void LogToWindowVerbose(const std::string& text)
{
    LogInternal(text, false);
}

namespace
{
    void LogFormattedInternal(bool verboseOnly, const char* format, va_list args)
    {
        if (format == nullptr)
        {
            return;
        }

        va_list argsCopy;
        va_copy(argsCopy, args);
        int required = _vscprintf(format, argsCopy);
        va_end(argsCopy);

        if (required < 0)
        {
            return;
        }

        std::vector<char> buffer(static_cast<size_t>(required) + 1);
        va_copy(argsCopy, args);
        vsnprintf(buffer.data(), buffer.size(), format, argsCopy);
        va_end(argsCopy);

        bool showOnScreen = !verboseOnly;
        LogInternal(std::string(buffer.data()), showOnScreen);
    }
}

void LogToWindowFormat(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    LogFormattedInternal(false, format, args);
    va_end(args);
}

void LogToWindowFormatVerbose(bool verboseOnly, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    LogFormattedInternal(verboseOnly, format, args);
    va_end(args);
}

// Platform-specific implementations
void DeviceLoggingInitializePlatform(void* platformContext)
{
    g_textConsole.store(static_cast<DX::TextConsole*>(platformContext));
}

void UpdateWindowTitleWithDeviceName(const std::string& deviceName)
{
    std::string msg = "GameTestAppXbox - Device: " + deviceName;
    OutputDebugStringA(msg.c_str());
    OutputDebugStringA("\n");
}

void DeviceLoggingPaintPlatform(void*)
{
    // No-op: TextConsole renders itself during TestDeviceApp::Render()
}

void DeviceLoggingHandleDestroy()
{
    g_textConsole.store(nullptr);
    CloseLogFile();
}
