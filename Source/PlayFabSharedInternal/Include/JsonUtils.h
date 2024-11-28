#pragma once

#include <playfab/core/PFTypes.h>
#include <playfab/core/cpp/TypeWrapperHelpers.h>
#include "Nlohmann.h"
#include "EnumTraits.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace JsonUtils
{

// Write JsonValue to String
String WriteToString(const JsonValue& jsonValue);

//------------------------------------------------------------------------------
// Helpers for serializing to JsonValue
//------------------------------------------------------------------------------

JsonValue ToJson(const char* string);

JsonValue ToJson(const String& string);

JsonValue ToJson(const PFJsonObject& jsonObject);

JsonValue ToJsonTime(time_t value);

// Specializations for InputModels
template <typename InternalModelWrapperT, typename std::enable_if_t<std::is_base_of_v<InputModel, InternalModelWrapperT>, bool> = true>
JsonValue ToJson(const typename InternalModelWrapperT::ModelType& value);

JsonValue ToJson(const InputModel& value);

// Specialization for fundamental types
template <typename FundamentalT, typename std::enable_if_t<std::is_fundamental_v<FundamentalT>, bool> = true>
JsonValue ToJson(FundamentalT value);

// Specialization for enum types
template <typename EnumT, typename std::enable_if_t<std::is_enum_v<EnumT>, bool> = true>
JsonValue ToJson(EnumT value);

//------------------------------------------------------------------------------
// Helpers for deserializing from JsonValue
//------------------------------------------------------------------------------
HRESULT FromJson(const JsonValue& input, String& output);

HRESULT FromJson(const JsonValue& input, bool& output);

HRESULT FromJson(const JsonValue& input, int16_t& output);

HRESULT FromJson(const JsonValue& input, uint16_t& output);

HRESULT FromJson(const JsonValue& input, int32_t& output);

HRESULT FromJson(const JsonValue& input, uint32_t& output);

HRESULT FromJson(const JsonValue& input, int64_t& output);

HRESULT FromJson(const JsonValue& input, uint64_t& output);

HRESULT FromJson(const JsonValue& input, float& output);

HRESULT FromJson(const JsonValue& input, double& output);

// Special case needed for time_t since it is an Iso8601 formatted JsonString that gets converted to a time_t
HRESULT FromJsonTime(const JsonValue& input, time_t& output);

HRESULT FromJson(const JsonValue& input, JsonObject& output);

template<typename InternalModelWrapperT, typename std::enable_if_t<std::is_base_of_v<ServiceOutputModel, InternalModelWrapperT>, bool> = true>
HRESULT FromJson(const JsonValue& input, InternalModelWrapperT& output);

template <typename EnumT, typename std::enable_if_t<std::is_enum_v<EnumT>, bool> = true>
HRESULT FromJson(const JsonValue& jsonValue, EnumT& output);

// Deep copies input to output
HRESULT FromJson(const JsonValue& input, JsonValue& output);

//------------------------------------------------------------------------------
// Helpers for adding members JsonObjects. 'value' param will be converted to a JsonValue added as a member to 'jsonObject'.
//------------------------------------------------------------------------------

HRESULT ObjectAddMember(JsonValue& jsonObject, StringRefType name, JsonValue&& value);

HRESULT ObjectAddMember(JsonValue& jsonObject, StringRefType name, const String& value);

template <typename T, typename std::enable_if_t<!std::is_pointer_v<T> || std::is_same_v<T, const char*>, bool> = true>
HRESULT ObjectAddMember(JsonValue& jsonObject, StringRefType name, const T& value);

template <typename T>
HRESULT ObjectAddMember(JsonValue& jsonObject, StringRefType name, const std::optional<T>& value);

template <typename PtrT, typename std::enable_if_t<std::is_pointer_v<PtrT> && !std::is_same_v<PtrT, const char*>, bool> = true>
HRESULT ObjectAddMember(JsonValue& jsonObject, StringRefType name, const PtrT value);

template <typename InternalModelWrapperT, typename std::enable_if_t<std::is_base_of_v<InputModel, InternalModelWrapperT>, bool> = true>
HRESULT ObjectAddMember(JsonValue& jsonObject, StringRefType name, const typename InternalModelWrapperT::ModelType* value);

template <typename T>
HRESULT ObjectAddMemberArray(JsonValue& jsonObject, StringRefType name, const Vector<T>& array);

template <typename T>
HRESULT ObjectAddMemberArray(JsonValue& jsonObject, StringRefType name, const T* array, uint32_t arrayCount);

template <typename InternalModelWrapperT>
HRESULT ObjectAddMemberArray(JsonValue& jsonObject, StringRefType name, const typename InternalModelWrapperT::ModelType* const* array, uint32_t arrayCount);

template <typename ValueT>
HRESULT ObjectAddMemberDictionary(JsonValue& jsonObject, StringRefType name, const Map<String, ValueT>& map);

template <typename EntryT, typename std::enable_if_t<PlayFab::Detail::IsDictionaryEntry<EntryT>::value, bool> = true>
HRESULT ObjectAddMemberDictionary(JsonValue& jsonObject, StringRefType name, const EntryT* associativeArray, uint32_t arrayCount);

template <typename InternalModelWrapperT, typename std::enable_if_t<std::is_base_of_v<InputModel, InternalModelWrapperT>, bool> = true>
HRESULT ObjectAddMemberDictionary(JsonValue& jsonObject, StringRefType name, const typename InternalModelWrapperT::DictionaryEntryType* associativeArray, uint32_t arrayCount);

HRESULT ObjectAddMemberTime(JsonValue& jsonObject, StringRefType name, time_t value);

HRESULT ObjectAddMemberTime(JsonValue& jsonObject, StringRefType name, const std::optional<time_t>& value);

HRESULT ObjectAddMemberTime(JsonValue& jsonObject, StringRefType name, const time_t* value);

HRESULT ObjectAddMemberTime(JsonValue& jsonObject, StringRefType name, const time_t* array, uint32_t arrayCount);

HRESULT ObjectAddMemberTime(JsonValue& jsonObject, StringRefType name, const PFDateTimeDictionaryEntry* associativeArray, uint32_t arrayCount);

//------------------------------------------------------------------------------
// Helpers for getting fields from JsonObjects as Cpp types.
// Unless the output field is optional, nlohmann will assert if the JsonObject is missing the requested
// field or if the requested fields are not the expected Json type.
//
// Some of the methods also have output parameters where a pointer to the retreived value & (in the case of collections) the count is returned.
//------------------------------------------------------------------------------

template <typename T>
HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, T& output);

template <typename T>
HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, std::optional<T>& output);

// Because they are already nullable, JsonObjects, Strings, and collection types will not be wrapped in StdExtra::optional even if the properties are optional.
// The default templated ObjectGetMember assumes the property is required, these specializations will not.
HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, JsonValue& output);

HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, JsonObject& output);

HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, String& output);

template <typename T>
HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, Vector<T>& output);

template <typename InternalModelWrapperT, typename std::enable_if_t<std::is_base_of_v<ServiceOutputModel, InternalModelWrapperT>, bool> = true>
HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, ModelVector<InternalModelWrapperT>& output);

HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, CStringVector& output);

template<class EntryT>
HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, DictionaryEntryVector<EntryT>& output);

template<class InternalModelWrapperT, typename std::enable_if_t<std::is_base_of_v<ServiceOutputModel, InternalModelWrapperT>, bool> = true>
HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, ModelDictionaryEntryVector<InternalModelWrapperT>& output);

HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, StringDictionaryEntryVector& output);

HRESULT ObjectGetMemberTime(const JsonValue& jsonObject, const char* name, time_t& output);

HRESULT ObjectGetMemberTime(const JsonValue& jsonObject, const char* name, std::optional<time_t>& output);

HRESULT ObjectGetMemberTime(const JsonValue& jsonObject, const char* name, Vector<time_t>& output);

HRESULT ObjectGetMemberTime(const JsonValue& jsonObject, const char* name, DictionaryEntryVector<PFDateTimeDictionaryEntry>& output);

namespace Detail
{
// Helper method for ObjectGetMember methods. Basically JsonValue::FindMember with type validation
Result<JsonValue::const_iterator> ObjectFindMember(const JsonValue& jsonObject, const char* name);
}

//------------------------------------------------------------------------------
// Template implementations
//------------------------------------------------------------------------------

template <typename InternalModelWrapperT, typename std::enable_if_t<std::is_base_of_v<InputModel, InternalModelWrapperT>, bool>>
JsonValue ToJson(const typename InternalModelWrapperT::ModelType& value)
{
    return InternalModelWrapperT::ToJson(value);
}

template <typename FundamentalT, typename std::enable_if_t<std::is_fundamental_v<FundamentalT>, bool>>
JsonValue ToJson(FundamentalT value)
{
    return value;
}

template <typename EnumT, typename std::enable_if_t<std::is_enum_v<EnumT>, bool>>
inline JsonValue ToJson(EnumT value)
{
    return EnumName<EnumT>(value);
}

template<typename InternalModelWrapperT, typename std::enable_if_t<std::is_base_of_v<ServiceOutputModel, InternalModelWrapperT>, bool>>
HRESULT FromJson(const JsonValue& input, InternalModelWrapperT& output)
{
    return output.FromJson(input);
}

template <typename EnumT, typename std::enable_if_t<std::is_enum_v<EnumT>, bool>>
HRESULT FromJson(const JsonValue& jsonValue, EnumT& output)
{
    if (!jsonValue.is_string())
    {
        TRACE_ERROR("Json Parse Error: unexpected token");
        return E_FAIL;
    }
    output = EnumValue<EnumT>(jsonValue.get<String>().c_str());
    return S_OK;
}

template <typename T, typename std::enable_if_t<!std::is_pointer_v<T> || std::is_same_v<T, const char*>, bool>>
HRESULT ObjectAddMember(JsonValue& jsonObject, StringRefType name, const T& value)
{
    return ObjectAddMember(jsonObject, name, ToJson(value));
}

template <typename T>
HRESULT ObjectAddMember(JsonValue& jsonObject, StringRefType name, const std::optional<T>& value)
{
    if (value.has_value())
    {
        return ObjectAddMember(jsonObject, name, *value);
    }
    else
    {
        return ObjectAddMember(jsonObject, name, JsonValue{});
    }
}

template <typename PtrT, typename std::enable_if_t<std::is_pointer_v<PtrT> && !std::is_same_v<PtrT, const char*>, bool>>
HRESULT ObjectAddMember(JsonValue& jsonObject, StringRefType name, const PtrT value)
{
    if (value)
    {
        return ObjectAddMember(jsonObject, name, *value);
    }
    else
    {
        return ObjectAddMember(jsonObject, name, JsonValue{});
    }
}

template <typename InternalModelWrapperT, typename std::enable_if_t<std::is_base_of_v<InputModel, InternalModelWrapperT>, bool>>
HRESULT ObjectAddMember(JsonValue& jsonObject, StringRefType name, const typename InternalModelWrapperT::ModelType* value)
{
    if (value)
    {
        return ObjectAddMember(jsonObject, name, ToJson<InternalModelWrapperT>(*value));
    }
    else
    {
        return ObjectAddMember(jsonObject, name, JsonValue{});
    }
}

template <typename T>
HRESULT ObjectAddMemberArray(JsonValue& jsonObject, StringRefType name, const Vector<T>& array)
{
    JsonValue member{ JsonValue::array() };
    for (auto& item : array)
    {
        member.push_back(ToJson(item));
    }
    return ObjectAddMember(jsonObject, name, std::move(member));
}

template <typename T>
HRESULT ObjectAddMemberArray(JsonValue& jsonObject, StringRefType name, const T* array, uint32_t arrayCount)
{
    JsonValue member{ JsonValue::array() };
    for (auto i = 0u; i < arrayCount; ++i)
    {
        member.push_back(ToJson(array[i]));
    }
    return ObjectAddMember(jsonObject, name, std::move(member));
}

template <typename InternalModelWrapperT>
HRESULT ObjectAddMemberArray(JsonValue& jsonObject, StringRefType name, const typename InternalModelWrapperT::ModelType* const* array, uint32_t arrayCount)
{
    JsonValue member{ JsonValue::array() };
    for (auto i = 0u; i < arrayCount; ++i)
    {
        member.push_back(ToJson<InternalModelWrapperT>(*array[i]));
    }
    return ObjectAddMember(jsonObject, name, std::move(member));
}

template <typename ValueT>
HRESULT ObjectAddMemberDictionary(JsonValue& jsonObject, StringRefType name, const Map<String, ValueT>& map)
{
    JsonValue member{ JsonValue::object() };

    for (auto& pair : map)
    {
        RETURN_IF_FAILED(ObjectAddMember(member, String(ToJson(pair.first)), ToJson(pair.second)));
    }
    return ObjectAddMember(jsonObject, name, std::move(member));
}

template <typename EntryT, typename std::enable_if_t<PlayFab::Detail::IsDictionaryEntry<EntryT>::value, bool>>
HRESULT ObjectAddMemberDictionary(JsonValue& jsonObject, StringRefType name, const EntryT* associativeArray, uint32_t arrayCount)
{
    JsonValue member{ JsonValue::object() };
    for (auto i = 0u; i < arrayCount; ++i)
    {
        auto& entry{ associativeArray[i] };
        RETURN_IF_FAILED(ObjectAddMember(member, String(ToJson(entry.key)), ToJson(entry.value)));
    }
    return ObjectAddMember(jsonObject, name, std::move(member));
}

template <typename InternalModelWrapperT, typename std::enable_if_t<std::is_base_of_v<InputModel, InternalModelWrapperT>, bool>>
HRESULT ObjectAddMemberDictionary(JsonValue& jsonObject, StringRefType name, const typename InternalModelWrapperT::DictionaryEntryType* associativeArray, uint32_t arrayCount)
{
    JsonValue member{ JsonValue::object() };
    for (auto i = 0u; i < arrayCount; ++i)
    {
        auto& entry{ associativeArray[i] };
        RETURN_IF_FAILED(ObjectAddMember(member, String(ToJson(entry.key)), ToJson<InternalModelWrapperT>(*entry.value)));
    }
    return ObjectAddMember(jsonObject, name, std::move(member));
}

template <typename T>
HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, T& output)
{
    auto findResult = Detail::ObjectFindMember(jsonObject, name);
    RETURN_IF_FAILED(findResult.hr);
    if (findResult.Payload() == jsonObject.cend())
    {
        TRACE_ERROR("Json Parse Error: object missing non-optional member \"%s\"", name);
        return E_FAIL;
    }
    return FromJson(findResult.Payload().value(), output);
}

template <typename T>
HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, std::optional<T>& output)
{
    auto findResult = Detail::ObjectFindMember(jsonObject, name);
    RETURN_IF_FAILED(findResult.hr);
    output.reset();
    if (findResult.Payload() != jsonObject.cend())
    {
        output.emplace();
        RETURN_IF_FAILED(FromJson(findResult.Payload().value(), *output));
    }
    return S_OK;
}

template <typename T>
HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, Vector<T>& output)
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

        auto jsonArray{ findResult.Payload()->get<Vector<JsonValue>>() };
        output.reserve(jsonArray.size());
        for (auto& value : jsonArray)
        {
            output.emplace_back();
            RETURN_IF_FAILED(FromJson(value, output.back()));
        }
    }
    return S_OK;
}

template <typename InternalModelWrapperT, typename std::enable_if_t<std::is_base_of_v<ServiceOutputModel, InternalModelWrapperT>, bool>>
HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, ModelVector<InternalModelWrapperT>& output)
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

        auto jsonArray{ findResult.Payload()->get<Vector<JsonValue>>() };
        output.reserve(jsonArray.size());

        for (const auto& value : jsonArray)
        {
            if (!value.is_object())
            {
                TRACE_ERROR("Json Parse Error: unexpected token");
                return E_FAIL;
            }

            InternalModelWrapperT model{};
            RETURN_IF_FAILED(model.FromJson(value));
            output.push_back(std::move(model));
        }
    }
    return S_OK;
}

template<class EntryT>
HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, DictionaryEntryVector<EntryT>& output)
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
        for (const auto& [key, value] : memberObject.items())
        {
            decltype(EntryT::value) entryValue{};
            RETURN_IF_FAILED(FromJson(value, entryValue));
            output.insert_or_assign(key.c_str(), entryValue);
        }
    }
    return S_OK;
}

template<class InternalModelWrapperT, typename std::enable_if_t<std::is_base_of_v<ServiceOutputModel, InternalModelWrapperT>, bool>>
HRESULT ObjectGetMember(const JsonValue& jsonObject, const char* name, ModelDictionaryEntryVector<InternalModelWrapperT>& output)
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
        for (const auto& [key, value] : memberObject.items())
        {
            InternalModelWrapperT model{};
            RETURN_IF_FAILED(model.FromJson(value));
            output.insert_or_assign(key.c_str(), std::move(model));
        }
    }
    return S_OK;
}

} // namespace JsonUtils
} // namespace PlayFab
