#pragma once

#include <playfab/core/cpp/TypeWrapperHelpers.h>
#include "ModelBuffer.h"

namespace PlayFab
{

using Wrappers::SafeString;

// Hooked public wrapper types
using JsonObject = Wrappers::JsonObject<Allocator>;

template<typename InternalModelWrapperT>
using ModelVector = Wrappers::ModelVector<typename InternalModelWrapperT::ModelWrapperType, Allocator>;

using CStringVector = Wrappers::CStringVector<Allocator>;

template<typename EntryT>
using DictionaryEntryVector = Wrappers::DictionaryEntryVector<EntryT, Allocator>;

template<class InternalModelWrapperT>
using ModelDictionaryEntryVector = Wrappers::ModelDictionaryEntryVector<typename InternalModelWrapperT::ModelWrapperType, Allocator>;

using StringDictionaryEntryVector = Wrappers::StringDictionaryEntryVector<Allocator>;

// Base class from all Model Wrappers that are input arguments to PlayFab APIs
class InputModel
{
public:
    virtual ~InputModel() = default;
    virtual JsonValue ToJson() const = 0;
};

// Base class from all Models that are returned from PlayFab service APIs
class ServiceOutputModel
{
public:
    virtual ~ServiceOutputModel() = default;
    virtual HRESULT FromJson(const JsonValue& input) = 0;
};

// Base class for all Model Wrappers that are returned from public client APIs
template<typename ModelT>
class ClientOutputModel
{
public:
    using ModelType = ModelT;

    virtual ~ClientOutputModel() = default;
    virtual size_t RequiredBufferSize() const = 0;
    virtual Result<ModelT const*> Copy(ModelBuffer& buffer) const = 0;
};

} // namespace PlayFab
