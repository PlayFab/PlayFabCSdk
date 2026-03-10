// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "ApiHelpers.h"
#include "InfoManifest.h"

namespace PlayFab
{
namespace GameSave
{

HRESULT InfoManifestData::ReadInfoManifest(_In_ const String& filePath, _Out_ InfoManifestData& infoManifestData)
{
    Vector<char> fileData;
    RETURN_IF_FAILED(ReadEntireFile(filePath, fileData));

    JsonValue json;
    bool parseError = false;
    String parseErrorMsg;

    try
    {
        String fileString(fileData.data(), fileData.size());
        bool isEmpty = (std::all_of(fileString.begin(), fileString.end(), [](char c) { return c == '\0'; }));
        json = !isEmpty ? JsonValue::parse(fileString) : "";
    }
    catch (const JsonValue::parse_error& e)
    {
        parseErrorMsg = e.what();
        parseError = true;
    }

    if (!parseError && json.contains("Data"))
    {
        auto& dataJson = json["Data"];
        if (dataJson.is_object())
        {
            JsonUtils::ObjectGetMember(dataJson, "DeviceID", infoManifestData.deviceId);
            return S_OK;
        }
        else
        {
            // Corrupted, so just delete
            FilePAL::DeleteLocalFile(filePath);
        }
    }
    else
    {
        // Corrupted, so just delete
        FilePAL::DeleteLocalFile(filePath);
    }

    return E_FAIL;
}

HRESULT InfoManifestData::WriteInfoManifest(_In_ const String& filePath, _Out_ InfoManifestData& infoManifestData)
{
    JsonValue jsonData = JsonValue::object();
    JsonUtils::ObjectAddMember(jsonData, "DeviceID", infoManifestData.deviceId);

    JsonValue jsonObj = JsonValue::object();
    JsonUtils::ObjectAddMember(jsonObj, "Data", std::move(jsonData));

    String str = JsonUtils::WriteToString(jsonObj);
    Vector<char> vData;
    std::copy(str.begin(), str.end(), std::back_inserter(vData));
    HRESULT hr = WriteEntireFile(filePath, vData);
    
    if (FAILED(hr))
    {
        TRACE_ERROR("[GAME SAVE] WriteInfoManifest: WriteEntireFile FAILED hr=0x%08X", hr);
    }
    
    return hr;
}

} // namespace GameSave
} // namespace PlayFab