// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#pragma once

#include "pch.h"
#include <filesystem>
#include "FilePAL.h"

std::vector<char> FilePALTestApp::ReadBinaryFile(const std::string& filepath)
{
    std::vector<char> buffer;
    std::ifstream file(filepath.c_str(), std::ios::binary);
    if (file) 
    {
        // Determine the file size
        file.seekg(0, std::ios::end);
        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        buffer.resize(fileSize);

        // Read file contents into buffer
        file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

        file.close();
    }

    return buffer;
}

HRESULT FilePALTestApp::WriteBinaryFile(const std::string& filepath, const std::vector<char>& data)
{
    std::ofstream file(filepath.c_str(), std::ios::binary);

    if (file) 
    {
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        file.close();
    }

    return S_OK;
}

bool FilePALTestApp::DoesDirectoryExist(const std::string& directoryPath) 
{
    try
    {
        return std::filesystem::is_directory(directoryPath);
    }
    catch (...)
    {
        return false;
    }
}

bool FilePALTestApp::DoesFileExist(const std::string& filepath)
{
    std::ifstream file(filepath.c_str());
    return file.good();
}

HRESULT FilePALTestApp::DeleteLocalFile(const std::string& filePath)
{
    TRACE_INFORMATION("[FILEPALTESTAPP] DeleteLocalFile %s", filePath.c_str());
    RETURN_HR_IF_FALSE(E_INVALIDARG, DoesFileExist(filePath));

    try
    {
        bool result = std::filesystem::remove(filePath);
        TRACE_INFORMATION("[FILEPALTESTAPP] std::filesystem::remove %d", result);
        bool exists = DoesFileExist(filePath);
        TRACE_INFORMATION("[FILEPALTESTAPP] DoesFileExist %d", exists);
        return result ? S_OK : E_FAIL;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

std::string FilePALTestApp::AppendPath(const std::string& p1, const std::string& p2) 
{
#ifdef _WIN32
    char sep = '\\';
#else
    char sep = '/';
#endif

    if (p2.empty())
    {
        return p1;
    }

    bool p1HasSepAtEnd = (p1.size() > 0 && p1[p1.length()-1] == sep);
    bool p2HasSepAtStart = (p2.size() > 0 && p2[0] == sep);
    if (!p1HasSepAtEnd && !p2HasSepAtStart)
    {
        // Need to add a path separator
        std::string tmp = p1;
        tmp += sep;
        tmp += p2;
        return tmp;
    }
    else
    {
        std::string tmp = p1 + p2;
        return tmp;
    }
}

HRESULT FilePALTestApp::CreatePath(const std::string& path)
{
    RETURN_HR_IF(S_OK, DoesDirectoryExist(path));

    try
    {
        std::error_code error;
        auto result = std::filesystem::create_directories(path, error);
        RETURN_HR_IF(E_FAIL, error);

        return S_OK;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

HRESULT FilePALTestApp::DeletePath(const std::string& directoryPath)
{
    TRACE_INFORMATION("[FILEPALTESTAPP] DeletePath %s", directoryPath.c_str());
    RETURN_HR_IF_FALSE(S_OK, DoesDirectoryExist(directoryPath));

    try
    {
        std::error_code ec;
        std::filesystem::remove_all(directoryPath, ec);
        if (!ec)
        {
            if (DoesDirectoryExist(directoryPath))
            {
                TRACE_INFORMATION("[FILEPALTESTAPP] std::filesystem::remove_all failure");
                return E_FAIL;
            }
        }

        TRACE_INFORMATION("[FILEPALTESTAPP] std::filesystem::remove_all success");
        return S_OK;
    }
    catch (...)
    {
        return E_FAIL;
    }
}


std::vector<std::string> FilePALTestApp::EnumFilesInFolder(const std::string& folderPath)
{
    if (!FilePALTestApp::DoesDirectoryExist(folderPath))
    {
        FilePALTestApp::CreatePath(folderPath);
    }

    std::vector<std::string> fileNames;
    try
    {
        for (const auto& entry : std::filesystem::directory_iterator(folderPath.c_str()))
        {
            if (std::filesystem::is_regular_file(entry.path()))
            {
                std::string fileName = entry.path().filename().string().c_str();
                fileNames.push_back(std::move(fileName));
            }
        }
    }
    catch (...)
    {
    }

    return fileNames;
}

uint64_t FilePALTestApp::GetFileSize(const std::string& filePath)
{
    if (!DoesFileExist(filePath))
    {
        return 0;
    }

    try
    {
        uint64_t fileSize = static_cast<uint64_t>(std::filesystem::file_size(filePath));
        return fileSize;
    }
    catch (...)
    {
        return 0;
    }
}

std::string FilePALTestApp::GetDeviceID()
{
    // Just doing something trivial for now
    // Could be GUID that's cached to file or whatever PF auth stack ends up using...
    char deviceId[MAX_COMPUTERNAME_LENGTH + 1] = {};
    DWORD nSize = MAX_COMPUTERNAME_LENGTH;
    GetComputerNameA(deviceId, &nSize);
    return deviceId;
}
