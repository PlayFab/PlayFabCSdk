// Copyright (C) Microsoft Corporation. All rights reserved.
#include "pch.h"

#include "Common/HarnessHash.h"

#include <bcrypt.h>

#include <iomanip>
#include <sstream>
#include <vector>

#pragma comment(lib, "bcrypt.lib")

namespace
{
    HRESULT ConvertNtStatusToHResult(NTSTATUS status)
    {
        if (BCRYPT_SUCCESS(status))
        {
            return S_OK;
        }

        return HRESULT_FROM_NT(status);
    }

    std::string FormatHrMessage(const char* context, HRESULT hr)
    {
        std::ostringstream oss;
        oss << context << " (hr=0x" << std::uppercase << std::hex << std::setw(8) << std::setfill('0')
            << static_cast<uint32_t>(hr) << ")";
        return oss.str();
    }

    struct FileHandle
    {
        HANDLE value{ INVALID_HANDLE_VALUE };

        ~FileHandle()
        {
            if (value != INVALID_HANDLE_VALUE)
            {
                CloseHandle(value);
                value = INVALID_HANDLE_VALUE;
            }
        }
    };

    struct AlgorithmHandle
    {
        BCRYPT_ALG_HANDLE value{ nullptr };

        ~AlgorithmHandle()
        {
            if (value)
            {
                BCryptCloseAlgorithmProvider(value, 0);
                value = nullptr;
            }
        }
    };

    struct HashHandle
    {
        BCRYPT_HASH_HANDLE value{ nullptr };

        ~HashHandle()
        {
            if (value)
            {
                BCryptDestroyHash(value);
                value = nullptr;
            }
        }
    };
}

namespace HarnessHash
{
    HRESULT ComputeSha256(
        const std::filesystem::path& filePath,
        std::string& hexValue,
        std::string& errorMessage)
    {
        hexValue.clear();
        errorMessage.clear();

        if (filePath.empty())
        {
            errorMessage = "File path is empty";
            return E_INVALIDARG;
        }

        FileHandle file;
        file.value = CreateFileW(
            filePath.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            nullptr,
            OPEN_EXISTING,
            FILE_FLAG_SEQUENTIAL_SCAN,
            nullptr);
        if (file.value == INVALID_HANDLE_VALUE)
        {
            const DWORD lastError = GetLastError();
            std::ostringstream oss;
            oss << "Failed to open file '" << filePath.string() << "' (error=" << lastError << ")";
            errorMessage = oss.str();
            return HRESULT_FROM_WIN32(lastError);
        }

        AlgorithmHandle algorithm;
        NTSTATUS status = BCryptOpenAlgorithmProvider(
            &algorithm.value,
            BCRYPT_SHA256_ALGORITHM,
            nullptr,
            0);
        if (!BCRYPT_SUCCESS(status))
        {
            HRESULT hr = ConvertNtStatusToHResult(status);
            errorMessage = FormatHrMessage("BCryptOpenAlgorithmProvider failed", hr);
            return hr;
        }

        DWORD objectLength = 0;
        DWORD hashLength = 0;
        ULONG bytesProduced = 0;

        status = BCryptGetProperty(
            algorithm.value,
            BCRYPT_OBJECT_LENGTH,
            reinterpret_cast<PUCHAR>(&objectLength),
            sizeof(objectLength),
            &bytesProduced,
            0);
        if (!BCRYPT_SUCCESS(status))
        {
            HRESULT hr = ConvertNtStatusToHResult(status);
            errorMessage = FormatHrMessage("BCryptGetProperty(BCRYPT_OBJECT_LENGTH) failed", hr);
            return hr;
        }

        status = BCryptGetProperty(
            algorithm.value,
            BCRYPT_HASH_LENGTH,
            reinterpret_cast<PUCHAR>(&hashLength),
            sizeof(hashLength),
            &bytesProduced,
            0);
        if (!BCRYPT_SUCCESS(status))
        {
            HRESULT hr = ConvertNtStatusToHResult(status);
            errorMessage = FormatHrMessage("BCryptGetProperty(BCRYPT_HASH_LENGTH) failed", hr);
            return hr;
        }

        std::vector<uint8_t> hashObject(objectLength);
        HashHandle hash;
        status = BCryptCreateHash(
            algorithm.value,
            &hash.value,
            hashObject.data(),
            static_cast<ULONG>(hashObject.size()),
            nullptr,
            0,
            0);
        if (!BCRYPT_SUCCESS(status))
        {
            HRESULT hr = ConvertNtStatusToHResult(status);
            errorMessage = FormatHrMessage("BCryptCreateHash failed", hr);
            return hr;
        }

        std::vector<uint8_t> buffer(64 * 1024);
        DWORD bytesRead = 0;
        while (true)
        {
            if (!ReadFile(file.value, buffer.data(), static_cast<DWORD>(buffer.size()), &bytesRead, nullptr))
            {
                const DWORD readError = GetLastError();
                std::ostringstream oss;
                oss << "ReadFile failed for '" << filePath.string() << "' (error=" << readError << ")";
                errorMessage = oss.str();
                return HRESULT_FROM_WIN32(readError);
            }

            if (bytesRead == 0)
            {
                break;
            }

            status = BCryptHashData(hash.value, buffer.data(), bytesRead, 0);
            if (!BCRYPT_SUCCESS(status))
            {
                HRESULT hr = ConvertNtStatusToHResult(status);
                errorMessage = FormatHrMessage("BCryptHashData failed", hr);
                return hr;
            }
        }

        std::vector<uint8_t> hashBuffer(hashLength);
        status = BCryptFinishHash(
            hash.value,
            hashBuffer.data(),
            static_cast<ULONG>(hashBuffer.size()),
            0);
        if (!BCRYPT_SUCCESS(status))
        {
            HRESULT hr = ConvertNtStatusToHResult(status);
            errorMessage = FormatHrMessage("BCryptFinishHash failed", hr);
            return hr;
        }

        static constexpr char hexDigits[] = "0123456789abcdef";
        hexValue.reserve(hashBuffer.size() * 2);
        for (uint8_t byte : hashBuffer)
        {
            hexValue.push_back(hexDigits[(byte >> 4) & 0x0F]);
            hexValue.push_back(hexDigits[byte & 0x0F]);
        }

        return S_OK;
    }
}
