#pragma once

#include <playfab/core/cpp/TypeWrappers.h>

namespace PlayFab
{

/// <summary>
/// OperationTypes enum.
/// </summary>
enum class PFOperationTypes : uint32_t
{
    Created,
    Updated,
    Deleted,
    None
};

// EnumRange definitions used for Enum (de)serialization
template<typename T> struct EnumRange;

template<> struct EnumRange<PFOperationTypes>
{
    static constexpr PFOperationTypes maxValue = PFOperationTypes::None;
};

class EntityKey : public Wrappers::PFEntityKeyWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFEntityKey>
{
public:
    using ModelWrapperType = typename Wrappers::PFEntityKeyWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEntityKey& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEntityKey const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEntityKey& model);
    static HRESULT Copy(const PFEntityKey& input, PFEntityKey& output, ModelBuffer& buffer);
};

} // namespace PlayFab
