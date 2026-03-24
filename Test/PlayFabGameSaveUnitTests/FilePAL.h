// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#pragma once

class FilePALTestApp
{
public:
    static std::vector<char> ReadBinaryFile(const std::string& filepath);
    static HRESULT WriteBinaryFile(const std::string& filepath, const std::vector<char>& data);
    static bool DoesDirectoryExist(const std::string& directoryPath);
    static bool DoesFileExist(const std::string& filepath);
    static HRESULT DeleteLocalFile(const std::string& filepath);
    static std::string AppendPath(const std::string& p1, const std::string& p2);
    static HRESULT CreatePath(const std::string& path);
    static HRESULT DeletePath(const std::string& path);
    static std::vector<std::string> EnumFilesInFolder(const std::string& folderPath);
    static uint64_t GetFileSize(const std::string& filePath);
    static std::string GetDeviceID();
};
