#pragma once

#include <playfab/services/cpp/StatisticsTypeWrappers.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace Statistics
{

// Statistics Classes
class StatisticColumn : public Wrappers::PFStatisticsStatisticColumnWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFStatisticsStatisticColumn>
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsStatisticColumnWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFStatisticsStatisticColumn& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFStatisticsStatisticColumn const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFStatisticsStatisticColumn& model);
    static HRESULT Copy(const PFStatisticsStatisticColumn& input, PFStatisticsStatisticColumn& output, ModelBuffer& buffer);
};

class CreateStatisticDefinitionRequest : public Wrappers::PFStatisticsCreateStatisticDefinitionRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsCreateStatisticDefinitionRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFStatisticsCreateStatisticDefinitionRequest& input);
};

class DeleteStatisticDefinitionRequest : public Wrappers::PFStatisticsDeleteStatisticDefinitionRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsDeleteStatisticDefinitionRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFStatisticsDeleteStatisticDefinitionRequest& input);
};

class StatisticDelete : public Wrappers::PFStatisticsStatisticDeleteWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsStatisticDeleteWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFStatisticsStatisticDelete& input);
};

class DeleteStatisticsRequest : public Wrappers::PFStatisticsDeleteStatisticsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsDeleteStatisticsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFStatisticsDeleteStatisticsRequest& input);
};

class DeleteStatisticsResponse : public Wrappers::PFStatisticsDeleteStatisticsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFStatisticsDeleteStatisticsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsDeleteStatisticsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFStatisticsDeleteStatisticsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFStatisticsDeleteStatisticsResponse& model);
    static HRESULT Copy(const PFStatisticsDeleteStatisticsResponse& input, PFStatisticsDeleteStatisticsResponse& output, ModelBuffer& buffer);
};

class GetStatisticDefinitionRequest : public Wrappers::PFStatisticsGetStatisticDefinitionRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsGetStatisticDefinitionRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFStatisticsGetStatisticDefinitionRequest& input);
};

class GetStatisticDefinitionResponse : public Wrappers::PFStatisticsGetStatisticDefinitionResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFStatisticsGetStatisticDefinitionResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsGetStatisticDefinitionResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFStatisticsGetStatisticDefinitionResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFStatisticsGetStatisticDefinitionResponse& model);
    static HRESULT Copy(const PFStatisticsGetStatisticDefinitionResponse& input, PFStatisticsGetStatisticDefinitionResponse& output, ModelBuffer& buffer);
};

class GetStatisticsRequest : public Wrappers::PFStatisticsGetStatisticsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsGetStatisticsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFStatisticsGetStatisticsRequest& input);
};

class StatisticColumnCollection : public Wrappers::PFStatisticsStatisticColumnCollectionWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFStatisticsStatisticColumnCollection>
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsStatisticColumnCollectionWrapper<Allocator>;
    using ModelWrapperType::ModelType;
    using DictionaryEntryType = ModelWrapperType::DictionaryEntryType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFStatisticsStatisticColumnCollection const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFStatisticsStatisticColumnCollection& model);
    static HRESULT Copy(const PFStatisticsStatisticColumnCollection& input, PFStatisticsStatisticColumnCollection& output, ModelBuffer& buffer);
};

class EntityStatisticValue : public Wrappers::PFStatisticsEntityStatisticValueWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFStatisticsEntityStatisticValue>
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsEntityStatisticValueWrapper<Allocator>;
    using ModelWrapperType::ModelType;
    using DictionaryEntryType = ModelWrapperType::DictionaryEntryType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFStatisticsEntityStatisticValue const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFStatisticsEntityStatisticValue& model);
    static HRESULT Copy(const PFStatisticsEntityStatisticValue& input, PFStatisticsEntityStatisticValue& output, ModelBuffer& buffer);
};

class GetStatisticsResponse : public Wrappers::PFStatisticsGetStatisticsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFStatisticsGetStatisticsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsGetStatisticsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFStatisticsGetStatisticsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFStatisticsGetStatisticsResponse& model);
    static HRESULT Copy(const PFStatisticsGetStatisticsResponse& input, PFStatisticsGetStatisticsResponse& output, ModelBuffer& buffer);
};

class GetStatisticsForEntitiesRequest : public Wrappers::PFStatisticsGetStatisticsForEntitiesRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsGetStatisticsForEntitiesRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFStatisticsGetStatisticsForEntitiesRequest& input);
};

class EntityStatistics : public Wrappers::PFStatisticsEntityStatisticsWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFStatisticsEntityStatistics>
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsEntityStatisticsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFStatisticsEntityStatistics const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFStatisticsEntityStatistics& model);
    static HRESULT Copy(const PFStatisticsEntityStatistics& input, PFStatisticsEntityStatistics& output, ModelBuffer& buffer);
};

class GetStatisticsForEntitiesResponse : public Wrappers::PFStatisticsGetStatisticsForEntitiesResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFStatisticsGetStatisticsForEntitiesResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsGetStatisticsForEntitiesResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFStatisticsGetStatisticsForEntitiesResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFStatisticsGetStatisticsForEntitiesResponse& model);
    static HRESULT Copy(const PFStatisticsGetStatisticsForEntitiesResponse& input, PFStatisticsGetStatisticsForEntitiesResponse& output, ModelBuffer& buffer);
};

class IncrementStatisticVersionRequest : public Wrappers::PFStatisticsIncrementStatisticVersionRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsIncrementStatisticVersionRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFStatisticsIncrementStatisticVersionRequest& input);
};

class IncrementStatisticVersionResponse : public Wrappers::PFStatisticsIncrementStatisticVersionResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFStatisticsIncrementStatisticVersionResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsIncrementStatisticVersionResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFStatisticsIncrementStatisticVersionResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFStatisticsIncrementStatisticVersionResponse& model);
    static HRESULT Copy(const PFStatisticsIncrementStatisticVersionResponse& input, PFStatisticsIncrementStatisticVersionResponse& output, ModelBuffer& buffer);
};

class ListStatisticDefinitionsRequest : public Wrappers::PFStatisticsListStatisticDefinitionsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsListStatisticDefinitionsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFStatisticsListStatisticDefinitionsRequest& input);
};

class StatisticDefinition : public Wrappers::PFStatisticsStatisticDefinitionWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFStatisticsStatisticDefinition>
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsStatisticDefinitionWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFStatisticsStatisticDefinition const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFStatisticsStatisticDefinition& model);
    static HRESULT Copy(const PFStatisticsStatisticDefinition& input, PFStatisticsStatisticDefinition& output, ModelBuffer& buffer);
};

class ListStatisticDefinitionsResponse : public Wrappers::PFStatisticsListStatisticDefinitionsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFStatisticsListStatisticDefinitionsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsListStatisticDefinitionsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFStatisticsListStatisticDefinitionsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFStatisticsListStatisticDefinitionsResponse& model);
    static HRESULT Copy(const PFStatisticsListStatisticDefinitionsResponse& input, PFStatisticsListStatisticDefinitionsResponse& output, ModelBuffer& buffer);
};

class StatisticUpdate : public Wrappers::PFStatisticsStatisticUpdateWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsStatisticUpdateWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFStatisticsStatisticUpdate& input);
};

class UpdateStatisticsRequest : public Wrappers::PFStatisticsUpdateStatisticsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsUpdateStatisticsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFStatisticsUpdateStatisticsRequest& input);
};

class UpdateStatisticsResponse : public Wrappers::PFStatisticsUpdateStatisticsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFStatisticsUpdateStatisticsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFStatisticsUpdateStatisticsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFStatisticsUpdateStatisticsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFStatisticsUpdateStatisticsResponse& model);
    static HRESULT Copy(const PFStatisticsUpdateStatisticsResponse& input, PFStatisticsUpdateStatisticsResponse& output, ModelBuffer& buffer);
};

} // namespace Statistics

// Json serialization helpers

// EnumRange definitions used for Enum (de)serialization
template<typename T> struct EnumRange;

template<> struct EnumRange<PFStatisticsStatisticAggregationMethod>
{
    static constexpr PFStatisticsStatisticAggregationMethod maxValue = PFStatisticsStatisticAggregationMethod::Sum;
};


} // namespace PlayFab
