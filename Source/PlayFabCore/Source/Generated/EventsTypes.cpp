#include "stdafx.h"
#include "EventsTypes.h"
#include "JsonUtils.h"

namespace PlayFab
{
namespace Events
{

JsonValue EventContents::ToJson() const
{
    return EventContents::ToJson(this->Model());
}

JsonValue EventContents::ToJson(const PFEventsEventContents& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "EventNamespace", input.eventNamespace);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    JsonUtils::ObjectAddMember(output, "OriginalId", input.originalId);
    JsonUtils::ObjectAddMemberTime(output, "OriginalTimestamp", input.originalTimestamp);
    JsonUtils::ObjectAddMember(output, "Payload", input.payload);
    JsonUtils::ObjectAddMember(output, "PayloadJSON", input.payloadJSON);
    return output;
}

JsonValue WriteEventsRequest::ToJson() const
{
    return WriteEventsRequest::ToJson(this->Model());
}

JsonValue WriteEventsRequest::ToJson(const PFEventsWriteEventsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMemberArray<EventContents>(output, "Events", input.events, input.eventsCount);
    return output;
}

HRESULT WriteEventsResponse::FromJson(const JsonValue& input)
{
    CStringVector assignedEventIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AssignedEventIds", assignedEventIds));
    this->SetAssignedEventIds(std::move(assignedEventIds));

    return S_OK;
}

size_t WriteEventsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsWriteEventsResponse const*> WriteEventsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<WriteEventsResponse>(&this->Model());
}

size_t WriteEventsResponse::RequiredBufferSize(const PFEventsWriteEventsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(char*) + sizeof(char*) * model.assignedEventIdsCount);
    for (size_t i = 0; i < model.assignedEventIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.assignedEventIds[i]) + 1);
    }
    return requiredSize;
}

HRESULT WriteEventsResponse::Copy(const PFEventsWriteEventsResponse& input, PFEventsWriteEventsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray(input.assignedEventIds, input.assignedEventIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.assignedEventIds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

} // namespace Events
} // namespace PlayFab
