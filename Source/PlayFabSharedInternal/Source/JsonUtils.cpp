#include "pch.h"
#include "JsonUtils.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace JsonUtils
{

JsonAllocator allocator{};

// Helper class for writing JsonValue directly into a PlayFab::String. Implements rapidjson write-only stream
// concept (see rapidjson/stream.h for details). Avoids additional copy needed when first writing to rapidjson::StringBuffer
class StringOutputStream
{
public:
    using Ch = String::traits_type::char_type;

    StringOutputStream(size_t initialCapacity = kDefaultCapacity)
    {
        m_string.reserve(initialCapacity);
    }

    void Put(Ch c)
    {
        m_string.push_back(c);
    }

    void Flush()
    {
        // no-op
    }

    const String& GetString() const
    {
        return m_string;
    }

    String&& ExtractString()
    {
        return std::move(m_string);
    }

    static const size_t kDefaultCapacity = 256; // use same default capacity as rapidjson::GenericStringBuffer

private:
    String m_string;
};

String WriteToString(const JsonValue& jsonValue)
{
    StringOutputStream stream;
    JsonWriter<StringOutputStream> writer{ stream, &allocator };
    jsonValue.Accept(writer);
    return stream.ExtractString();
}

JsonValue ToJson(const char* string)
{
    if (!string)
    {
        return JsonValue{ rapidjson::kNullType };
    }
    return JsonValue{ string, allocator };
}

JsonValue ToJson(const String& string)
{
    // By design, map empty string to null JsonValue
    if (string.empty())
    {
        return JsonValue{ rapidjson::kNullType };
    }
    return JsonValue{ string.data(), allocator };
}

JsonValue ToJson(const PFJsonObject& jsonObject)
{
    // By design, map empty jsonObject to null JsonValue
    if (!jsonObject.stringValue)
    {
        return JsonValue{ rapidjson::kNullType };
    }
    JsonDocument document{ &JsonUtils::allocator, JsonUtils::kDefaultStackCapacity, &JsonUtils::allocator };
    document.Parse(jsonObject.stringValue);
    return JsonValue{ document, allocator };
}

JsonValue ToJsonTime(time_t value)
{
    return JsonValue{ TimeTToIso8601String(value).data(), allocator };
}

JsonValue ToJson(const InputModel& value)
{
    return value.ToJson();
}

HRESULT FromJson(const JsonValue& input, String& output)
{
    if (!input.IsString())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = input.GetString();
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, bool& output)
{
    if (!input.IsBool())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = input.GetBool();
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, int16_t& output)
{
    if (!input.IsInt())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = static_cast<int16_t>(input.GetInt());
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, uint16_t& output)
{
    if (!input.IsUint())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = static_cast<uint16_t>(input.GetUint());
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, int32_t& output)
{
    if (!input.IsInt())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = static_cast<int32_t>(input.GetInt());
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, uint32_t& output)
{
    if (!input.IsUint())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = static_cast<uint32_t>(input.GetUint());
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, int64_t& output)
{
    if (!input.IsInt64())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = input.GetInt64();
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, uint64_t& output)
{
    if (!input.IsUint64())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = input.GetUint64();
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, float& output)
{
    if (!input.IsFloat())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = input.GetFloat();
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, double& output)
{
    if (!input.IsDouble())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = input.GetDouble();
    return S_OK;
}

HRESULT FromJsonTime(const JsonValue& input, time_t& output)
{
    if (!input.IsString())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = Iso8601StringToTimeT(input.GetString());
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, JsonObject& output)
{
    if (!input.IsObject())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output.stringValue = JsonUtils::WriteToString(input);
    return S_OK;
}

HRESULT FromJson(const JsonValue& input, JsonValue& output)
{
    output.CopyFrom(input, allocator); // Deep copy
    return S_OK;
}

HRESULT ObjectAddMember(JsonValue& jsonObject, JsonValue&& name, JsonValue&& value)
{
    if (!jsonObject.IsObject())
    {
        TRACE_ERROR("Json Error: cannot add member to non-object");
        return E_FAIL;
    }
    jsonObject.AddMember(name, value, allocator);
    return S_OK;
}

HRESULT ObjectAddMember(JsonValue& jsonObject, JsonValue::StringRefType name, JsonValue&& value)
{
    return ObjectAddMember(jsonObject, JsonValue{ name }, std::move(value));
}

HRESULT ObjectAddMember(JsonValue& jsonObject, JsonValue::StringRefType name, const JsonValue& value)
{
    return ObjectAddMember(jsonObject, name, std::move(JsonValue{}.CopyFrom(value, allocator)));
}

HRESULT ObjectAddMember(JsonValue& jsonObject, JsonValue::StringRefType name, const String& value)
{
    return ObjectAddMember(jsonObject, name, ToJson(value));
}

HRESULT ObjectAddMemberTime(JsonValue& jsonObject, JsonValue::StringRefType name, time_t value)
{
    return ObjectAddMember(jsonObject, name, ToJsonTime(value));
}

HRESULT ObjectAddMemberTime(JsonValue& jsonObject, JsonValue::StringRefType name, const std::optional<time_t>& value)
{
    if (value.has_value())
    {
        return ObjectAddMember(jsonObject, name, ToJsonTime(*value));
    }
    else
    {
        return ObjectAddMember(jsonObject, name, JsonValue{ rapidjson::kNullType });
    }
}

HRESULT ObjectAddMemberTime(JsonValue& jsonObject, JsonValue::StringRefType name, const time_t* value)
{
    if (value != nullptr)
    {
        return ObjectAddMember(jsonObject, name, ToJsonTime(*value));
    }
    else
    {
        return ObjectAddMember(jsonObject, name, JsonValue{ rapidjson::kNullType });
    }
}

HRESULT ObjectAddMemberTime(JsonValue& jsonObject, JsonValue::StringRefType name, const time_t* array, uint32_t arrayCount)
{
    JsonValue member{ rapidjson::kArrayType };
    for (auto i = 0u; i < arrayCount; ++i)
    {
        member.PushBack(ToJsonTime(array[i]), allocator);
    }
    return ObjectAddMember(jsonObject, name, std::move(member));
}

HRESULT ObjectAddMemberTime(JsonValue& jsonObject, JsonValue::StringRefType name, const PFDateTimeDictionaryEntry* associativeArray, uint32_t arrayCount)
{
    JsonValue member{ rapidjson::kObjectType };
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
    output.SetNull();
    if (findResult.Payload() != jsonObject.MemberEnd())
    {
        RETURN_IF_FAILED(FromJson(findResult.Payload()->value, output));
    }
    return S_OK;
}

HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, String& output)
{
    auto findResult = Detail::ObjectFindMember(jsonObject, name);
    RETURN_IF_FAILED(findResult.hr);
    output.clear();
    if (findResult.Payload() != jsonObject.MemberEnd())
    {
        RETURN_IF_FAILED(FromJson(findResult.Payload()->value, output));
    }
    return S_OK;
}

HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, JsonObject& output)
{
    auto findResult = Detail::ObjectFindMember(jsonObject, name);
    RETURN_IF_FAILED(findResult.hr);
    output.stringValue.clear();
    if (findResult.Payload() != jsonObject.MemberEnd())
    {
        RETURN_IF_FAILED(FromJson(findResult.Payload()->value, output));
    }
    return S_OK;
}

HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, CStringVector& output)
{
    auto findResult = Detail::ObjectFindMember(jsonObject, name);
    RETURN_IF_FAILED(findResult.hr);
    output.clear();
    if (findResult.Payload() != jsonObject.MemberEnd())
    {
        if (!findResult.Payload()->value.IsArray())
        {
            TRACE_ERROR("Json Parse Error: unexpected token");
            return E_FAIL;
        }

        auto jsonArray{ findResult.Payload()->value.GetArray() };
        output.reserve(jsonArray.Size());
        for (auto& value : jsonArray)
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
    if (findResult.Payload() != jsonObject.MemberEnd())
    {
        if (!findResult.Payload()->value.IsObject())
        {
            TRACE_ERROR("Json Parse Error: unexpected token");
            return E_FAIL;
        }

        auto memberObject{ findResult.Payload()->value.GetObj()};
        output.reserve(memberObject.MemberCount());
        for (auto& pair : memberObject)
        {
            String stringValue{};
            RETURN_IF_FAILED(FromJson(pair.value, stringValue));
            output.insert_or_assign(pair.name.GetString(), std::move(stringValue));
        }
    }
    return S_OK;
}

HRESULT ObjectGetMemberTime(const JsonValue& jsonObject, const char* name, time_t& output)
{
    auto findResult = Detail::ObjectFindMember(jsonObject, name);
    RETURN_IF_FAILED(findResult.hr);
    if (findResult.Payload() == jsonObject.MemberEnd())
    {
        TRACE_ERROR("Json Parse Error: object missing non-optional member \"%s\"", name);
        return E_FAIL;
    }
    return FromJsonTime(findResult.Payload()->value, output);
}

HRESULT ObjectGetMemberTime(const JsonValue& jsonObject, const char* name, std::optional<time_t>& output)
{
    auto findResult = Detail::ObjectFindMember(jsonObject, name);
    RETURN_IF_FAILED(findResult.hr);
    output.reset();
    if (findResult.Payload() != jsonObject.MemberEnd())
    {
        output.emplace();
        RETURN_IF_FAILED(FromJsonTime(findResult.Payload()->value, *output));
    }
    return S_OK;
}

HRESULT ObjectGetMemberTime(const JsonValue& jsonObject, const char* name, Vector<time_t>& output)
{
    auto findResult = Detail::ObjectFindMember(jsonObject, name);
    RETURN_IF_FAILED(findResult.hr);
    output.clear();
    if (findResult.Payload() != jsonObject.MemberEnd())
    {
        if (!findResult.Payload()->value.IsArray())
        {
            TRACE_ERROR("Json Parse Error: unexpected token");
            return E_FAIL;
        }

        auto jsonArray{ findResult.Payload()->value.GetArray() };
        output.reserve(jsonArray.Size());
        for (auto& value : jsonArray)
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
    if (findResult.Payload() != jsonObject.MemberEnd())
    {
        if (!findResult.Payload()->value.IsObject())
        {
            TRACE_ERROR("Json Parse Error: unexpected token");
            return E_FAIL;
        }

        auto memberObject{ findResult.Payload()->value.GetObj() };
        output.reserve(memberObject.MemberCount());
        for (auto& pair : memberObject)
        {
            time_t value{};
            RETURN_IF_FAILED(FromJsonTime(pair.value, value));
            output.insert_or_assign(pair.name.GetString(), value);
        }
    }
    return S_OK;
}

namespace Detail
{

Result<JsonValue::ConstMemberIterator> ObjectFindMember(const JsonValue& jsonObject, const char* name)
{
    if (!jsonObject.IsObject())
    {
        TRACE_ERROR("Json Parse Error: cannot get member from a JsonValue that isn't an object");
        return E_FAIL;
    }
    return jsonObject.FindMember(name);
}

} // namespace Detail
} // namespace JsonUtils
} // namespace PlayFab
