// Copyright (C) Microsoft Corporation. All rights reserved.
#include "pch.h"
#include "Common/DeviceGameSaveState.h"
#include "Common/DeviceLogging.h"

#include <httpClient/trace.h>

#include <atomic>
#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <vector>

namespace
{
    constexpr int MAX_LOG_LINES = 50;
    constexpr size_t MAX_FILE_LOG_LINES = 500;

    std::vector<std::string> g_logLines;
    std::vector<std::string> g_fileLogHistory;
    std::mutex g_logLinesMutex;
    HFONT g_hFont = nullptr;
    COLORREF g_textColor = RGB(255, 255, 255);
    COLORREF g_bgColor = RGB(0, 0, 128);
    std::unique_ptr<std::ofstream> g_logFile;
    std::unique_ptr<std::ofstream> g_summaryLogFile;
    bool g_fileLoggingActive = false;
    std::string g_currentLogDeviceName;
    std::filesystem::path g_logFilePath;
    std::filesystem::path g_summaryLogFilePath;
    std::atomic<HWND> g_logWindow(nullptr);

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

        // Fallback: use the same folder as the executable
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
                case '<':
                case '>':
                case ':':
                case '"':
                case '/':
                case '\\':
                case '|':
                case '?':
                case '*':
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
        bool shouldInvalidate = false;
        std::string timestamped = PrependTimestamp(text);

        {
            std::lock_guard<std::mutex> lock(g_logLinesMutex);

            if (showOnScreen)
            {
                g_logLines.push_back(timestamped);
                TrimLogVector(g_logLines, MAX_LOG_LINES);
                shouldInvalidate = true;
            }

            g_fileLogHistory.push_back(timestamped);
            TrimLogVector(g_fileLogHistory, MAX_FILE_LOG_LINES);

            if (g_fileLoggingActive && g_logFile && g_logFile->is_open())
            {
                (*g_logFile) << timestamped << std::endl;
                g_logFile->flush();
            }

            if (showOnScreen && g_fileLoggingActive && g_summaryLogFile && g_summaryLogFile->is_open())
            {
                (*g_summaryLogFile) << timestamped << std::endl;
                g_summaryLogFile->flush();
            }
        }

        if (shouldInvalidate)
        {
            DeviceGameSaveState* state = GetSampleGameSaveState();
            if (state != nullptr && !state->quit)
            {
                HWND hwnd = g_logWindow.load();
                if (hwnd)
                {
                    if (IsWindow(hwnd))
                    {
                        InvalidateRect(hwnd, nullptr, TRUE);
                    }
                }
            }
        }
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

    std::filesystem::path logDir = GetExecutableDirectory();
    std::string sanitizedDeviceName = SanitizeDeviceName(deviceName);
    std::filesystem::path logPath = logDir / ("pfgamesaves-test-" + sanitizedDeviceName + "-log.txt");
    std::filesystem::path summaryLogPath = logDir / ("pfgamesaves-test-" + sanitizedDeviceName + "-summary.txt");

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
    }

    if (g_summaryLogFile && g_summaryLogFile->is_open())
    {
        g_summaryLogFile->flush();
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
    std::filesystem::path logDir = GetLogsDirectory();
    std::string sanitizedDeviceName = SanitizeDeviceName(deviceName);
    std::filesystem::path resolved = logDir / ("pfgamesaves-test-" + sanitizedDeviceName + "-log.txt");
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

// Win32-specific implementations
void DeviceLoggingInitializePlatform(void* platformContext)
{
    HWND hwnd = static_cast<HWND>(platformContext);
    g_logWindow.store(hwnd);

    if (g_hFont == nullptr)
    {
        g_hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, L"Consolas");
    }
}

void UpdateWindowTitleWithDeviceName(const std::string& deviceName)
{
    HWND hwnd = g_logWindow.load();
    if (hwnd && IsWindow(hwnd))
    {
#ifdef _DEBUG
        std::wstring title = L"PlayFab Game Save Test [Debug] - " + std::wstring(deviceName.begin(), deviceName.end());
#else
        std::wstring title = L"PlayFab Game Save Test [Release] - " + std::wstring(deviceName.begin(), deviceName.end());
#endif
        SetWindowTextW(hwnd, title.c_str());
    }
}

void DeviceLoggingPaintPlatform(void* platformContext)
{
    HWND hwnd = g_logWindow.load();
    HDC hdc = static_cast<HDC>(platformContext);
    
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    HBRUSH bgBrush = CreateSolidBrush(g_bgColor);
    FillRect(hdc, &clientRect, bgBrush);
    DeleteObject(bgBrush);

    SetBkColor(hdc, g_bgColor);
    SetTextColor(hdc, g_textColor);
    SelectObject(hdc, g_hFont);

    constexpr int lineSpacing = 20;
    const int topMargin = 10;
    int yPos = topMargin;

    {
        std::lock_guard<std::mutex> lock(g_logLinesMutex);

        const int availableHeight = (clientRect.bottom - clientRect.top) - topMargin;
        size_t maxVisibleLines = availableHeight > 0 ? static_cast<size_t>(availableHeight / lineSpacing) : g_logLines.size();
        if (maxVisibleLines == 0)
        {
            maxVisibleLines = 1;
        }

        size_t startIndex = 0;
        if (g_logLines.size() > maxVisibleLines)
        {
            startIndex = g_logLines.size() - maxVisibleLines;
        }

        for (size_t i = startIndex; i < g_logLines.size(); ++i)
        {
            const std::string& line = g_logLines[i];
            TextOutA(hdc, 10, yPos, line.c_str(), static_cast<int>(line.length()));
            yPos += lineSpacing;
        }
    }
}

void DeviceLoggingHandleDestroy()
{
    g_logWindow.store(nullptr);

    if (g_hFont)
    {
        DeleteObject(g_hFont);
        g_hFont = nullptr;
    }

    CloseLogFile();
}
