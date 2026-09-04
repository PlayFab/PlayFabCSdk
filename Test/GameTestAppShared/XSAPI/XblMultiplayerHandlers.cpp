#include "pch.h"
#include "XblMultiplayerHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

static XblMultiplayerSessionHandle s_multiplayerSession = nullptr;
static XblMultiplayerSearchHandle s_searchHandle = nullptr;
static std::vector<uint8_t> s_multiplayerBuffer;
static std::vector<XblMultiplayerSearchHandle> s_searchHandles;

// ── Session Reference ───────────────────────────────────────────────────────

CommandResultPayload HandleXblMultiplayerSessionReferenceCreate(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            std::string error;
            std::string scid, templateName, sessionName;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error))
            {
                const char* configScid = nullptr;
                RETURN_IF_FAILED(XblGetScid(&configScid));
                scid = configScid;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "sessionTemplateName", templateName, error))
            {
                templateName = "GameSession";
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "sessionName", sessionName, error))
            {
                sessionName = "TestSession-1";
            }

            XblMultiplayerSessionReference ref = XblMultiplayerSessionReferenceCreate(
                scid.c_str(), templateName.c_str(), sessionName.c_str());

            payload.result["scid"] = ref.Scid;
            payload.result["sessionTemplateName"] = ref.SessionTemplateName;
            payload.result["sessionName"] = ref.SessionName;
            LogToWindowFormat("XblMultiplayerSessionReferenceCreate");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionReferenceParseFromUriPath(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            std::string error;
            std::string uriPath;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "uriPath", uriPath, error))
            {
                return E_INVALIDARG;
            }

            XblMultiplayerSessionReference ref{};
            const HRESULT hr = XblMultiplayerSessionReferenceParseFromUriPath(uriPath.c_str(), &ref);
            if (FAILED(hr)) return hr;

            payload.result["scid"] = ref.Scid;
            payload.result["sessionTemplateName"] = ref.SessionTemplateName;
            payload.result["sessionName"] = ref.SessionName;
            LogToWindowFormat("XblMultiplayerSessionReferenceParseFromUriPath");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionReferenceToUriPath(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            std::string error;
            std::string scid, templateName, sessionName;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "sessionTemplateName", templateName, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "sessionName", sessionName, error))
            {
                return E_INVALIDARG;
            }

            XblMultiplayerSessionReference ref = XblMultiplayerSessionReferenceCreate(
                scid.c_str(), templateName.c_str(), sessionName.c_str());

            XblMultiplayerSessionReferenceUri uri{};
            const HRESULT hr = XblMultiplayerSessionReferenceToUriPath(&ref, &uri);
            if (SUCCEEDED(hr))
            {
                payload.result["uriPath"] = uri.value;
            }
            LogToWindowFormat("XblMultiplayerSessionReferenceToUriPath (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionReferenceIsValid(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            std::string error;
            std::string scid, templateName, sessionName;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "sessionTemplateName", templateName, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "sessionName", sessionName, error))
            {
                return E_INVALIDARG;
            }

            XblMultiplayerSessionReference ref = XblMultiplayerSessionReferenceCreate(
                scid.c_str(), templateName.c_str(), sessionName.c_str());

            const bool isValid = XblMultiplayerSessionReferenceIsValid(&ref);
            payload.result["isValid"] = isValid;
            LogToWindowFormat("XblMultiplayerSessionReferenceIsValid (isValid=%s)", isValid ? "true" : "false");
            return S_OK;
        });
}

// ── Session Handle Management ───────────────────────────────────────────────

CommandResultPayload HandleXblMultiplayerSessionCreateHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            std::string scid, templateName, sessionName;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error))
            {
                const char* configScid = nullptr;
                RETURN_IF_FAILED(XblGetScid(&configScid));
                scid = configScid;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "sessionTemplateName", templateName, error))
            {
                templateName = "GameSession";
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "sessionName", sessionName, error))
            {
                sessionName = "TestSession-1";
            }

            int64_t xboxUserId = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error);

            int64_t maxMembersInSession = 10;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "maxMembersInSession", maxMembersInSession, error);

            XblMultiplayerSessionReference ref = XblMultiplayerSessionReferenceCreate(
                scid.c_str(), templateName.c_str(), sessionName.c_str());

            if (s_multiplayerSession)
            {
                XblMultiplayerSessionCloseHandle(s_multiplayerSession);
                s_multiplayerSession = nullptr;
            }

            s_multiplayerSession = XblMultiplayerSessionCreateHandle(
                static_cast<uint64_t>(xboxUserId), &ref, nullptr);
            RETURN_HR_IF(E_FAIL, !s_multiplayerSession);
            XblMultiplayerSessionConstantsSetMaxMembersInSession(s_multiplayerSession, static_cast<uint32_t>(maxMembersInSession));
            LogToWindowFormat("XblMultiplayerSessionCreateHandle (success)");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionDuplicateHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            XblMultiplayerSessionHandle duplicated = nullptr;
            const HRESULT hr = XblMultiplayerSessionDuplicateHandle(s_multiplayerSession, &duplicated);
            if (SUCCEEDED(hr))
            {
                XblMultiplayerSessionCloseHandle(s_multiplayerSession);
                s_multiplayerSession = duplicated;
            }
            LogToWindowFormat("XblMultiplayerSessionDuplicateHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionCloseHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (s_multiplayerSession)
            {
                XblMultiplayerSessionCloseHandle(s_multiplayerSession);
                s_multiplayerSession = nullptr;
            }
            LogToWindowFormat("XblMultiplayerSessionCloseHandle");
            return S_OK;
        });
}

// ── Session Members ─────────────────────────────────────────────────────────

CommandResultPayload HandleXblMultiplayerSessionMembers(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            const XblMultiplayerSessionMember* members = nullptr;
            size_t memberCount = 0;
            const HRESULT hr = XblMultiplayerSessionMembers(s_multiplayerSession, &members, &memberCount);
            if (SUCCEEDED(hr))
            {
                payload.result["memberCount"] = memberCount;
            }
            LogToWindowFormat("XblMultiplayerSessionMembers (count=%zu, hr=0x%08X)", memberCount, static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionJoin(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string memberCustomConstantsJson;
            if (parameters.contains("memberCustomConstantsJson"))
            {
                CommandHandlerShared::TryGetStringParameter(parameters, "memberCustomConstantsJson", memberCustomConstantsJson, error);
            }

            bool initializeRequested = false;
            CommandHandlerShared::TryParseBoolParameter(parameters, "initializeRequested", initializeRequested, error);

            bool joinWithActiveStatus = true;
            CommandHandlerShared::TryParseBoolParameter(parameters, "joinWithActiveStatus", joinWithActiveStatus, error);

            const HRESULT hr = XblMultiplayerSessionJoin(
                s_multiplayerSession,
                memberCustomConstantsJson.empty() ? nullptr : memberCustomConstantsJson.c_str(),
                initializeRequested,
                joinWithActiveStatus);
            LogToWindowFormat("XblMultiplayerSessionJoin (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionLeave(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            const HRESULT hr = XblMultiplayerSessionLeave(s_multiplayerSession);
            LogToWindowFormat("XblMultiplayerSessionLeave (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionAddMemberReservation(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            int64_t xuid = 0;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xuid", xuid, error))
            {
                return E_INVALIDARG;
            }

            std::string memberCustomConstantsJson;
            if (parameters.contains("memberCustomConstantsJson"))
            {
                CommandHandlerShared::TryGetStringParameter(parameters, "memberCustomConstantsJson", memberCustomConstantsJson, error);
            }

            bool initializeRequested = false;
            CommandHandlerShared::TryParseBoolParameter(parameters, "initializeRequested", initializeRequested, error);

            const HRESULT hr = XblMultiplayerSessionAddMemberReservation(
                s_multiplayerSession,
                static_cast<uint64_t>(xuid),
                memberCustomConstantsJson.empty() ? nullptr : memberCustomConstantsJson.c_str(),
                initializeRequested);
            LogToWindowFormat("XblMultiplayerSessionAddMemberReservation (xuid=%llu, hr=0x%08X)",
                static_cast<unsigned long long>(xuid), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionCurrentUserSetStatus(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            int64_t status = 0;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "status", status, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerSessionCurrentUserSetStatus(
                s_multiplayerSession,
                static_cast<XblMultiplayerSessionMemberStatus>(status));
            LogToWindowFormat("XblMultiplayerSessionCurrentUserSetStatus (status=%lld, hr=0x%08X)",
                static_cast<long long>(status), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionCurrentUserSetSecureDeviceAddressBase64(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string value;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "value", value, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerSessionCurrentUserSetSecureDeviceAddressBase64(
                s_multiplayerSession, value.c_str());
            LogToWindowFormat("XblMultiplayerSessionCurrentUserSetSecureDeviceAddressBase64 (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionCurrentUserSetRoles(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string roleName, roleValue;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "roleName", roleName, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "roleValue", roleValue, error))
            {
                return E_INVALIDARG;
            }

            XblMultiplayerSessionMemberRole role{};
            role.roleTypeName = roleName.c_str();
            role.roleName = roleValue.c_str();

            const HRESULT hr = XblMultiplayerSessionCurrentUserSetRoles(
                s_multiplayerSession, &role, 1);
            LogToWindowFormat("XblMultiplayerSessionCurrentUserSetRoles (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionCurrentUserSetGroups(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string groupName;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "groupName", groupName, error))
            {
                return E_INVALIDARG;
            }

            const char* groups[] = { groupName.c_str() };
            const HRESULT hr = XblMultiplayerSessionCurrentUserSetGroups(
                s_multiplayerSession, groups, 1);
            LogToWindowFormat("XblMultiplayerSessionCurrentUserSetGroups (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionCurrentUserSetMembersInGroup(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            auto members = CommandHandlerShared::GetUint64Array(parameters, "membersInGroup");
            RETURN_HR_IF(E_INVALIDARG, members.empty());

            std::vector<uint32_t> memberIds;
            memberIds.reserve(members.size());
            for (auto m : members) { memberIds.push_back(static_cast<uint32_t>(m)); }

            const HRESULT hr = XblMultiplayerSessionCurrentUserSetMembersInGroup(
                s_multiplayerSession, memberIds.data(), memberIds.size());
            LogToWindowFormat("XblMultiplayerSessionCurrentUserSetMembersInGroup (count=%zu, hr=0x%08X)",
                memberIds.size(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionCurrentUserSetEncounters(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string encounters;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "encounters", encounters, error))
            {
                return E_INVALIDARG;
            }

            const char* encountersList[] = { encounters.c_str() };
            const HRESULT hr = XblMultiplayerSessionCurrentUserSetEncounters(
                s_multiplayerSession, encountersList, 1);
            LogToWindowFormat("XblMultiplayerSessionCurrentUserSetEncounters (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionCurrentUserSetCustomPropertyJson(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string name, valueJson;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "name", name, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "valueJson", valueJson, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerSessionCurrentUserSetCustomPropertyJson(
                s_multiplayerSession, name.c_str(), valueJson.c_str());
            LogToWindowFormat("XblMultiplayerSessionCurrentUserSetCustomPropertyJson (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionCurrentUserDeleteCustomPropertyJson(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string name;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "name", name, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerSessionCurrentUserDeleteCustomPropertyJson(
                s_multiplayerSession, name.c_str());
            LogToWindowFormat("XblMultiplayerSessionCurrentUserDeleteCustomPropertyJson (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionCurrentUserSetQosMeasurements(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string measurementsJson;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "measurementsJson", measurementsJson, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerSessionCurrentUserSetQosMeasurements(
                s_multiplayerSession, measurementsJson.c_str());
            LogToWindowFormat("XblMultiplayerSessionCurrentUserSetQosMeasurements (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionCurrentUserSetServerQosMeasurements(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string measurementsJson;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "measurementsJson", measurementsJson, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerSessionCurrentUserSetServerQosMeasurements(
                s_multiplayerSession, measurementsJson.c_str());
            LogToWindowFormat("XblMultiplayerSessionCurrentUserSetServerQosMeasurements (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionHostCandidates(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            const XblDeviceToken* hostCandidates = nullptr;
            size_t hostCandidatesCount = 0;
            const HRESULT hr = XblMultiplayerSessionHostCandidates(
                s_multiplayerSession, &hostCandidates, &hostCandidatesCount);
            if (SUCCEEDED(hr))
            {
                payload.result["hostCandidatesCount"] = hostCandidatesCount;
            }
            LogToWindowFormat("XblMultiplayerSessionHostCandidates (count=%zu, hr=0x%08X)",
                hostCandidatesCount, static_cast<uint32_t>(hr));
            return hr;
        });
}

// ── Session Properties ──────────────────────────────────────────────────────

CommandResultPayload HandleXblMultiplayerSessionSetCustomPropertyJson(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string name, valueJson;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "name", name, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "valueJson", valueJson, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerSessionSetCustomPropertyJson(
                s_multiplayerSession, name.c_str(), valueJson.c_str());
            LogToWindowFormat("XblMultiplayerSessionSetCustomPropertyJson (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionDeleteCustomPropertyJson(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string name;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "name", name, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerSessionDeleteCustomPropertyJson(
                s_multiplayerSession, name.c_str());
            LogToWindowFormat("XblMultiplayerSessionDeleteCustomPropertyJson (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionPropertiesSetKeywords(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::vector<std::string> keywordStrings;
            std::vector<const char*> keywordPtrs;
            if (parameters.contains("keywords") && parameters["keywords"].is_array())
            {
                for (const auto& s : parameters["keywords"])
                {
                    if (s.is_string()) keywordStrings.push_back(s.get<std::string>());
                }
                for (const auto& s : keywordStrings) keywordPtrs.push_back(s.c_str());
            }
            RETURN_HR_IF(E_INVALIDARG, keywordPtrs.empty());

            const HRESULT hr = XblMultiplayerSessionPropertiesSetKeywords(
                s_multiplayerSession, keywordPtrs.data(), keywordPtrs.size());
            LogToWindowFormat("XblMultiplayerSessionPropertiesSetKeywords (count=%zu, hr=0x%08X)",
                keywordPtrs.size(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionPropertiesSetTurnCollection(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            auto values = CommandHandlerShared::GetUint64Array(parameters, "turnCollection");
            RETURN_HR_IF(E_INVALIDARG, values.empty());

            std::vector<uint32_t> turnCollection;
            turnCollection.reserve(values.size());
            for (auto v : values) { turnCollection.push_back(static_cast<uint32_t>(v)); }

            const HRESULT hr = XblMultiplayerSessionPropertiesSetTurnCollection(
                s_multiplayerSession, turnCollection.data(), turnCollection.size());
            LogToWindowFormat("XblMultiplayerSessionPropertiesSetTurnCollection (count=%zu, hr=0x%08X)",
                turnCollection.size(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionSetMatchmakingTargetSessionConstantsJson(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string constantsJson;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "constantsJson", constantsJson, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerSessionSetMatchmakingTargetSessionConstantsJson(
                s_multiplayerSession, constantsJson.c_str());
            LogToWindowFormat("XblMultiplayerSessionSetMatchmakingTargetSessionConstantsJson (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionSetRawServersJson(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string serversJson;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "serversJson", serversJson, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerSessionSetRawServersJson(
                s_multiplayerSession, serversJson.c_str());
            LogToWindowFormat("XblMultiplayerSessionSetRawServersJson (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionSetServerConnectionStringCandidates(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::vector<std::string> candidateStrings;
            std::vector<const char*> candidatePtrs;
            if (parameters.contains("candidates") && parameters["candidates"].is_array())
            {
                for (const auto& s : parameters["candidates"])
                {
                    if (s.is_string()) candidateStrings.push_back(s.get<std::string>());
                }
                for (const auto& s : candidateStrings) candidatePtrs.push_back(s.c_str());
            }
            RETURN_HR_IF(E_INVALIDARG, candidatePtrs.empty());

            const HRESULT hr = XblMultiplayerSessionSetServerConnectionStringCandidates(
                s_multiplayerSession, candidatePtrs.data(), candidatePtrs.size());
            LogToWindowFormat("XblMultiplayerSessionSetServerConnectionStringCandidates (count=%zu, hr=0x%08X)",
                candidatePtrs.size(), static_cast<uint32_t>(hr));
            return hr;
        });
}

// ── Session Constants ───────────────────────────────────────────────────────

CommandResultPayload HandleXblMultiplayerSessionConstantsSetTimeouts(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            int64_t reserved{}, inactive{}, ready{}, empty{};
            CommandHandlerShared::TryGetInt64Parameter(parameters, "memberReservedTimeout", reserved, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "memberInactiveTimeout", inactive, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "memberReadyTimeout", ready, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "sessionEmptyTimeout", empty, error);

            const HRESULT hr = XblMultiplayerSessionConstantsSetTimeouts(
                s_multiplayerSession,
                static_cast<uint64_t>(reserved), static_cast<uint64_t>(inactive),
                static_cast<uint64_t>(ready), static_cast<uint64_t>(empty));
            LogToWindowFormat("XblMultiplayerSessionConstantsSetTimeouts (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionConstantsSetArbitrationTimeouts(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            int64_t arbitrationTimeout{}, forfeitTimeout{};
            CommandHandlerShared::TryGetInt64Parameter(parameters, "arbitrationTimeout", arbitrationTimeout, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "forfeitTimeout", forfeitTimeout, error);

            #pragma warning(suppress: 4996) // deprecated
            const HRESULT hr = XblMultiplayerSessionConstantsSetArbitrationTimeouts(
                s_multiplayerSession,
                static_cast<uint64_t>(arbitrationTimeout),
                static_cast<uint64_t>(forfeitTimeout));
            LogToWindowFormat("XblMultiplayerSessionConstantsSetArbitrationTimeouts (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionConstantsSetQosConnectivityMetrics(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            bool enableLatencyMetric = false;
            bool enableBandwidthDownMetric = false;
            bool enableBandwidthUpMetric = false;
            bool enableCustomMetric = false;
            CommandHandlerShared::TryParseBoolParameter(parameters, "enableLatencyMetric", enableLatencyMetric, error);
            CommandHandlerShared::TryParseBoolParameter(parameters, "enableBandwidthDownMetric", enableBandwidthDownMetric, error);
            CommandHandlerShared::TryParseBoolParameter(parameters, "enableBandwidthUpMetric", enableBandwidthUpMetric, error);
            CommandHandlerShared::TryParseBoolParameter(parameters, "enableCustomMetric", enableCustomMetric, error);

            const HRESULT hr = XblMultiplayerSessionConstantsSetQosConnectivityMetrics(
                s_multiplayerSession,
                enableLatencyMetric, enableBandwidthDownMetric,
                enableBandwidthUpMetric, enableCustomMetric);
            LogToWindowFormat("XblMultiplayerSessionConstantsSetQosConnectivityMetrics (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionConstantsSetMemberInitialization(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            int64_t joinTimeout{}, measurementTimeout{}, evaluationTimeout{}, membersNeededToStart{};
            bool externalEvaluation = false;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "joinTimeout", joinTimeout, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "measurementTimeout", measurementTimeout, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "evaluationTimeout", evaluationTimeout, error);
            CommandHandlerShared::TryParseBoolParameter(parameters, "externalEvaluation", externalEvaluation, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "membersNeededToStart", membersNeededToStart, error);

            XblMultiplayerMemberInitialization init{};
            init.JoinTimeout = static_cast<uint64_t>(joinTimeout);
            init.MeasurementTimeout = static_cast<uint64_t>(measurementTimeout);
            init.EvaluationTimeout = static_cast<uint64_t>(evaluationTimeout);
            init.ExternalEvaluation = externalEvaluation;
            init.MembersNeededToStart = static_cast<uint32_t>(membersNeededToStart);

            const HRESULT hr = XblMultiplayerSessionConstantsSetMemberInitialization(
                s_multiplayerSession, init);
            LogToWindowFormat("XblMultiplayerSessionConstantsSetMemberInitialization (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionConstantsSetPeerToPeerRequirements(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            int64_t latencyMaximum{}, bandwidthMinimum{};
            CommandHandlerShared::TryGetInt64Parameter(parameters, "latencyMaximum", latencyMaximum, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "bandwidthMinimumInKilobitsPerSecond", bandwidthMinimum, error);

            XblMultiplayerPeerToPeerRequirements requirements{};
            requirements.LatencyMaximum = static_cast<uint64_t>(latencyMaximum);
            requirements.BandwidthMinimumInKbps = static_cast<uint64_t>(bandwidthMinimum);

            const HRESULT hr = XblMultiplayerSessionConstantsSetPeerToPeerRequirements(
                s_multiplayerSession, requirements);
            LogToWindowFormat("XblMultiplayerSessionConstantsSetPeerToPeerRequirements (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionConstantsSetPeerToHostRequirements(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            int64_t latencyMaximum{}, bandwidthDown{}, bandwidthUp{}, hostSelectionMetric{};
            CommandHandlerShared::TryGetInt64Parameter(parameters, "latencyMaximum", latencyMaximum, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "bandwidthDownMinimumInKilobitsPerSecond", bandwidthDown, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "bandwidthUpMinimumInKilobitsPerSecond", bandwidthUp, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "hostSelectionMetric", hostSelectionMetric, error);

            XblMultiplayerPeerToHostRequirements requirements{};
            requirements.LatencyMaximum = static_cast<uint64_t>(latencyMaximum);
            requirements.BandwidthDownMinimumInKbps = static_cast<uint64_t>(bandwidthDown);
            requirements.BandwidthUpMinimumInKbps = static_cast<uint64_t>(bandwidthUp);
            requirements.HostSelectionMetric = static_cast<XblMultiplayerMetrics>(hostSelectionMetric);

            const HRESULT hr = XblMultiplayerSessionConstantsSetPeerToHostRequirements(
                s_multiplayerSession, requirements);
            LogToWindowFormat("XblMultiplayerSessionConstantsSetPeerToHostRequirements (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionConstantsSetMeasurementServerAddressesJson(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string addressesJson;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "measurementServerAddressesJson", addressesJson, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerSessionConstantsSetMeasurementServerAddressesJson(
                s_multiplayerSession, addressesJson.c_str());
            LogToWindowFormat("XblMultiplayerSessionConstantsSetMeasurementServerAddressesJson (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionConstantsSetCapabilities(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            XblMultiplayerSessionCapabilities caps{};
            CommandHandlerShared::TryParseBoolParameter(parameters, "connectivity", caps.Connectivity, error);
            CommandHandlerShared::TryParseBoolParameter(parameters, "suppressPresenceActivityCheck", caps.SuppressPresenceActivityCheck, error);
            CommandHandlerShared::TryParseBoolParameter(parameters, "gameplay", caps.Gameplay, error);
            CommandHandlerShared::TryParseBoolParameter(parameters, "large", caps.Large, error);
            CommandHandlerShared::TryParseBoolParameter(parameters, "connectionRequiredForActiveMembers", caps.ConnectionRequiredForActiveMembers, error);
            CommandHandlerShared::TryParseBoolParameter(parameters, "userAuthorizationStyle", caps.UserAuthorizationStyle, error);
            CommandHandlerShared::TryParseBoolParameter(parameters, "crossplay", caps.Crossplay, error);
            CommandHandlerShared::TryParseBoolParameter(parameters, "searchable", caps.Searchable, error);
            CommandHandlerShared::TryParseBoolParameter(parameters, "hasOwners", caps.HasOwners, error);

            const HRESULT hr = XblMultiplayerSessionConstantsSetCapabilities(
                s_multiplayerSession, caps);
            LogToWindowFormat("XblMultiplayerSessionConstantsSetCapabilities (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionConstantsSetCloudComputePackageJson(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string packageJson;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "sessionCloudComputePackageConstantsJson", packageJson, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerSessionConstantsSetCloudComputePackageJson(
                s_multiplayerSession, packageJson.c_str());
            LogToWindowFormat("XblMultiplayerSessionConstantsSetCloudComputePackageJson (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        });
}

// ── Session Roles ───────────────────────────────────────────────────────────

CommandResultPayload HandleXblMultiplayerSessionRoleTypes(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            const XblMultiplayerRoleType* roleTypes = nullptr;
            size_t roleTypeCount = 0;
            const HRESULT hr = XblMultiplayerSessionRoleTypes(
                s_multiplayerSession, &roleTypes, &roleTypeCount);
            if (SUCCEEDED(hr))
            {
                payload.result["roleTypeCount"] = roleTypeCount;
            }
            LogToWindowFormat("XblMultiplayerSessionRoleTypes (count=%zu, hr=0x%08X)",
                roleTypeCount, static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionGetRoleByName(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string roleTypeName, roleName;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "roleTypeName", roleTypeName, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "roleName", roleName, error))
            {
                return E_INVALIDARG;
            }

            const XblMultiplayerRole* role = nullptr;
            const HRESULT hr = XblMultiplayerSessionGetRoleByName(
                s_multiplayerSession, roleTypeName.c_str(), roleName.c_str(), &role);
            if (SUCCEEDED(hr) && role)
            {
                payload.result["maxMemberCount"] = role->MaxMemberCount;
                payload.result["targetMemberCount"] = role->TargetCount;
                payload.result["memberCount"] = role->MemberCount;
            }
            LogToWindowFormat("XblMultiplayerSessionGetRoleByName (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSessionSetMutableRoleSettings(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string roleTypeName, roleName;
            int64_t maxMemberCount = 0, targetMemberCount = 0;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "roleTypeName", roleTypeName, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "roleName", roleName, error))
            {
                return E_INVALIDARG;
            }
            CommandHandlerShared::TryGetInt64Parameter(parameters, "maxMemberCount", maxMemberCount, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "targetMemberCount", targetMemberCount, error);

            uint32_t maxCount = static_cast<uint32_t>(maxMemberCount);
            uint32_t targetCount = static_cast<uint32_t>(targetMemberCount);

            const HRESULT hr = XblMultiplayerSessionSetMutableRoleSettings(
                s_multiplayerSession,
                roleTypeName.c_str(), roleName.c_str(),
                &maxCount, &targetCount);
            LogToWindowFormat("XblMultiplayerSessionSetMutableRoleSettings (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

// ── Session Async Operations ────────────────────────────────────────────────

CommandResultPayload HandleXblMultiplayerWriteSessionAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            int64_t writeMode = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "writeMode", writeMode, error);

            HRESULT hr = XblMultiplayerWriteSessionAsync(
                state->xblContext, s_multiplayerSession,
                static_cast<XblMultiplayerSessionWriteMode>(writeMode), &async);
            LogToWindowFormat("XblMultiplayerWriteSessionAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            XblMultiplayerSessionHandle newHandle = nullptr;
            HRESULT hr = XblMultiplayerWriteSessionResult(&async, &newHandle);
            if (SUCCEEDED(hr) && newHandle)
            {
                XblMultiplayerSessionCloseHandle(s_multiplayerSession);
                s_multiplayerSession = newHandle;
            }
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerWriteSessionByHandleAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            std::string handleId;
            int64_t writeMode = 0;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "handleId", handleId, error))
            {
                return E_INVALIDARG;
            }
            CommandHandlerShared::TryGetInt64Parameter(parameters, "writeMode", writeMode, error);

            HRESULT hr = XblMultiplayerWriteSessionByHandleAsync(
                state->xblContext, s_multiplayerSession,
                static_cast<XblMultiplayerSessionWriteMode>(writeMode),
                handleId.c_str(), &async);
            LogToWindowFormat("XblMultiplayerWriteSessionByHandleAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            XblMultiplayerSessionHandle newHandle = nullptr;
            HRESULT hr = XblMultiplayerWriteSessionResult(&async, &newHandle);
            if (SUCCEEDED(hr) && newHandle)
            {
                XblMultiplayerSessionCloseHandle(s_multiplayerSession);
                s_multiplayerSession = newHandle;
            }
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerGetSessionAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string scid, templateName, sessionName;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "sessionTemplateName", templateName, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "sessionName", sessionName, error))
            {
                return E_INVALIDARG;
            }

            XblMultiplayerSessionReference ref = XblMultiplayerSessionReferenceCreate(
                scid.c_str(), templateName.c_str(), sessionName.c_str());

            HRESULT hr = XblMultiplayerGetSessionAsync(state->xblContext, &ref, &async);
            LogToWindowFormat("XblMultiplayerGetSessionAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            XblMultiplayerSessionHandle newHandle = nullptr;
            HRESULT hr = XblMultiplayerGetSessionResult(&async, &newHandle);
            if (SUCCEEDED(hr) && newHandle)
            {
                if (s_multiplayerSession) { XblMultiplayerSessionCloseHandle(s_multiplayerSession); }
                s_multiplayerSession = newHandle;
            }
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerGetSessionByHandleAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string handleId;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "handleId", handleId, error))
            {
                return E_INVALIDARG;
            }

            HRESULT hr = XblMultiplayerGetSessionByHandleAsync(state->xblContext, handleId.c_str(), &async);
            LogToWindowFormat("XblMultiplayerGetSessionByHandleAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            XblMultiplayerSessionHandle newHandle = nullptr;
            HRESULT hr = XblMultiplayerGetSessionResult(&async, &newHandle);
            if (SUCCEEDED(hr) && newHandle)
            {
                if (s_multiplayerSession) { XblMultiplayerSessionCloseHandle(s_multiplayerSession); }
                s_multiplayerSession = newHandle;
            }
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSetActivityAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string scid, templateName, sessionName;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "sessionTemplateName", templateName, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "sessionName", sessionName, error))
            {
                return E_INVALIDARG;
            }

            XblMultiplayerSessionReference ref = XblMultiplayerSessionReferenceCreate(
                scid.c_str(), templateName.c_str(), sessionName.c_str());

            HRESULT hr = XblMultiplayerSetActivityAsync(state->xblContext, &ref, &async);
            LogToWindowFormat("XblMultiplayerSetActivityAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerClearActivityAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string scid;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error))
            {
                return E_INVALIDARG;
            }

            HRESULT hr = XblMultiplayerClearActivityAsync(state->xblContext, scid.c_str(), &async);
            LogToWindowFormat("XblMultiplayerClearActivityAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSendInvitesAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string scid, templateName, sessionName;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "sessionTemplateName", templateName, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "sessionName", sessionName, error))
            {
                return E_INVALIDARG;
            }

            auto xuids = CommandHandlerShared::GetUint64Array(parameters, "xuids");
            RETURN_HR_IF(E_INVALIDARG, xuids.empty());

            XblMultiplayerSessionReference ref = XblMultiplayerSessionReferenceCreate(
                scid.c_str(), templateName.c_str(), sessionName.c_str());

            int64_t titleId = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "titleId", titleId, error);

            std::string contextString, customActivationContext;
            CommandHandlerShared::TryGetStringParameter(parameters, "contextString", contextString, error);
            CommandHandlerShared::TryGetStringParameter(parameters, "customActivationContext", customActivationContext, error);

            HRESULT hr = XblMultiplayerSendInvitesAsync(
                state->xblContext, &ref,
                xuids.data(), xuids.size(),
                static_cast<uint32_t>(titleId),
                contextString.empty() ? nullptr : contextString.c_str(),
                customActivationContext.empty() ? nullptr : customActivationContext.c_str(),
                &async);
            LogToWindowFormat("XblMultiplayerSendInvitesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSetTransferHandleAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string targetScid, targetTemplate, targetSession;
            std::string originScid, originTemplate, originSession;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "targetScid", targetScid, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "targetSessionTemplateName", targetTemplate, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "targetSessionName", targetSession, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "originScid", originScid, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "originSessionTemplateName", originTemplate, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "originSessionName", originSession, error))
            {
                return E_INVALIDARG;
            }

            XblMultiplayerSessionReference targetRef = XblMultiplayerSessionReferenceCreate(
                targetScid.c_str(), targetTemplate.c_str(), targetSession.c_str());
            XblMultiplayerSessionReference originRef = XblMultiplayerSessionReferenceCreate(
                originScid.c_str(), originTemplate.c_str(), originSession.c_str());

            HRESULT hr = XblMultiplayerSetTransferHandleAsync(
                state->xblContext, targetRef, originRef, &async);
            LogToWindowFormat("XblMultiplayerSetTransferHandleAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            XblMultiplayerSessionHandleId transferHandle{};
            HRESULT hr = XblMultiplayerSetTransferHandleResult(&async, &transferHandle);
            if (SUCCEEDED(hr))
            {
                payload.result["transferHandle"] = transferHandle.value;
            }
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerQuerySessionsAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string scid;
            int64_t maxItems = 0;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error) ||
                !CommandHandlerShared::TryGetInt64Parameter(parameters, "maxItems", maxItems, error))
            {
                return E_INVALIDARG;
            }

            XblMultiplayerSessionQuery query{};
            strcpy_s(query.Scid, scid.c_str());
            query.MaxItems = static_cast<uint32_t>(maxItems);

            std::string keyword, templateName, xuidKeyword;
            CommandHandlerShared::TryGetStringParameter(parameters, "sessionTemplateNameFilter", templateName, error);
            CommandHandlerShared::TryGetStringParameter(parameters, "xuidKeywordFilter", xuidKeyword, error);
            CommandHandlerShared::TryGetStringParameter(parameters, "keywordFilter", keyword, error);
            if (!templateName.empty()) { strcpy_s(query.SessionTemplateNameFilter, templateName.c_str()); }
            if (!keyword.empty()) { query.KeywordFilter = keyword.c_str(); }

            int64_t visibilityFilter = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "visibilityFilter", visibilityFilter, error);
            query.VisibilityFilter = static_cast<XblMultiplayerSessionVisibility>(visibilityFilter);

            int64_t contractFilter = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "contractVersionFilter", contractFilter, error);
            query.ContractVersionFilter = static_cast<uint32_t>(contractFilter);

            HRESULT hr = XblMultiplayerQuerySessionsAsync(state->xblContext, &query, &async);
            LogToWindowFormat("XblMultiplayerQuerySessionsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultCount = 0;
            HRESULT hr = XblMultiplayerQuerySessionsResultCount(&async, &resultCount);
            if (SUCCEEDED(hr))
            {
                payload.result["sessionCount"] = resultCount;
            }
            return hr;
        });
}

// ── Session Subscriptions ───────────────────────────────────────────────────

CommandResultPayload HandleXblMultiplayerSessionSetSessionChangeSubscription(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            std::string error;
            int64_t types = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "types", types, error);

            const HRESULT hr = XblMultiplayerSessionSetSessionChangeSubscription(
                s_multiplayerSession,
                static_cast<XblMultiplayerSessionChangeTypes>(types));
            LogToWindowFormat("XblMultiplayerSessionSetSessionChangeSubscription (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSetSubscriptionsEnabled(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            bool enabled = false;
            if (!CommandHandlerShared::TryParseBoolParameter(parameters, "enabled", enabled, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerSetSubscriptionsEnabled(state->xblContext, enabled);
            LogToWindowFormat("XblMultiplayerSetSubscriptionsEnabled (enabled=%s, hr=0x%08X)",
                enabled ? "true" : "false", static_cast<uint32_t>(hr));
            return hr;
        });
}

// ── Activities ──────────────────────────────────────────────────────────────

CommandResultPayload HandleXblMultiplayerGetActivitiesForSocialGroupAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string scid, socialGroupName;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "socialGroupName", socialGroupName, error))
            {
                return E_INVALIDARG;
            }

            int64_t socialGroupOwnerXuid = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "socialGroupOwnerXuid", socialGroupOwnerXuid, error);

            HRESULT hr = XblMultiplayerGetActivitiesForSocialGroupAsync(
                state->xblContext, scid.c_str(),
                static_cast<uint64_t>(socialGroupOwnerXuid),
                socialGroupName.c_str(), &async);
            LogToWindowFormat("XblMultiplayerGetActivitiesForSocialGroupAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultCount = 0;
            HRESULT hr = XblMultiplayerGetActivitiesForSocialGroupResultCount(&async, &resultCount);
            if (SUCCEEDED(hr))
            {
                payload.result["resultCount"] = resultCount;
            }
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerGetActivitiesForUsersAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string scid;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error))
            {
                return E_INVALIDARG;
            }

            auto xuids = CommandHandlerShared::GetUint64Array(parameters, "xuids");
            RETURN_HR_IF(E_INVALIDARG, xuids.empty());

            HRESULT hr = XblMultiplayerGetActivitiesForUsersAsync(
                state->xblContext, scid.c_str(), xuids.data(), xuids.size(), &async);
            LogToWindowFormat("XblMultiplayerGetActivitiesForUsersAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultCount = 0;
            HRESULT hr = XblMultiplayerGetActivitiesForUsersResultCount(&async, &resultCount);
            if (SUCCEEDED(hr))
            {
                payload.result["resultCount"] = resultCount;
            }
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerGetActivitiesWithPropertiesForSocialGroupAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string scid, socialGroupName;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "socialGroupName", socialGroupName, error))
            {
                return E_INVALIDARG;
            }

            int64_t socialGroupOwnerXuid = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "socialGroupOwnerXuid", socialGroupOwnerXuid, error);

            HRESULT hr = XblMultiplayerGetActivitiesWithPropertiesForSocialGroupAsync(
                state->xblContext, scid.c_str(),
                static_cast<uint64_t>(socialGroupOwnerXuid),
                socialGroupName.c_str(), &async);
            LogToWindowFormat("XblMultiplayerGetActivitiesWithPropertiesForSocialGroupAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultSize = 0;
            HRESULT hr = XblMultiplayerGetActivitiesWithPropertiesForSocialGroupResultSize(&async, &resultSize);
            if (SUCCEEDED(hr))
            {
                payload.result["resultSize"] = resultSize;
            }
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerGetActivitiesWithPropertiesForUsersAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string scid;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error))
            {
                return E_INVALIDARG;
            }

            auto xuids = CommandHandlerShared::GetUint64Array(parameters, "xuids");
            RETURN_HR_IF(E_INVALIDARG, xuids.empty());

            HRESULT hr = XblMultiplayerGetActivitiesWithPropertiesForUsersAsync(
                state->xblContext, scid.c_str(), xuids.data(), xuids.size(), &async);
            LogToWindowFormat("XblMultiplayerGetActivitiesWithPropertiesForUsersAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultSize = 0;
            HRESULT hr = XblMultiplayerGetActivitiesWithPropertiesForUsersResultSize(&async, &resultSize);
            if (SUCCEEDED(hr))
            {
                payload.result["resultSize"] = resultSize;
            }
            return hr;
        });
}

// ── Search Handles ──────────────────────────────────────────────────────────

CommandResultPayload HandleXblMultiplayerCreateSearchHandleAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);

            const XblMultiplayerSessionReference* ref = XblMultiplayerSessionSessionReference(s_multiplayerSession);
            RETURN_HR_IF(E_FAIL, !ref);

            HRESULT hr = XblMultiplayerCreateSearchHandleAsync(state->xblContext, ref, nullptr, 0,
                nullptr, 0, nullptr, 0, &async);
            LogToWindowFormat("XblMultiplayerCreateSearchHandleAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            if (s_searchHandle) { XblMultiplayerSearchHandleCloseHandle(s_searchHandle); s_searchHandle = nullptr; }
            HRESULT hr = XblMultiplayerCreateSearchHandleResult(&async, &s_searchHandle);
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerDeleteSearchHandleAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string handleId;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "handleId", handleId, error))
            {
                return E_INVALIDARG;
            }

            HRESULT hr = XblMultiplayerDeleteSearchHandleAsync(state->xblContext, handleId.c_str(), &async);
            LogToWindowFormat("XblMultiplayerDeleteSearchHandleAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerGetSearchHandlesAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string scid, templateName;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "sessionTemplateName", templateName, error))
            {
                return E_INVALIDARG;
            }

            std::string orderBy, searchFilter;
            CommandHandlerShared::TryGetStringParameter(parameters, "orderBy", orderBy, error);
            CommandHandlerShared::TryGetStringParameter(parameters, "searchFilter", searchFilter, error);

            HRESULT hr = XblMultiplayerGetSearchHandlesAsync(
                state->xblContext, scid.c_str(), templateName.c_str(),
                orderBy.empty() ? nullptr : orderBy.c_str(),
                true, searchFilter.empty() ? nullptr : searchFilter.c_str(),
                nullptr, &async);
            LogToWindowFormat("XblMultiplayerGetSearchHandlesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultCount = 0;
            HRESULT hr = XblMultiplayerGetSearchHandlesResultCount(&async, &resultCount);
            if (SUCCEEDED(hr))
            {
                for (auto& h : s_searchHandles) { XblMultiplayerSearchHandleCloseHandle(h); }
                s_searchHandles.resize(resultCount);
                if (resultCount > 0)
                {
                    hr = XblMultiplayerGetSearchHandlesResult(&async, s_searchHandles.data(), resultCount);
                }
                payload.result["resultCount"] = resultCount;
            }
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSearchHandleGetId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblMultiplayerSearchHandle handle = s_searchHandle ? s_searchHandle :
                (!s_searchHandles.empty() ? s_searchHandles[0] : nullptr);
            RETURN_HR_IF(E_POINTER, !handle);

            const char* id = nullptr;
            const HRESULT hr = XblMultiplayerSearchHandleGetId(handle, &id);
            if (SUCCEEDED(hr) && id) { payload.result["id"] = id; }
            LogToWindowFormat("XblMultiplayerSearchHandleGetId (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSearchHandleGetSessionReference(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblMultiplayerSearchHandle handle = s_searchHandle ? s_searchHandle :
                (!s_searchHandles.empty() ? s_searchHandles[0] : nullptr);
            RETURN_HR_IF(E_POINTER, !handle);

            XblMultiplayerSessionReference ref{};
            const HRESULT hr = XblMultiplayerSearchHandleGetSessionReference(handle, &ref);
            if (SUCCEEDED(hr))
            {
                payload.result["scid"] = ref.Scid;
                payload.result["sessionTemplateName"] = ref.SessionTemplateName;
                payload.result["sessionName"] = ref.SessionName;
            }
            LogToWindowFormat("XblMultiplayerSearchHandleGetSessionReference (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSearchHandleGetSessionOwnerXuids(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblMultiplayerSearchHandle handle = s_searchHandle ? s_searchHandle :
                (!s_searchHandles.empty() ? s_searchHandles[0] : nullptr);
            RETURN_HR_IF(E_POINTER, !handle);

            const uint64_t* xuids = nullptr;
            size_t count = 0;
            const HRESULT hr = XblMultiplayerSearchHandleGetSessionOwnerXuids(handle, &xuids, &count);
            if (SUCCEEDED(hr)) { payload.result["count"] = count; }
            LogToWindowFormat("XblMultiplayerSearchHandleGetSessionOwnerXuids (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSearchHandleGetTags(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblMultiplayerSearchHandle handle = s_searchHandle ? s_searchHandle :
                (!s_searchHandles.empty() ? s_searchHandles[0] : nullptr);
            RETURN_HR_IF(E_POINTER, !handle);

            const XblMultiplayerSessionTag* tags = nullptr;
            size_t count = 0;
            const HRESULT hr = XblMultiplayerSearchHandleGetTags(handle, &tags, &count);
            if (SUCCEEDED(hr)) { payload.result["count"] = count; }
            LogToWindowFormat("XblMultiplayerSearchHandleGetTags (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSearchHandleGetStringAttributes(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblMultiplayerSearchHandle handle = s_searchHandle ? s_searchHandle :
                (!s_searchHandles.empty() ? s_searchHandles[0] : nullptr);
            RETURN_HR_IF(E_POINTER, !handle);

            const XblMultiplayerSessionStringAttribute* attrs = nullptr;
            size_t count = 0;
            const HRESULT hr = XblMultiplayerSearchHandleGetStringAttributes(handle, &attrs, &count);
            if (SUCCEEDED(hr)) { payload.result["count"] = count; }
            LogToWindowFormat("XblMultiplayerSearchHandleGetStringAttributes (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSearchHandleGetNumberAttributes(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblMultiplayerSearchHandle handle = s_searchHandle ? s_searchHandle :
                (!s_searchHandles.empty() ? s_searchHandles[0] : nullptr);
            RETURN_HR_IF(E_POINTER, !handle);

            const XblMultiplayerSessionNumberAttribute* attrs = nullptr;
            size_t count = 0;
            const HRESULT hr = XblMultiplayerSearchHandleGetNumberAttributes(handle, &attrs, &count);
            if (SUCCEEDED(hr)) { payload.result["count"] = count; }
            LogToWindowFormat("XblMultiplayerSearchHandleGetNumberAttributes (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSearchHandleGetVisibility(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblMultiplayerSearchHandle handle = s_searchHandle ? s_searchHandle :
                (!s_searchHandles.empty() ? s_searchHandles[0] : nullptr);
            RETURN_HR_IF(E_POINTER, !handle);

            XblMultiplayerSessionVisibility visibility{};
            const HRESULT hr = XblMultiplayerSearchHandleGetVisibility(handle, &visibility);
            if (SUCCEEDED(hr)) { payload.result["visibility"] = static_cast<int>(visibility); }
            LogToWindowFormat("XblMultiplayerSearchHandleGetVisibility (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSearchHandleGetJoinRestriction(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblMultiplayerSearchHandle handle = s_searchHandle ? s_searchHandle :
                (!s_searchHandles.empty() ? s_searchHandles[0] : nullptr);
            RETURN_HR_IF(E_POINTER, !handle);

            XblMultiplayerSessionRestriction restriction{};
            const HRESULT hr = XblMultiplayerSearchHandleGetJoinRestriction(handle, &restriction);
            if (SUCCEEDED(hr)) { payload.result["joinRestriction"] = static_cast<int>(restriction); }
            LogToWindowFormat("XblMultiplayerSearchHandleGetJoinRestriction (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSearchHandleGetSessionClosed(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblMultiplayerSearchHandle handle = s_searchHandle ? s_searchHandle :
                (!s_searchHandles.empty() ? s_searchHandles[0] : nullptr);
            RETURN_HR_IF(E_POINTER, !handle);

            bool closed = false;
            const HRESULT hr = XblMultiplayerSearchHandleGetSessionClosed(handle, &closed);
            if (SUCCEEDED(hr)) { payload.result["closed"] = closed; }
            LogToWindowFormat("XblMultiplayerSearchHandleGetSessionClosed (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSearchHandleGetMemberCounts(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblMultiplayerSearchHandle handle = s_searchHandle ? s_searchHandle :
                (!s_searchHandles.empty() ? s_searchHandles[0] : nullptr);
            RETURN_HR_IF(E_POINTER, !handle);

            size_t currentMembers = 0, maxMembers = 0;
            const HRESULT hr = XblMultiplayerSearchHandleGetMemberCounts(handle, &maxMembers, &currentMembers);
            if (SUCCEEDED(hr))
            {
                payload.result["currentMembers"] = currentMembers;
                payload.result["maxMembers"] = maxMembers;
            }
            LogToWindowFormat("XblMultiplayerSearchHandleGetMemberCounts (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSearchHandleGetCreationTime(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblMultiplayerSearchHandle handle = s_searchHandle ? s_searchHandle :
                (!s_searchHandles.empty() ? s_searchHandles[0] : nullptr);
            RETURN_HR_IF(E_POINTER, !handle);

            time_t creationTime = 0;
            const HRESULT hr = XblMultiplayerSearchHandleGetCreationTime(handle, &creationTime);
            if (SUCCEEDED(hr)) { payload.result["creationTime"] = static_cast<int64_t>(creationTime); }
            LogToWindowFormat("XblMultiplayerSearchHandleGetCreationTime (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSearchHandleGetCustomSessionPropertiesJson(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblMultiplayerSearchHandle handle = s_searchHandle ? s_searchHandle :
                (!s_searchHandles.empty() ? s_searchHandles[0] : nullptr);
            RETURN_HR_IF(E_POINTER, !handle);

            const char* json = nullptr;
            const HRESULT hr = XblMultiplayerSearchHandleGetCustomSessionPropertiesJson(handle, &json);
            if (SUCCEEDED(hr) && json) { payload.result["customPropertiesJson"] = json; }
            LogToWindowFormat("XblMultiplayerSearchHandleGetCustomSessionPropertiesJson (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerSearchHandleDuplicateHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XblMultiplayerSearchHandle handle = s_searchHandle ? s_searchHandle :
                (!s_searchHandles.empty() ? s_searchHandles[0] : nullptr);
            RETURN_HR_IF(E_POINTER, !handle);

            XblMultiplayerSearchHandle duplicate = nullptr;
            const HRESULT hr = XblMultiplayerSearchHandleDuplicateHandle(handle, &duplicate);
            if (SUCCEEDED(hr) && duplicate)
            {
                if (s_searchHandle) { XblMultiplayerSearchHandleCloseHandle(s_searchHandle); }
                s_searchHandle = duplicate;
            }
            LogToWindowFormat("XblMultiplayerSearchHandleDuplicateHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

// ── Misc ────────────────────────────────────────────────────────────────────

CommandResultPayload HandleXblFormatSecureDeviceAddress(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            std::string error;
            std::string deviceAddress;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "deviceAddress", deviceAddress, error))
            {
                return E_INVALIDARG;
            }

            XblFormattedSecureDeviceAddress formatted{};
            const HRESULT hr = XblFormatSecureDeviceAddress(deviceAddress.c_str(), &formatted);
            if (SUCCEEDED(hr)) { payload.result["formattedAddress"] = formatted.value; }
            LogToWindowFormat("XblFormatSecureDeviceAddress (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

// ── Multiplayer Manager ─────────────────────────────────────────────────────

CommandResultPayload HandleXblMultiplayerManagerInitialize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            std::string lobbySessionTemplateName;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "lobbySessionTemplateName", lobbySessionTemplateName, error))
            {
                lobbySessionTemplateName = "LobbySession";
            }

            const HRESULT hr = XblMultiplayerManagerInitialize(lobbySessionTemplateName.c_str(), state->taskQueue);
            LogToWindowFormat("XblMultiplayerManagerInitialize (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerDoWork(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const XblMultiplayerEvent* events = nullptr;
            size_t eventsCount = 0;
            const HRESULT hr = XblMultiplayerManagerDoWork(&events, &eventsCount);
            if (SUCCEEDED(hr)) { payload.result["eventsCount"] = eventsCount; }
            LogToWindowFormat("XblMultiplayerManagerDoWork (eventsCount=%zu, hr=0x%08X)",
                eventsCount, static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerJoinLobby(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            std::string handleId;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "handleId", handleId, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerManagerJoinLobby(handleId.c_str(), state->xuser);
            LogToWindowFormat("XblMultiplayerManagerJoinLobby (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerJoinGameFromLobby(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            std::string sessionTemplateName;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "sessionTemplateName", sessionTemplateName, error))
            {
                sessionTemplateName = "GameSession";
            }

            const HRESULT hr = XblMultiplayerManagerJoinGameFromLobby(sessionTemplateName.c_str());
            LogToWindowFormat("XblMultiplayerManagerJoinGameFromLobby (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerJoinGame(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            std::string sessionName, sessionTemplateName;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "sessionName", sessionName, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "sessionTemplateName", sessionTemplateName, error))
            {
                return E_INVALIDARG;
            }

            auto xuids = CommandHandlerShared::GetUint64Array(parameters, "xuids");

            const HRESULT hr = XblMultiplayerManagerJoinGame(
                sessionName.c_str(), sessionTemplateName.c_str(),
                xuids.empty() ? nullptr : xuids.data(),
                xuids.size());
            LogToWindowFormat("XblMultiplayerManagerJoinGame (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLeaveGame(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XblMultiplayerManagerLeaveGame();
            LogToWindowFormat("XblMultiplayerManagerLeaveGame (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerFindMatch(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            std::string hopperName;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "hopperName", hopperName, error))
            {
                hopperName = "TestHopper";
            }

            std::string attributesJson;
            CommandHandlerShared::TryGetStringParameter(parameters, "attributesJson", attributesJson, error);

            int64_t timeout = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "timeout", timeout, error);

            const HRESULT hr = XblMultiplayerManagerFindMatch(
                hopperName.c_str(),
                attributesJson.empty() ? nullptr : attributesJson.c_str(),
                static_cast<uint32_t>(timeout));
            LogToWindowFormat("XblMultiplayerManagerFindMatch (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerSetJoinability(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            int64_t joinability = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "joinability", joinability, error);

            const HRESULT hr = XblMultiplayerManagerSetJoinability(
                static_cast<XblMultiplayerJoinability>(joinability), nullptr);
            LogToWindowFormat("XblMultiplayerManagerSetJoinability (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerSetQosMeasurements(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            std::string measurementsJson;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "measurementsJson", measurementsJson, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerManagerSetQosMeasurements(measurementsJson.c_str());
            LogToWindowFormat("XblMultiplayerManagerSetQosMeasurements (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionAddLocalUser(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            const HRESULT hr = XblMultiplayerManagerLobbySessionAddLocalUser(state->xuser);
            LogToWindowFormat("XblMultiplayerManagerLobbySessionAddLocalUser (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionRemoveLocalUser(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            const HRESULT hr = XblMultiplayerManagerLobbySessionRemoveLocalUser(state->xuser);
            LogToWindowFormat("XblMultiplayerManagerLobbySessionRemoveLocalUser (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionSetLocalMemberProperties(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            std::string error;
            std::string name, valueJson;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "name", name, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "valueJson", valueJson, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerManagerLobbySessionSetLocalMemberProperties(
                state->xuser, name.c_str(), valueJson.c_str(), nullptr);
            LogToWindowFormat("XblMultiplayerManagerLobbySessionSetLocalMemberProperties (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionDeleteLocalMemberProperties(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            std::string error;
            std::string name;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "name", name, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerManagerLobbySessionDeleteLocalMemberProperties(
                state->xuser, name.c_str(), nullptr);
            LogToWindowFormat("XblMultiplayerManagerLobbySessionDeleteLocalMemberProperties (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionSetLocalMemberConnectionAddress(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            std::string error;
            std::string address;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "address", address, error))
            {
                address = "TestConnectionAddress";
            }

            const HRESULT hr = XblMultiplayerManagerLobbySessionSetLocalMemberConnectionAddress(
                state->xuser, address.c_str(), nullptr);
            LogToWindowFormat("XblMultiplayerManagerLobbySessionSetLocalMemberConnectionAddress (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionSetProperties(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            std::string name, valueJson;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "name", name, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "valueJson", valueJson, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerManagerLobbySessionSetProperties(
                name.c_str(), valueJson.c_str(), nullptr);
            LogToWindowFormat("XblMultiplayerManagerLobbySessionSetProperties (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionSetSynchronizedProperties(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            std::string name, valueJson;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "name", name, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "valueJson", valueJson, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerManagerLobbySessionSetSynchronizedProperties(
                name.c_str(), valueJson.c_str(), nullptr);
            LogToWindowFormat("XblMultiplayerManagerLobbySessionSetSynchronizedProperties (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionSetSynchronizedHost(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            std::string deviceToken;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "deviceToken", deviceToken, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerManagerLobbySessionSetSynchronizedHost(deviceToken.c_str(), nullptr);
            LogToWindowFormat("XblMultiplayerManagerLobbySessionSetSynchronizedHost (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionInviteFriends(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            std::string error;
            std::string contextString, customActivationContext;
            CommandHandlerShared::TryGetStringParameter(parameters, "contextString", contextString, error);
            CommandHandlerShared::TryGetStringParameter(parameters, "customActivationContext", customActivationContext, error);

            const HRESULT hr = XblMultiplayerManagerLobbySessionInviteFriends(
                state->xuser,
                contextString.empty() ? nullptr : contextString.c_str(),
                customActivationContext.empty() ? nullptr : customActivationContext.c_str());
            LogToWindowFormat("XblMultiplayerManagerLobbySessionInviteFriends (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionInviteUsers(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            auto xuids = CommandHandlerShared::GetUint64Array(parameters, "xuids");
            RETURN_HR_IF(E_INVALIDARG, xuids.empty());

            const HRESULT hr = XblMultiplayerManagerLobbySessionInviteUsers(
                state->xuser, xuids.data(), xuids.size(), nullptr, nullptr);
            LogToWindowFormat("XblMultiplayerManagerLobbySessionInviteUsers (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionHost(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblMultiplayerManagerMember hostMember{};
            const HRESULT hr = XblMultiplayerManagerLobbySessionHost(&hostMember);
            if (SUCCEEDED(hr))
            {
                payload.result["xuid"] = std::to_string(hostMember.Xuid);
                payload.result["isLocal"] = hostMember.IsLocal;
            }
            else
            {
                payload.result["host"] = nullptr;
            }
            LogToWindowFormat("XblMultiplayerManagerLobbySessionHost (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionMembers(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const size_t maxCount = 100;
            std::vector<XblMultiplayerManagerMember> members(maxCount);
            const HRESULT hr = XblMultiplayerManagerLobbySessionMembers(maxCount, members.data());
            size_t count = SUCCEEDED(hr) ? maxCount : 0;
            if (SUCCEEDED(hr)) { payload.result["count"] = count; }
            LogToWindowFormat("XblMultiplayerManagerLobbySessionMembers (count=%zu, hr=0x%08X)",
                count, static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionLocalMembers(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const size_t maxCount = 16;
            std::vector<XblMultiplayerManagerMember> members(maxCount);
            const HRESULT hr = XblMultiplayerManagerLobbySessionLocalMembers(maxCount, members.data());
            size_t count = SUCCEEDED(hr) ? maxCount : 0;
            if (SUCCEEDED(hr)) { payload.result["count"] = count; }
            LogToWindowFormat("XblMultiplayerManagerLobbySessionLocalMembers (count=%zu, hr=0x%08X)",
                count, static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionCorrelationId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblGuid correlationId{};
            const HRESULT hr = XblMultiplayerManagerLobbySessionCorrelationId(&correlationId);
            if (SUCCEEDED(hr)) { payload.result["correlationId"] = correlationId.value; }
            LogToWindowFormat("XblMultiplayerManagerLobbySessionCorrelationId (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionSessionReference(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblMultiplayerSessionReference ref{};
            const HRESULT hr = XblMultiplayerManagerLobbySessionSessionReference(&ref);
            if (FAILED(hr)) return hr;

            payload.result["scid"] = ref.Scid;
            payload.result["sessionTemplateName"] = ref.SessionTemplateName;
            payload.result["sessionName"] = ref.SessionName;
            LogToWindowFormat("XblMultiplayerManagerLobbySessionSessionReference (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerGameSessionHost(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblMultiplayerManagerMember hostMember{};
            const HRESULT hr = XblMultiplayerManagerGameSessionHost(&hostMember);
            if (SUCCEEDED(hr))
            {
                payload.result["xuid"] = std::to_string(hostMember.Xuid);
                payload.result["isLocal"] = hostMember.IsLocal;
            }
            else
            {
                payload.result["host"] = nullptr;
            }
            LogToWindowFormat("XblMultiplayerManagerGameSessionHost (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerGameSessionMembers(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const size_t maxCount = 100;
            std::vector<XblMultiplayerManagerMember> members(maxCount);
            const HRESULT hr = XblMultiplayerManagerGameSessionMembers(maxCount, members.data());
            size_t count = SUCCEEDED(hr) ? maxCount : 0;
            if (SUCCEEDED(hr)) { payload.result["count"] = count; }
            LogToWindowFormat("XblMultiplayerManagerGameSessionMembers (count=%zu, hr=0x%08X)",
                count, static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerGameSessionSetProperties(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            std::string name, valueJson;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "name", name, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "valueJson", valueJson, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerManagerGameSessionSetProperties(
                name.c_str(), valueJson.c_str(), nullptr);
            LogToWindowFormat("XblMultiplayerManagerGameSessionSetProperties (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerGameSessionSetSynchronizedProperties(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            std::string name, valueJson;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "name", name, error) ||
                !CommandHandlerShared::TryGetStringParameter(parameters, "valueJson", valueJson, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerManagerGameSessionSetSynchronizedProperties(
                name.c_str(), valueJson.c_str(), nullptr);
            LogToWindowFormat("XblMultiplayerManagerGameSessionSetSynchronizedProperties (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerManagerGameSessionSetSynchronizedHost(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            std::string deviceToken;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "deviceToken", deviceToken, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblMultiplayerManagerGameSessionSetSynchronizedHost(deviceToken.c_str(), nullptr);
            LogToWindowFormat("XblMultiplayerManagerGameSessionSetSynchronizedHost (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

// ── Event Args ──────────────────────────────────────────────────────────────

CommandResultPayload HandleXblMultiplayerEventArgsXuid(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XblMultiplayerEventArgsHandle argsHandle = nullptr;
            uint64_t xuid{};
            const HRESULT hr = XblMultiplayerEventArgsXuid(argsHandle, &xuid);
            LogToWindowFormat("XblMultiplayerEventArgsXuid (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerEventArgsMembersCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XblMultiplayerEventArgsHandle argsHandle = nullptr;
            size_t memberCount{};
            const HRESULT hr = XblMultiplayerEventArgsMembersCount(argsHandle, &memberCount);
            LogToWindowFormat("XblMultiplayerEventArgsMembersCount (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerEventArgsMembers(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XblMultiplayerEventArgsHandle argsHandle = nullptr;
            XblMultiplayerManagerMember member{};
            const HRESULT hr = XblMultiplayerEventArgsMembers(argsHandle, 1, &member);
            LogToWindowFormat("XblMultiplayerEventArgsMembers (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerEventArgsMember(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XblMultiplayerEventArgsHandle argsHandle = nullptr;
            XblMultiplayerManagerMember member{};
            const HRESULT hr = XblMultiplayerEventArgsMember(argsHandle, &member);
            LogToWindowFormat("XblMultiplayerEventArgsMember (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerEventArgsPropertiesJson(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XblMultiplayerEventArgsHandle argsHandle = nullptr;
            const char* properties = nullptr;
            const HRESULT hr = XblMultiplayerEventArgsPropertiesJson(argsHandle, &properties);
            LogToWindowFormat("XblMultiplayerEventArgsPropertiesJson (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerEventArgsFindMatchCompleted(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XblMultiplayerEventArgsHandle argsHandle = nullptr;
            XblMultiplayerMatchStatus matchStatus{};
            XblMultiplayerMeasurementFailure failureCause{};
            const HRESULT hr = XblMultiplayerEventArgsFindMatchCompleted(argsHandle, &matchStatus, &failureCause);
            LogToWindowFormat("XblMultiplayerEventArgsFindMatchCompleted (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerEventArgsPerformQoSMeasurements(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XblMultiplayerEventArgsHandle argsHandle = nullptr;
            XblMultiplayerPerformQoSMeasurementsArgs qosArgs{};
            const HRESULT hr = XblMultiplayerEventArgsPerformQoSMeasurements(argsHandle, &qosArgs);
            LogToWindowFormat("XblMultiplayerEventArgsPerformQoSMeasurements (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

// ── Member Helper ───────────────────────────────────────────────────────────

CommandResultPayload HandleXblMultiplayerManagerMemberAreMembersOnSameDevice(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XblMultiplayerManagerMember first{};
            XblMultiplayerManagerMember second{};
            const bool result = XblMultiplayerManagerMemberAreMembersOnSameDevice(&first, &second);
            LogToWindowFormat("XblMultiplayerManagerMemberAreMembersOnSameDevice (result=%s)", result ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionTimeOfSession(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            const time_t t = XblMultiplayerSessionTimeOfSession(s_multiplayerSession);
            LogToWindowFormat("XblMultiplayerSessionTimeOfSession (time=%lld)", static_cast<long long>(t));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionGetInitializationInfo(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            const XblMultiplayerSessionInitializationInfo* info = XblMultiplayerSessionGetInitializationInfo(s_multiplayerSession);
            LogToWindowFormat("XblMultiplayerSessionGetInitializationInfo (info=%p)", static_cast<const void*>(info));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionSubscribedChangeTypes(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            const XblMultiplayerSessionChangeTypes changeTypes = XblMultiplayerSessionSubscribedChangeTypes(s_multiplayerSession);
            LogToWindowFormat("XblMultiplayerSessionSubscribedChangeTypes (changeTypes=0x%08X)", static_cast<uint32_t>(changeTypes));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionSessionReference(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            const XblMultiplayerSessionReference* ref = XblMultiplayerSessionSessionReference(s_multiplayerSession);
            LogToWindowFormat("XblMultiplayerSessionSessionReference (ref=%p)", static_cast<const void*>(ref));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionSessionConstants(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            const XblMultiplayerSessionConstants* constants = XblMultiplayerSessionSessionConstants(s_multiplayerSession);
            LogToWindowFormat("XblMultiplayerSessionSessionConstants (constants=%p)", static_cast<const void*>(constants));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionConstantsSetMaxMembersInSession(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            std::string error;
            int64_t maxMembers = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "maxMembersInSession", maxMembers, error);
            XblMultiplayerSessionConstantsSetMaxMembersInSession(s_multiplayerSession, static_cast<uint32_t>(maxMembers));
            LogToWindowFormat("XblMultiplayerSessionConstantsSetMaxMembersInSession (maxMembers=%u)", static_cast<uint32_t>(maxMembers));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionConstantsSetVisibility(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            std::string error;
            int64_t visibility = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "visibility", visibility, error);
            XblMultiplayerSessionConstantsSetVisibility(s_multiplayerSession, static_cast<XblMultiplayerSessionVisibility>(visibility));
            LogToWindowFormat("XblMultiplayerSessionConstantsSetVisibility (visibility=%lld)", static_cast<long long>(visibility));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionSessionProperties(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            const XblMultiplayerSessionProperties* props = XblMultiplayerSessionSessionProperties(s_multiplayerSession);
            LogToWindowFormat("XblMultiplayerSessionSessionProperties (props=%p)", static_cast<const void*>(props));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionPropertiesSetJoinRestriction(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            std::string error;
            int64_t restriction = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "joinRestriction", restriction, error);
            XblMultiplayerSessionPropertiesSetJoinRestriction(s_multiplayerSession, static_cast<XblMultiplayerSessionRestriction>(restriction));
            LogToWindowFormat("XblMultiplayerSessionPropertiesSetJoinRestriction (restriction=%lld)", static_cast<long long>(restriction));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionPropertiesSetReadRestriction(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            std::string error;
            int64_t restriction = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "readRestriction", restriction, error);
            XblMultiplayerSessionPropertiesSetReadRestriction(s_multiplayerSession, static_cast<XblMultiplayerSessionRestriction>(restriction));
            LogToWindowFormat("XblMultiplayerSessionPropertiesSetReadRestriction (restriction=%lld)", static_cast<long long>(restriction));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionGetMember(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            std::string error;
            int64_t memberId = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "memberId", memberId, error);
            const XblMultiplayerSessionMember* member = XblMultiplayerSessionGetMember(s_multiplayerSession, static_cast<uint32_t>(memberId));
            LogToWindowFormat("XblMultiplayerSessionGetMember (member=%p)", static_cast<const void*>(member));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionMatchmakingServer(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            const XblMultiplayerMatchmakingServer* server = XblMultiplayerSessionMatchmakingServer(s_multiplayerSession);
            LogToWindowFormat("XblMultiplayerSessionMatchmakingServer (server=%p)", static_cast<const void*>(server));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionMembersAccepted(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            const uint32_t accepted = XblMultiplayerSessionMembersAccepted(s_multiplayerSession);
            LogToWindowFormat("XblMultiplayerSessionMembersAccepted (accepted=%u)", accepted);
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionRawServersJson(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            const char* serversJson = XblMultiplayerSessionRawServersJson(s_multiplayerSession);
            LogToWindowFormat("XblMultiplayerSessionRawServersJson (json=%s)", serversJson ? serversJson : "null");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionEtag(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            const char* etag = XblMultiplayerSessionEtag(s_multiplayerSession);
            LogToWindowFormat("XblMultiplayerSessionEtag (etag=%s)", etag ? etag : "null");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionCurrentUser(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            const XblMultiplayerSessionMember* member = XblMultiplayerSessionCurrentUser(s_multiplayerSession);
            LogToWindowFormat("XblMultiplayerSessionCurrentUser (member=%p)", static_cast<const void*>(member));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionGetInfo(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            const XblMultiplayerSessionInfo* info = XblMultiplayerSessionGetInfo(s_multiplayerSession);
            LogToWindowFormat("XblMultiplayerSessionGetInfo (info=%p)", static_cast<const void*>(info));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionWriteStatus(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            const XblWriteSessionStatus status = XblMultiplayerSessionWriteStatus(s_multiplayerSession);
            LogToWindowFormat("XblMultiplayerSessionWriteStatus (status=%d)", static_cast<int>(status));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionSetInitializationSucceeded(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            std::string error;
            bool succeeded = true;
            CommandHandlerShared::TryParseBoolParameter(parameters, "initializationSucceeded", succeeded, error);
            XblMultiplayerSessionSetInitializationSucceeded(s_multiplayerSession, succeeded);
            LogToWindowFormat("XblMultiplayerSessionSetInitializationSucceeded (succeeded=%s)", succeeded ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionSetHostDeviceToken(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            std::string error;
            std::string deviceToken;
            CommandHandlerShared::TryGetStringParameter(parameters, "deviceToken", deviceToken, error);
            XblDeviceToken token{};
            if (!deviceToken.empty()) { strncpy_s(token.Value, deviceToken.c_str(), _TRUNCATE); }
            XblMultiplayerSessionSetHostDeviceToken(s_multiplayerSession, token);
            LogToWindowFormat("XblMultiplayerSessionSetHostDeviceToken");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionSetMatchmakingServerConnectionPath(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            std::string error;
            std::string serverConnectionPath;
            CommandHandlerShared::TryGetStringParameter(parameters, "serverConnectionPath", serverConnectionPath, error);
            XblMultiplayerSessionSetMatchmakingServerConnectionPath(s_multiplayerSession, serverConnectionPath.c_str());
            LogToWindowFormat("XblMultiplayerSessionSetMatchmakingServerConnectionPath");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionSetClosed(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            std::string error;
            bool closed = true;
            CommandHandlerShared::TryParseBoolParameter(parameters, "closed", closed, error);
            XblMultiplayerSessionSetClosed(s_multiplayerSession, closed);
            LogToWindowFormat("XblMultiplayerSessionSetClosed (closed=%s)", closed ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionSetLocked(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            std::string error;
            bool locked = true;
            CommandHandlerShared::TryParseBoolParameter(parameters, "locked", locked, error);
            XblMultiplayerSessionSetLocked(s_multiplayerSession, locked);
            LogToWindowFormat("XblMultiplayerSessionSetLocked (locked=%s)", locked ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionSetAllocateCloudCompute(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            std::string error;
            bool allocate = true;
            CommandHandlerShared::TryParseBoolParameter(parameters, "allocateCloudCompute", allocate, error);
            XblMultiplayerSessionSetAllocateCloudCompute(s_multiplayerSession, allocate);
            LogToWindowFormat("XblMultiplayerSessionSetAllocateCloudCompute (allocate=%s)", allocate ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionSetMatchmakingResubmit(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            std::string error;
            bool resubmit = true;
            CommandHandlerShared::TryParseBoolParameter(parameters, "matchResubmit", resubmit, error);
            XblMultiplayerSessionSetMatchmakingResubmit(s_multiplayerSession, resubmit);
            LogToWindowFormat("XblMultiplayerSessionSetMatchmakingResubmit (resubmit=%s)", resubmit ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSessionCompare(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_multiplayerSession);
            const XblMultiplayerSessionChangeTypes changes = XblMultiplayerSessionCompare(s_multiplayerSession, s_multiplayerSession);
            LogToWindowFormat("XblMultiplayerSessionCompare (changes=0x%08X)", static_cast<uint32_t>(changes));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSearchHandleCloseHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (s_searchHandle)
            {
                XblMultiplayerSearchHandleCloseHandle(s_searchHandle);
                s_searchHandle = nullptr;
            }
            LogToWindowFormat("XblMultiplayerSearchHandleCloseHandle");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerWriteSessionResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerWriteSessionResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerWriteSessionByHandleResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerWriteSessionByHandleResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerGetSessionResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerGetSessionResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerGetSessionByHandleResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerGetSessionByHandleResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerQuerySessionsResultCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerQuerySessionsResultCount: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerQuerySessionsResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerQuerySessionsResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSetTransferHandleResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerSetTransferHandleResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerCreateSearchHandleResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerCreateSearchHandleResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerGetSearchHandlesResultCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerGetSearchHandlesResultCount: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerGetSearchHandlesResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerGetSearchHandlesResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSendInvitesResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerSendInvitesResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerGetActivitiesForSocialGroupResultCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerGetActivitiesForSocialGroupResultCount: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerGetActivitiesForSocialGroupResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerGetActivitiesForSocialGroupResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerGetActivitiesWithPropertiesForSocialGroupResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerGetActivitiesWithPropertiesForSocialGroupResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerGetActivitiesWithPropertiesForSocialGroupResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerGetActivitiesWithPropertiesForSocialGroupResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerGetActivitiesForUsersResultCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerGetActivitiesForUsersResultCount: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerGetActivitiesForUsersResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerGetActivitiesForUsersResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerGetActivitiesWithPropertiesForUsersResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerGetActivitiesWithPropertiesForUsersResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerGetActivitiesWithPropertiesForUsersResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerGetActivitiesWithPropertiesForUsersResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerSubscriptionsEnabled(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            const bool enabled = XblMultiplayerSubscriptionsEnabled(state->xblContext);
            LogToWindowFormat("XblMultiplayerSubscriptionsEnabled (enabled=%s)", enabled ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerAddSessionChangedHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            static XblFunctionContext s_sessionChangedToken = 0;
            s_sessionChangedToken = XblMultiplayerAddSessionChangedHandler(
                state->xblContext,
                [](void* /*context*/, XblMultiplayerSessionChangeEventArgs args)
                {
                    LogToWindowFormat("SessionChanged (changeNumber=%llu)", static_cast<unsigned long long>(args.ChangeNumber));
                },
                nullptr);
            LogToWindowFormat("XblMultiplayerAddSessionChangedHandler (token=%lld)", static_cast<long long>(s_sessionChangedToken));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerRemoveSessionChangedHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            std::string error;
            int64_t token{};
            CommandHandlerShared::TryGetInt64Parameter(parameters, "token", token, error);
            XblMultiplayerRemoveSessionChangedHandler(state->xblContext, static_cast<XblFunctionContext>(token));
            LogToWindowFormat("XblMultiplayerRemoveSessionChangedHandler (token=%lld)", token);
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerAddSubscriptionLostHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            static XblFunctionContext s_subscriptionLostToken = 0;
            s_subscriptionLostToken = XblMultiplayerAddSubscriptionLostHandler(
                state->xblContext,
                [](void* /*context*/)
                {
                    LogToWindowFormat("SubscriptionLost");
                },
                nullptr);
            LogToWindowFormat("XblMultiplayerAddSubscriptionLostHandler (token=%lld)", static_cast<long long>(s_subscriptionLostToken));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerRemoveSubscriptionLostHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            std::string error;
            int64_t token{};
            CommandHandlerShared::TryGetInt64Parameter(parameters, "token", token, error);
            XblMultiplayerRemoveSubscriptionLostHandler(state->xblContext, static_cast<XblFunctionContext>(token));
            LogToWindowFormat("XblMultiplayerRemoveSubscriptionLostHandler (token=%lld)", token);
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerAddConnectionIdChangedHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            static XblFunctionContext s_connectionIdChangedToken = 0;
            s_connectionIdChangedToken = XblMultiplayerAddConnectionIdChangedHandler(
                state->xblContext,
                [](void* /*context*/)
                {
                    LogToWindowFormat("ConnectionIdChanged");
                },
                nullptr);
            LogToWindowFormat("XblMultiplayerAddConnectionIdChangedHandler (token=%lld)", static_cast<long long>(s_connectionIdChangedToken));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerRemoveConnectionIdChangedHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            std::string error;
            int64_t token{};
            CommandHandlerShared::TryGetInt64Parameter(parameters, "token", token, error);
            XblMultiplayerRemoveConnectionIdChangedHandler(state->xblContext, static_cast<XblFunctionContext>(token));
            LogToWindowFormat("XblMultiplayerRemoveConnectionIdChangedHandler (token=%lld)", token);
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionLocalMembersCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const size_t count = XblMultiplayerManagerLobbySessionLocalMembersCount();
            LogToWindowFormat("XblMultiplayerManagerLobbySessionLocalMembersCount (count=%zu)", count);
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionMembersCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const size_t count = XblMultiplayerManagerLobbySessionMembersCount();
            LogToWindowFormat("XblMultiplayerManagerLobbySessionMembersCount (count=%zu)", count);
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionPropertiesJson(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const char* json = XblMultiplayerManagerLobbySessionPropertiesJson();
            LogToWindowFormat("XblMultiplayerManagerLobbySessionPropertiesJson (json=%s)", json ? json : "null");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionConstants(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const XblMultiplayerSessionConstants* constants = XblMultiplayerManagerLobbySessionConstants();
            LogToWindowFormat("XblMultiplayerManagerLobbySessionConstants (constants=%p)", static_cast<const void*>(constants));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerLobbySessionIsHost(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            int64_t xuid = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "xuid", xuid, error);
            const bool isHost = XblMultiplayerManagerLobbySessionIsHost(static_cast<uint64_t>(xuid));
            LogToWindowFormat("XblMultiplayerManagerLobbySessionIsHost (isHost=%s)", isHost ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerGameSessionActive(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const bool active = XblMultiplayerManagerGameSessionActive();
            LogToWindowFormat("XblMultiplayerManagerGameSessionActive (active=%s)", active ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerGameSessionCorrelationId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const char* correlationId = XblMultiplayerManagerGameSessionCorrelationId();
            LogToWindowFormat("XblMultiplayerManagerGameSessionCorrelationId (id=%s)", correlationId ? correlationId : "null");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerGameSessionSessionReference(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const XblMultiplayerSessionReference* ref = XblMultiplayerManagerGameSessionSessionReference();
            LogToWindowFormat("XblMultiplayerManagerGameSessionSessionReference (ref=%p)", static_cast<const void*>(ref));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerGameSessionMembersCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const size_t count = XblMultiplayerManagerGameSessionMembersCount();
            LogToWindowFormat("XblMultiplayerManagerGameSessionMembersCount (count=%zu)", count);
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerGameSessionPropertiesJson(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const char* json = XblMultiplayerManagerGameSessionPropertiesJson();
            LogToWindowFormat("XblMultiplayerManagerGameSessionPropertiesJson (json=%s)", json ? json : "null");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerGameSessionConstants(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const XblMultiplayerSessionConstants* constants = XblMultiplayerManagerGameSessionConstants();
            LogToWindowFormat("XblMultiplayerManagerGameSessionConstants (constants=%p)", static_cast<const void*>(constants));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerGameSessionIsHost(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            int64_t xuid = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "xuid", xuid, error);
            const bool isHost = XblMultiplayerManagerGameSessionIsHost(static_cast<uint64_t>(xuid));
            LogToWindowFormat("XblMultiplayerManagerGameSessionIsHost (isHost=%s)", isHost ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerCancelMatch(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XblMultiplayerManagerCancelMatch();
            LogToWindowFormat("XblMultiplayerManagerCancelMatch");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerMatchStatus(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const XblMultiplayerMatchStatus status = XblMultiplayerManagerMatchStatus();
            LogToWindowFormat("XblMultiplayerManagerMatchStatus (status=%d)", static_cast<int>(status));
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerEstimatedMatchWaitTime(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const uint32_t waitTime = XblMultiplayerManagerEstimatedMatchWaitTime();
            LogToWindowFormat("XblMultiplayerManagerEstimatedMatchWaitTime (waitTime=%u)", waitTime);
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerAutoFillMembersDuringMatchmaking(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const bool autoFill = XblMultiplayerManagerAutoFillMembersDuringMatchmaking();
            LogToWindowFormat("XblMultiplayerManagerAutoFillMembersDuringMatchmaking (autoFill=%s)", autoFill ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerSetAutoFillMembersDuringMatchmaking(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            bool autoFillMembers = true;
            CommandHandlerShared::TryParseBoolParameter(parameters, "autoFillMembers", autoFillMembers, error);
            XblMultiplayerManagerSetAutoFillMembersDuringMatchmaking(autoFillMembers);
            LogToWindowFormat("XblMultiplayerManagerSetAutoFillMembersDuringMatchmaking (autoFill=%s)", autoFillMembers ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerManagerJoinability(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const XblMultiplayerJoinability joinability = XblMultiplayerManagerJoinability();
            LogToWindowFormat("XblMultiplayerManagerJoinability (joinability=%d)", static_cast<int>(joinability));
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblFormatSecureDeviceAddress", HandleXblFormatSecureDeviceAddress },
    { "XblMultiplayerAddConnectionIdChangedHandler", HandleXblMultiplayerAddConnectionIdChangedHandler },
    { "XblMultiplayerAddSessionChangedHandler", HandleXblMultiplayerAddSessionChangedHandler },
    { "XblMultiplayerAddSubscriptionLostHandler", HandleXblMultiplayerAddSubscriptionLostHandler },
    { "XblMultiplayerClearActivityAsync", HandleXblMultiplayerClearActivityAsync },
    { "XblMultiplayerCreateSearchHandleAsync", HandleXblMultiplayerCreateSearchHandleAsync },
    { "XblMultiplayerCreateSearchHandleResult", HandleXblMultiplayerCreateSearchHandleResult },
    { "XblMultiplayerDeleteSearchHandleAsync", HandleXblMultiplayerDeleteSearchHandleAsync },
    { "XblMultiplayerEventArgsFindMatchCompleted", HandleXblMultiplayerEventArgsFindMatchCompleted },
    { "XblMultiplayerEventArgsMember", HandleXblMultiplayerEventArgsMember },
    { "XblMultiplayerEventArgsMembers", HandleXblMultiplayerEventArgsMembers },
    { "XblMultiplayerEventArgsMembersCount", HandleXblMultiplayerEventArgsMembersCount },
    { "XblMultiplayerEventArgsPerformQoSMeasurements", HandleXblMultiplayerEventArgsPerformQoSMeasurements },
    { "XblMultiplayerEventArgsPropertiesJson", HandleXblMultiplayerEventArgsPropertiesJson },
    { "XblMultiplayerEventArgsXuid", HandleXblMultiplayerEventArgsXuid },
    { "XblMultiplayerGetActivitiesForSocialGroupAsync", HandleXblMultiplayerGetActivitiesForSocialGroupAsync },
    { "XblMultiplayerGetActivitiesForSocialGroupResult", HandleXblMultiplayerGetActivitiesForSocialGroupResult },
    { "XblMultiplayerGetActivitiesForSocialGroupResultCount", HandleXblMultiplayerGetActivitiesForSocialGroupResultCount },
    { "XblMultiplayerGetActivitiesForUsersAsync", HandleXblMultiplayerGetActivitiesForUsersAsync },
    { "XblMultiplayerGetActivitiesForUsersResult", HandleXblMultiplayerGetActivitiesForUsersResult },
    { "XblMultiplayerGetActivitiesForUsersResultCount", HandleXblMultiplayerGetActivitiesForUsersResultCount },
    { "XblMultiplayerGetActivitiesWithPropertiesForSocialGroupAsync", HandleXblMultiplayerGetActivitiesWithPropertiesForSocialGroupAsync },
    { "XblMultiplayerGetActivitiesWithPropertiesForSocialGroupResult", HandleXblMultiplayerGetActivitiesWithPropertiesForSocialGroupResult },
    { "XblMultiplayerGetActivitiesWithPropertiesForSocialGroupResultSize", HandleXblMultiplayerGetActivitiesWithPropertiesForSocialGroupResultSize },
    { "XblMultiplayerGetActivitiesWithPropertiesForUsersAsync", HandleXblMultiplayerGetActivitiesWithPropertiesForUsersAsync },
    { "XblMultiplayerGetActivitiesWithPropertiesForUsersResult", HandleXblMultiplayerGetActivitiesWithPropertiesForUsersResult },
    { "XblMultiplayerGetActivitiesWithPropertiesForUsersResultSize", HandleXblMultiplayerGetActivitiesWithPropertiesForUsersResultSize },
    { "XblMultiplayerGetSearchHandlesAsync", HandleXblMultiplayerGetSearchHandlesAsync },
    { "XblMultiplayerGetSearchHandlesResult", HandleXblMultiplayerGetSearchHandlesResult },
    { "XblMultiplayerGetSearchHandlesResultCount", HandleXblMultiplayerGetSearchHandlesResultCount },
    { "XblMultiplayerGetSessionAsync", HandleXblMultiplayerGetSessionAsync },
    { "XblMultiplayerGetSessionByHandleAsync", HandleXblMultiplayerGetSessionByHandleAsync },
    { "XblMultiplayerGetSessionByHandleResult", HandleXblMultiplayerGetSessionByHandleResult },
    { "XblMultiplayerGetSessionResult", HandleXblMultiplayerGetSessionResult },
    { "XblMultiplayerManagerAutoFillMembersDuringMatchmaking", HandleXblMultiplayerManagerAutoFillMembersDuringMatchmaking },
    { "XblMultiplayerManagerCancelMatch", HandleXblMultiplayerManagerCancelMatch },
    { "XblMultiplayerManagerDoWork", HandleXblMultiplayerManagerDoWork },
    { "XblMultiplayerManagerEstimatedMatchWaitTime", HandleXblMultiplayerManagerEstimatedMatchWaitTime },
    { "XblMultiplayerManagerFindMatch", HandleXblMultiplayerManagerFindMatch },
    { "XblMultiplayerManagerGameSessionActive", HandleXblMultiplayerManagerGameSessionActive },
    { "XblMultiplayerManagerGameSessionConstants", HandleXblMultiplayerManagerGameSessionConstants },
    { "XblMultiplayerManagerGameSessionCorrelationId", HandleXblMultiplayerManagerGameSessionCorrelationId },
    { "XblMultiplayerManagerGameSessionHost", HandleXblMultiplayerManagerGameSessionHost },
    { "XblMultiplayerManagerGameSessionIsHost", HandleXblMultiplayerManagerGameSessionIsHost },
    { "XblMultiplayerManagerGameSessionMembers", HandleXblMultiplayerManagerGameSessionMembers },
    { "XblMultiplayerManagerGameSessionMembersCount", HandleXblMultiplayerManagerGameSessionMembersCount },
    { "XblMultiplayerManagerGameSessionPropertiesJson", HandleXblMultiplayerManagerGameSessionPropertiesJson },
    { "XblMultiplayerManagerGameSessionSessionReference", HandleXblMultiplayerManagerGameSessionSessionReference },
    { "XblMultiplayerManagerGameSessionSetProperties", HandleXblMultiplayerManagerGameSessionSetProperties },
    { "XblMultiplayerManagerGameSessionSetSynchronizedHost", HandleXblMultiplayerManagerGameSessionSetSynchronizedHost },
    { "XblMultiplayerManagerGameSessionSetSynchronizedProperties", HandleXblMultiplayerManagerGameSessionSetSynchronizedProperties },
    { "XblMultiplayerManagerInitialize", HandleXblMultiplayerManagerInitialize },
    { "XblMultiplayerManagerJoinGame", HandleXblMultiplayerManagerJoinGame },
    { "XblMultiplayerManagerJoinGameFromLobby", HandleXblMultiplayerManagerJoinGameFromLobby },
    { "XblMultiplayerManagerJoinLobby", HandleXblMultiplayerManagerJoinLobby },
    { "XblMultiplayerManagerJoinability", HandleXblMultiplayerManagerJoinability },
    { "XblMultiplayerManagerLeaveGame", HandleXblMultiplayerManagerLeaveGame },
    { "XblMultiplayerManagerLobbySessionAddLocalUser", HandleXblMultiplayerManagerLobbySessionAddLocalUser },
    { "XblMultiplayerManagerLobbySessionConstants", HandleXblMultiplayerManagerLobbySessionConstants },
    { "XblMultiplayerManagerLobbySessionCorrelationId", HandleXblMultiplayerManagerLobbySessionCorrelationId },
    { "XblMultiplayerManagerLobbySessionDeleteLocalMemberProperties", HandleXblMultiplayerManagerLobbySessionDeleteLocalMemberProperties },
    { "XblMultiplayerManagerLobbySessionHost", HandleXblMultiplayerManagerLobbySessionHost },
    { "XblMultiplayerManagerLobbySessionInviteFriends", HandleXblMultiplayerManagerLobbySessionInviteFriends },
    { "XblMultiplayerManagerLobbySessionInviteUsers", HandleXblMultiplayerManagerLobbySessionInviteUsers },
    { "XblMultiplayerManagerLobbySessionIsHost", HandleXblMultiplayerManagerLobbySessionIsHost },
    { "XblMultiplayerManagerLobbySessionLocalMembers", HandleXblMultiplayerManagerLobbySessionLocalMembers },
    { "XblMultiplayerManagerLobbySessionLocalMembersCount", HandleXblMultiplayerManagerLobbySessionLocalMembersCount },
    { "XblMultiplayerManagerLobbySessionMembers", HandleXblMultiplayerManagerLobbySessionMembers },
    { "XblMultiplayerManagerLobbySessionMembersCount", HandleXblMultiplayerManagerLobbySessionMembersCount },
    { "XblMultiplayerManagerLobbySessionPropertiesJson", HandleXblMultiplayerManagerLobbySessionPropertiesJson },
    { "XblMultiplayerManagerLobbySessionRemoveLocalUser", HandleXblMultiplayerManagerLobbySessionRemoveLocalUser },
    { "XblMultiplayerManagerLobbySessionSessionReference", HandleXblMultiplayerManagerLobbySessionSessionReference },
    { "XblMultiplayerManagerLobbySessionSetLocalMemberConnectionAddress", HandleXblMultiplayerManagerLobbySessionSetLocalMemberConnectionAddress },
    { "XblMultiplayerManagerLobbySessionSetLocalMemberProperties", HandleXblMultiplayerManagerLobbySessionSetLocalMemberProperties },
    { "XblMultiplayerManagerLobbySessionSetProperties", HandleXblMultiplayerManagerLobbySessionSetProperties },
    { "XblMultiplayerManagerLobbySessionSetSynchronizedHost", HandleXblMultiplayerManagerLobbySessionSetSynchronizedHost },
    { "XblMultiplayerManagerLobbySessionSetSynchronizedProperties", HandleXblMultiplayerManagerLobbySessionSetSynchronizedProperties },
    { "XblMultiplayerManagerMatchStatus", HandleXblMultiplayerManagerMatchStatus },
    { "XblMultiplayerManagerMemberAreMembersOnSameDevice", HandleXblMultiplayerManagerMemberAreMembersOnSameDevice },
    { "XblMultiplayerManagerSetAutoFillMembersDuringMatchmaking", HandleXblMultiplayerManagerSetAutoFillMembersDuringMatchmaking },
    { "XblMultiplayerManagerSetJoinability", HandleXblMultiplayerManagerSetJoinability },
    { "XblMultiplayerManagerSetQosMeasurements", HandleXblMultiplayerManagerSetQosMeasurements },
    { "XblMultiplayerQuerySessionsAsync", HandleXblMultiplayerQuerySessionsAsync },
    { "XblMultiplayerQuerySessionsResult", HandleXblMultiplayerQuerySessionsResult },
    { "XblMultiplayerQuerySessionsResultCount", HandleXblMultiplayerQuerySessionsResultCount },
    { "XblMultiplayerRemoveConnectionIdChangedHandler", HandleXblMultiplayerRemoveConnectionIdChangedHandler },
    { "XblMultiplayerRemoveSessionChangedHandler", HandleXblMultiplayerRemoveSessionChangedHandler },
    { "XblMultiplayerRemoveSubscriptionLostHandler", HandleXblMultiplayerRemoveSubscriptionLostHandler },
    { "XblMultiplayerSearchHandleCloseHandle", HandleXblMultiplayerSearchHandleCloseHandle },
    { "XblMultiplayerSearchHandleDuplicateHandle", HandleXblMultiplayerSearchHandleDuplicateHandle },
    { "XblMultiplayerSearchHandleGetCreationTime", HandleXblMultiplayerSearchHandleGetCreationTime },
    { "XblMultiplayerSearchHandleGetCustomSessionPropertiesJson", HandleXblMultiplayerSearchHandleGetCustomSessionPropertiesJson },
    { "XblMultiplayerSearchHandleGetId", HandleXblMultiplayerSearchHandleGetId },
    { "XblMultiplayerSearchHandleGetJoinRestriction", HandleXblMultiplayerSearchHandleGetJoinRestriction },
    { "XblMultiplayerSearchHandleGetMemberCounts", HandleXblMultiplayerSearchHandleGetMemberCounts },
    { "XblMultiplayerSearchHandleGetNumberAttributes", HandleXblMultiplayerSearchHandleGetNumberAttributes },
    { "XblMultiplayerSearchHandleGetSessionClosed", HandleXblMultiplayerSearchHandleGetSessionClosed },
    { "XblMultiplayerSearchHandleGetSessionOwnerXuids", HandleXblMultiplayerSearchHandleGetSessionOwnerXuids },
    { "XblMultiplayerSearchHandleGetSessionReference", HandleXblMultiplayerSearchHandleGetSessionReference },
    { "XblMultiplayerSearchHandleGetStringAttributes", HandleXblMultiplayerSearchHandleGetStringAttributes },
    { "XblMultiplayerSearchHandleGetTags", HandleXblMultiplayerSearchHandleGetTags },
    { "XblMultiplayerSearchHandleGetVisibility", HandleXblMultiplayerSearchHandleGetVisibility },
    { "XblMultiplayerSendInvitesAsync", HandleXblMultiplayerSendInvitesAsync },
    { "XblMultiplayerSendInvitesResult", HandleXblMultiplayerSendInvitesResult },
    { "XblMultiplayerSessionAddMemberReservation", HandleXblMultiplayerSessionAddMemberReservation },
    { "XblMultiplayerSessionCloseHandle", HandleXblMultiplayerSessionCloseHandle },
    { "XblMultiplayerSessionCompare", HandleXblMultiplayerSessionCompare },
    { "XblMultiplayerSessionConstantsSetArbitrationTimeouts", HandleXblMultiplayerSessionConstantsSetArbitrationTimeouts },
    { "XblMultiplayerSessionConstantsSetCapabilities", HandleXblMultiplayerSessionConstantsSetCapabilities },
    { "XblMultiplayerSessionConstantsSetCloudComputePackageJson", HandleXblMultiplayerSessionConstantsSetCloudComputePackageJson },
    { "XblMultiplayerSessionConstantsSetMaxMembersInSession", HandleXblMultiplayerSessionConstantsSetMaxMembersInSession },
    { "XblMultiplayerSessionConstantsSetMeasurementServerAddressesJson", HandleXblMultiplayerSessionConstantsSetMeasurementServerAddressesJson },
    { "XblMultiplayerSessionConstantsSetMemberInitialization", HandleXblMultiplayerSessionConstantsSetMemberInitialization },
    { "XblMultiplayerSessionConstantsSetPeerToHostRequirements", HandleXblMultiplayerSessionConstantsSetPeerToHostRequirements },
    { "XblMultiplayerSessionConstantsSetPeerToPeerRequirements", HandleXblMultiplayerSessionConstantsSetPeerToPeerRequirements },
    { "XblMultiplayerSessionConstantsSetQosConnectivityMetrics", HandleXblMultiplayerSessionConstantsSetQosConnectivityMetrics },
    { "XblMultiplayerSessionConstantsSetTimeouts", HandleXblMultiplayerSessionConstantsSetTimeouts },
    { "XblMultiplayerSessionConstantsSetVisibility", HandleXblMultiplayerSessionConstantsSetVisibility },
    { "XblMultiplayerSessionCreateHandle", HandleXblMultiplayerSessionCreateHandle },
    { "XblMultiplayerSessionCurrentUser", HandleXblMultiplayerSessionCurrentUser },
    { "XblMultiplayerSessionCurrentUserDeleteCustomPropertyJson", HandleXblMultiplayerSessionCurrentUserDeleteCustomPropertyJson },
    { "XblMultiplayerSessionCurrentUserSetCustomPropertyJson", HandleXblMultiplayerSessionCurrentUserSetCustomPropertyJson },
    { "XblMultiplayerSessionCurrentUserSetEncounters", HandleXblMultiplayerSessionCurrentUserSetEncounters },
    { "XblMultiplayerSessionCurrentUserSetGroups", HandleXblMultiplayerSessionCurrentUserSetGroups },
    { "XblMultiplayerSessionCurrentUserSetMembersInGroup", HandleXblMultiplayerSessionCurrentUserSetMembersInGroup },
    { "XblMultiplayerSessionCurrentUserSetQosMeasurements", HandleXblMultiplayerSessionCurrentUserSetQosMeasurements },
    { "XblMultiplayerSessionCurrentUserSetRoles", HandleXblMultiplayerSessionCurrentUserSetRoles },
    { "XblMultiplayerSessionCurrentUserSetSecureDeviceAddressBase64", HandleXblMultiplayerSessionCurrentUserSetSecureDeviceAddressBase64 },
    { "XblMultiplayerSessionCurrentUserSetServerQosMeasurements", HandleXblMultiplayerSessionCurrentUserSetServerQosMeasurements },
    { "XblMultiplayerSessionCurrentUserSetStatus", HandleXblMultiplayerSessionCurrentUserSetStatus },
    { "XblMultiplayerSessionDeleteCustomPropertyJson", HandleXblMultiplayerSessionDeleteCustomPropertyJson },
    { "XblMultiplayerSessionDuplicateHandle", HandleXblMultiplayerSessionDuplicateHandle },
    { "XblMultiplayerSessionEtag", HandleXblMultiplayerSessionEtag },
    { "XblMultiplayerSessionGetInfo", HandleXblMultiplayerSessionGetInfo },
    { "XblMultiplayerSessionGetInitializationInfo", HandleXblMultiplayerSessionGetInitializationInfo },
    { "XblMultiplayerSessionGetMember", HandleXblMultiplayerSessionGetMember },
    { "XblMultiplayerSessionGetRoleByName", HandleXblMultiplayerSessionGetRoleByName },
    { "XblMultiplayerSessionHostCandidates", HandleXblMultiplayerSessionHostCandidates },
    { "XblMultiplayerSessionJoin", HandleXblMultiplayerSessionJoin },
    { "XblMultiplayerSessionLeave", HandleXblMultiplayerSessionLeave },
    { "XblMultiplayerSessionMatchmakingServer", HandleXblMultiplayerSessionMatchmakingServer },
    { "XblMultiplayerSessionMembers", HandleXblMultiplayerSessionMembers },
    { "XblMultiplayerSessionMembersAccepted", HandleXblMultiplayerSessionMembersAccepted },
    { "XblMultiplayerSessionPropertiesSetJoinRestriction", HandleXblMultiplayerSessionPropertiesSetJoinRestriction },
    { "XblMultiplayerSessionPropertiesSetKeywords", HandleXblMultiplayerSessionPropertiesSetKeywords },
    { "XblMultiplayerSessionPropertiesSetReadRestriction", HandleXblMultiplayerSessionPropertiesSetReadRestriction },
    { "XblMultiplayerSessionPropertiesSetTurnCollection", HandleXblMultiplayerSessionPropertiesSetTurnCollection },
    { "XblMultiplayerSessionRawServersJson", HandleXblMultiplayerSessionRawServersJson },
    { "XblMultiplayerSessionReferenceCreate", HandleXblMultiplayerSessionReferenceCreate },
    { "XblMultiplayerSessionReferenceIsValid", HandleXblMultiplayerSessionReferenceIsValid },
    { "XblMultiplayerSessionReferenceParseFromUriPath", HandleXblMultiplayerSessionReferenceParseFromUriPath },
    { "XblMultiplayerSessionReferenceToUriPath", HandleXblMultiplayerSessionReferenceToUriPath },
    { "XblMultiplayerSessionRoleTypes", HandleXblMultiplayerSessionRoleTypes },
    { "XblMultiplayerSessionSessionConstants", HandleXblMultiplayerSessionSessionConstants },
    { "XblMultiplayerSessionSessionProperties", HandleXblMultiplayerSessionSessionProperties },
    { "XblMultiplayerSessionSessionReference", HandleXblMultiplayerSessionSessionReference },
    { "XblMultiplayerSessionSetAllocateCloudCompute", HandleXblMultiplayerSessionSetAllocateCloudCompute },
    { "XblMultiplayerSessionSetClosed", HandleXblMultiplayerSessionSetClosed },
    { "XblMultiplayerSessionSetCustomPropertyJson", HandleXblMultiplayerSessionSetCustomPropertyJson },
    { "XblMultiplayerSessionSetHostDeviceToken", HandleXblMultiplayerSessionSetHostDeviceToken },
    { "XblMultiplayerSessionSetInitializationSucceeded", HandleXblMultiplayerSessionSetInitializationSucceeded },
    { "XblMultiplayerSessionSetLocked", HandleXblMultiplayerSessionSetLocked },
    { "XblMultiplayerSessionSetMatchmakingResubmit", HandleXblMultiplayerSessionSetMatchmakingResubmit },
    { "XblMultiplayerSessionSetMatchmakingServerConnectionPath", HandleXblMultiplayerSessionSetMatchmakingServerConnectionPath },
    { "XblMultiplayerSessionSetMatchmakingTargetSessionConstantsJson", HandleXblMultiplayerSessionSetMatchmakingTargetSessionConstantsJson },
    { "XblMultiplayerSessionSetMutableRoleSettings", HandleXblMultiplayerSessionSetMutableRoleSettings },
    { "XblMultiplayerSessionSetRawServersJson", HandleXblMultiplayerSessionSetRawServersJson },
    { "XblMultiplayerSessionSetServerConnectionStringCandidates", HandleXblMultiplayerSessionSetServerConnectionStringCandidates },
    { "XblMultiplayerSessionSetSessionChangeSubscription", HandleXblMultiplayerSessionSetSessionChangeSubscription },
    { "XblMultiplayerSessionSubscribedChangeTypes", HandleXblMultiplayerSessionSubscribedChangeTypes },
    { "XblMultiplayerSessionTimeOfSession", HandleXblMultiplayerSessionTimeOfSession },
    { "XblMultiplayerSessionWriteStatus", HandleXblMultiplayerSessionWriteStatus },
    { "XblMultiplayerSetActivityAsync", HandleXblMultiplayerSetActivityAsync },
    { "XblMultiplayerSetSubscriptionsEnabled", HandleXblMultiplayerSetSubscriptionsEnabled },
    { "XblMultiplayerSetTransferHandleAsync", HandleXblMultiplayerSetTransferHandleAsync },
    { "XblMultiplayerSetTransferHandleResult", HandleXblMultiplayerSetTransferHandleResult },
    { "XblMultiplayerSubscriptionsEnabled", HandleXblMultiplayerSubscriptionsEnabled },
    { "XblMultiplayerWriteSessionAsync", HandleXblMultiplayerWriteSessionAsync },
    { "XblMultiplayerWriteSessionByHandleAsync", HandleXblMultiplayerWriteSessionByHandleAsync },
    { "XblMultiplayerWriteSessionByHandleResult", HandleXblMultiplayerWriteSessionByHandleResult },
    { "XblMultiplayerWriteSessionResult", HandleXblMultiplayerWriteSessionResult }
});
