#pragma once

#include <playfab/core/cpp/Entity.h>
#include <playfab/core/cpp/ServiceConfig.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
using Entity = Wrappers::Entity<Allocator>;
using ServiceConfig = Wrappers::ServiceConfig<Allocator>;

// Shared Classes
class VersionConfiguration : public Wrappers::PFVersionConfigurationWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFVersionConfiguration>
{
public:
    using ModelWrapperType = typename Wrappers::PFVersionConfigurationWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFVersionConfiguration& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFVersionConfiguration const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFVersionConfiguration& model);
    static HRESULT Copy(const PFVersionConfiguration& input, PFVersionConfiguration& output, ModelBuffer& buffer);
};


// Json serialization helpers

// EnumRange definitions used for Enum (de)serialization
template<typename T> struct EnumRange;

template<> struct EnumRange<PFOperationTypes>
{
    static constexpr PFOperationTypes maxValue = PFOperationTypes::None;
};

template<> struct EnumRange<PFResetInterval>
{
    static constexpr PFResetInterval maxValue = PFResetInterval::Month;
};


} // namespace PlayFab
