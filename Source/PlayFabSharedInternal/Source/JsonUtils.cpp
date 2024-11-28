#include "pch.h"
#include "JsonUtils.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace JsonUtils
{

String WriteToString(const JsonValue& jsonValue)
{
    // Convert JSON object to string
    return String{ jsonValue.dump() };
}

JsonValue ToJson(const char* string)
{
    if (!string)
    {
        return JsonValue::value_t::null;
    }

    return string;
}

JsonValue ToJson(const String& string)
{
    // By design, map empty string to null JsonValue
    if (string.empty())
    {
        return JsonValue::value_t::null;
    }

    return string.data();
}

JsonValue ToJson(const PFJsonObject& jsonObject)
{
    // By design, map empty jsonObject to null JsonValue
    if (!jsonObject.stringValue)
    {
        return JsonValue{};
    }

    return JsonValue::parse(jsonObject.stringValue);
}

JsonValue ToJsonTime(time_t value)
{
    return TimeTToIso8601String(value).data();
}

JsonValue ToJson(const InputModel& value)
{
    return value.ToJson();
}

HRESULT FromJson(const JsonValue& input, String& output)
{
    if (!input.is_string())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = input.get<String>();
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, bool& output)
{
    if (!input.is_boolean())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = input.get<bool>();
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, int16_t& output)
{
    if (!input.is_number_integer())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = static_cast<int16_t>(input.get<int>());
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, uint16_t& output)
{
    if (!input.is_number_unsigned())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = static_cast<uint16_t>(input.get<unsigned int>());
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, int32_t& output)
{
    if (!input.is_number_integer())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = static_cast<int32_t>(input.get<int>());
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, uint32_t& output)
{
    if (!input.is_number_unsigned())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = static_cast<uint32_t>(input.get<unsigned int>());
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, int64_t& output)
{
    if (!input.is_number_integer())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = static_cast<int64_t>(input.get<int64_t>());

    return S_OK;
}

HRESULT FromJson(const JsonValue& input, uint64_t& output)
{
    if (!input.is_number_unsigned())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = static_cast<uint64_t>(input.get<unsigned int>());
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, float& output)
{
    if (!input.is_number_float())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = input.get<float>();
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, double& output)
{
    if (!input.is_number_float())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = input.get<double>();
    return S_OK;
}

HRESULT FromJsonTime(const JsonValue& input, time_t& output)
{
    if (!input.is_string())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = Iso8601StringToTimeT(input.get<String>());
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, JsonObject& output)
{
    if (!input.is_object())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output.stringValue = JsonUtils::WriteToString(input);
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, JsonValue& output)
{
    output = input;
    return S_OK;
}

HRESULT ObjectAddMember(JsonValue& jsonObject, JsonValue&& name, JsonValue&& value)
{
    if (!jsonObject.is_object())
    {
        TRACE_ERROR("Json Error: cannot add member to non-object");
        return E_FAIL;
    }

    jsonObject.emplace(name, value);
    return S_OK;
}

HRESULT ObjectAddMember(JsonValue& jsonObject, StringRefType name, JsonValue&& value)
{
    if (!jsonObject.is_object())
    {
        TRACE_ERROR("Json Error: cannot add member to non-object");
        return E_FAIL;
    }

    jsonObject.emplace(name, value);
    return S_OK;
}

HRESULT ObjectAddMember(JsonValue& jsonObject, StringRefType name, const String& value)
{
    return ObjectAddMember(jsonObject, name, ToJson(value));
}

HRESULT ObjectAddMemberTime(JsonValue& jsonObject, StringRefType name, time_t value)
{
    return ObjectAddMember(jsonObject, name, ToJsonTime(value));
}

HRESULT ObjectAddMemberTime(JsonValue& jsonObject, StringRefType name, const std::optional<time_t>& value)
{
    if (value.has_value())
    {
        return ObjectAddMember(jsonObject, name, ToJsonTime(*value));
    }
    else
    {
        return ObjectAddMember(jsonObject, name, JsonValue{});
    }
}

HRESULT ObjectAddMemberTime(JsonValue& jsonObject, StringRefType name, const time_t* value)
{
    if (value != nullptr)
    {
        return ObjectAddMember(jsonObject, name, ToJsonTime(*value));
    }
    else
    {
        return ObjectAddMember(jsonObject, name, JsonValue{});
    }
}

HRESULT ObjectAddMemberTime(JsonValue& jsonObject, StringRefType name, const time_t* array, uint32_t arrayCount)
{
    JsonValue member{};
    for (auto i = 0u; i < arrayCount; ++i)
    {
        member.push_back(ToJsonTime(array[i]));
    }
    return ObjectAddMember(jsonObject, name, std::move(member));
}

HRESULT ObjectAddMemberTime(JsonValue& jsonObject, StringRefType name, const PFDateTimeDictionaryEntry* associativeArray, uint32_t arrayCount)
{
    JsonValue member{};
    for (auto i = 0u; i < arrayCount; ++i)
    {
        auto& entry{ associativeArray[i] };
        RETURN_IF_FAILED(ObjectAddMember(member, ToJson(entry.key), ToJsonTime(entry.value)));
    }
    return ObjectAddMember(jsonObject, name, std::move(member));
}

HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, JsonValue& output)
{
    auto findResult = Detail::ObjectFindMember(jsonObject, name);
    RETURN_IF_FAILED(findResult.hr);
    output = nullptr;

    if (findResult.Payload() != jsonObject.cend())
    {
        RETURN_IF_FAILED(FromJson(*findResult.Payload(), output));
    }
    return S_OK;
}

HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, String& output)
{
    auto findResult = Detail::ObjectFindMember(jsonObject, name);
    RETURN_IF_FAILED(findResult.hr);
    output.clear();
    if (findResult.Payload() != jsonObject.cend())
    {
        RETURN_IF_FAILED(FromJson(*findResult.Payload(), output));
    }
    return S_OK;
}

HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, JsonObject& output)
{
    auto findResult = Detail::ObjectFindMember(jsonObject, name);
    RETURN_IF_FAILED(findResult.hr);
    output.stringValue.clear();
    if (findResult.Payload() != jsonObject.cend())
    {
        RETURN_IF_FAILED(FromJson(*findResult.Payload(), output));
    }
    return S_OK;
}

HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, CStringVector& output)
{
    auto findResult = Detail::ObjectFindMember(jsonObject, name);
    RETURN_IF_FAILED(findResult.hr);
    output.clear();
    if (findResult.Payload() != jsonObject.cend())
    {
        if (!findResult.Payload().value().is_array())
        {
            TRACE_ERROR("Json Parse Error: unexpected token");
            return E_FAIL;
        }

        auto jsonArray{ findResult.Payload()->get<Vector<String>>() };
        output.reserve(jsonArray.size());
        for (const auto& value : jsonArray)
        {
            String stringValue;
            RETURN_IF_FAILED(FromJson(value, stringValue));
            output.push_back(std::move(stringValue));
        }
    }
    return S_OK;
}

HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, StringDictionaryEntryVector& output)
{
    auto findResult = Detail::ObjectFindMember(jsonObject, name);
    RETURN_IF_FAILED(findResult.hr);
    output.clear();
    if (findResult.Payload() != jsonObject.cend())
    {
        if (!findResult.Payload().value().is_object())
        {
            TRACE_ERROR("Json Parse Error: unexpected token");
            return E_FAIL;
        }

        auto memberObject{ findResult.Payload()->get<JsonValue>() };
        output.reserve(memberObject.size());
        for (auto& [key, value] : memberObject.items()) {
            String stringValue{};
            RETURN_IF_FAILED(FromJson(value, stringValue));
            output.insert_or_assign(key.c_str(), std::move(stringValue));
        }
    }
    return S_OK;
}

HRESULT ObjectGetMemberTime(const JsonValue& jsonObject, const char* name, time_t& output)
{
    auto findResult = Detail::ObjectFindMember(jsonObject, name);
    RETURN_IF_FAILED(findResult.hr);
    if (findResult.Payload() == jsonObject.cend())
    {
        TRACE_ERROR("Json Parse Error: object missing non-optional member \"%s\"", name);
        return E_FAIL;
    }
    return FromJsonTime(findResult.Payload().value(), output);
}

HRESULT ObjectGetMemberTime(const JsonValue& jsonObject, const char* name, std::optional<time_t>& output)
{
    auto findResult = Detail::ObjectFindMember(jsonObject, name);
    RETURN_IF_FAILED(findResult.hr);
    output.reset();
    if (findResult.Payload() != jsonObject.cend())
    {
        output.emplace();
        RETURN_IF_FAILED(FromJsonTime(findResult.Payload().value(), *output));
    }
    return S_OK;
}

HRESULT ObjectGetMemberTime(const JsonValue& jsonObject, const char* name, Vector<time_t>& output)
{
    auto findResult = Detail::ObjectFindMember(jsonObject, name);
    RETURN_IF_FAILED(findResult.hr);
    output.clear();
    if (findResult.Payload() != jsonObject.cend())
    {
        if (!findResult.Payload().value().is_array())
        {
            TRACE_ERROR("Json Parse Error: unexpected token");
            return E_FAIL;
        }

        auto jsonArray{ findResult.Payload()->get<Vector<time_t>>() };
        output.reserve(jsonArray.size());
        for (const auto& value : jsonArray)
        {
            output.emplace_back();
            RETURN_IF_FAILED(FromJsonTime(value, output.back()));
        }
    }
    return S_OK;
}

HRESULT ObjectGetMemberTime(const JsonValue& jsonObject, const char* name, DictionaryEntryVector<PFDateTimeDictionaryEntry>& output)
{
    auto findResult = Detail::ObjectFindMember(jsonObject, name);
    RETURN_IF_FAILED(findResult.hr);
    output.clear();
    if (findResult.Payload() != jsonObject.cend())
    {
        if (!findResult.Payload().value().is_object())
        {
            TRACE_ERROR("Json Parse Error: unexpected token");
            return E_FAIL;
        }

        auto memberObject{ findResult.Payload()->get<JsonValue>() };
        output.reserve(memberObject.size());
        for (auto& [key, value] : memberObject.items()) {
            time_t timeValue{};
            RETURN_IF_FAILED(FromJsonTime(value, timeValue));
            output.insert_or_assign(key.c_str(), timeValue);
        }
    }
    return S_OK;
}

namespace Detail
{

Result<JsonValue::const_iterator> ObjectFindMember(const JsonValue& jsonObject, const char* name)
{
    if (!jsonObject.is_object())
    {
        TRACE_ERROR("Json Parse Error: cannot get member from a JsonValue that isn't an object");
        return E_FAIL;
    }
    return jsonObject.find(name);
}

} // namespace Detail
} // namespace JsonUtils
} // namespace PlayFab
