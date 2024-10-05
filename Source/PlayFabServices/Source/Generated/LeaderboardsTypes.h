#pragma once

#include <playfab/services/cpp/LeaderboardsTypeWrappers.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace Leaderboards
{

// Leaderboards Classes
class LinkedStatisticColumn : public Wrappers::PFLeaderboardsLinkedStatisticColumnWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFLeaderboardsLinkedStatisticColumn>
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsLinkedStatisticColumnWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFLeaderboardsLinkedStatisticColumn& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFLeaderboardsLinkedStatisticColumn const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFLeaderboardsLinkedStatisticColumn& model);
    static HRESULT Copy(const PFLeaderboardsLinkedStatisticColumn& input, PFLeaderboardsLinkedStatisticColumn& output, ModelBuffer& buffer);
};

class LeaderboardColumn : public Wrappers::PFLeaderboardsLeaderboardColumnWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFLeaderboardsLeaderboardColumn>
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsLeaderboardColumnWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFLeaderboardsLeaderboardColumn& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFLeaderboardsLeaderboardColumn const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFLeaderboardsLeaderboardColumn& model);
    static HRESULT Copy(const PFLeaderboardsLeaderboardColumn& input, PFLeaderboardsLeaderboardColumn& output, ModelBuffer& buffer);
};

class CreateLeaderboardDefinitionRequest : public Wrappers::PFLeaderboardsCreateLeaderboardDefinitionRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsCreateLeaderboardDefinitionRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFLeaderboardsCreateLeaderboardDefinitionRequest& input);
};

class DeleteLeaderboardDefinitionRequest : public Wrappers::PFLeaderboardsDeleteLeaderboardDefinitionRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsDeleteLeaderboardDefinitionRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFLeaderboardsDeleteLeaderboardDefinitionRequest& input);
};

class DeleteLeaderboardEntriesRequest : public Wrappers::PFLeaderboardsDeleteLeaderboardEntriesRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsDeleteLeaderboardEntriesRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFLeaderboardsDeleteLeaderboardEntriesRequest& input);
};

class GetFriendLeaderboardForEntityRequest : public Wrappers::PFLeaderboardsGetFriendLeaderboardForEntityRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsGetFriendLeaderboardForEntityRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFLeaderboardsGetFriendLeaderboardForEntityRequest& input);
};

class EntityLeaderboardEntry : public Wrappers::PFLeaderboardsEntityLeaderboardEntryWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFLeaderboardsEntityLeaderboardEntry>
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsEntityLeaderboardEntryWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFLeaderboardsEntityLeaderboardEntry const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFLeaderboardsEntityLeaderboardEntry& model);
    static HRESULT Copy(const PFLeaderboardsEntityLeaderboardEntry& input, PFLeaderboardsEntityLeaderboardEntry& output, ModelBuffer& buffer);
};

class GetEntityLeaderboardResponse : public Wrappers::PFLeaderboardsGetEntityLeaderboardResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFLeaderboardsGetEntityLeaderboardResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsGetEntityLeaderboardResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFLeaderboardsGetEntityLeaderboardResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFLeaderboardsGetEntityLeaderboardResponse& model);
    static HRESULT Copy(const PFLeaderboardsGetEntityLeaderboardResponse& input, PFLeaderboardsGetEntityLeaderboardResponse& output, ModelBuffer& buffer);
};

class GetEntityLeaderboardRequest : public Wrappers::PFLeaderboardsGetEntityLeaderboardRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsGetEntityLeaderboardRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFLeaderboardsGetEntityLeaderboardRequest& input);
};

class GetLeaderboardAroundEntityRequest : public Wrappers::PFLeaderboardsGetLeaderboardAroundEntityRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsGetLeaderboardAroundEntityRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFLeaderboardsGetLeaderboardAroundEntityRequest& input);
};

class GetLeaderboardDefinitionRequest : public Wrappers::PFLeaderboardsGetLeaderboardDefinitionRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsGetLeaderboardDefinitionRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFLeaderboardsGetLeaderboardDefinitionRequest& input);
};

class GetLeaderboardDefinitionResponse : public Wrappers::PFLeaderboardsGetLeaderboardDefinitionResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFLeaderboardsGetLeaderboardDefinitionResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsGetLeaderboardDefinitionResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFLeaderboardsGetLeaderboardDefinitionResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFLeaderboardsGetLeaderboardDefinitionResponse& model);
    static HRESULT Copy(const PFLeaderboardsGetLeaderboardDefinitionResponse& input, PFLeaderboardsGetLeaderboardDefinitionResponse& output, ModelBuffer& buffer);
};

class GetLeaderboardForEntitiesRequest : public Wrappers::PFLeaderboardsGetLeaderboardForEntitiesRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsGetLeaderboardForEntitiesRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFLeaderboardsGetLeaderboardForEntitiesRequest& input);
};

class IncrementLeaderboardVersionRequest : public Wrappers::PFLeaderboardsIncrementLeaderboardVersionRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsIncrementLeaderboardVersionRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFLeaderboardsIncrementLeaderboardVersionRequest& input);
};

class IncrementLeaderboardVersionResponse : public Wrappers::PFLeaderboardsIncrementLeaderboardVersionResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFLeaderboardsIncrementLeaderboardVersionResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsIncrementLeaderboardVersionResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFLeaderboardsIncrementLeaderboardVersionResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFLeaderboardsIncrementLeaderboardVersionResponse& model);
    static HRESULT Copy(const PFLeaderboardsIncrementLeaderboardVersionResponse& input, PFLeaderboardsIncrementLeaderboardVersionResponse& output, ModelBuffer& buffer);
};

class ListLeaderboardDefinitionsRequest : public Wrappers::PFLeaderboardsListLeaderboardDefinitionsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsListLeaderboardDefinitionsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFLeaderboardsListLeaderboardDefinitionsRequest& input);
};

class LeaderboardDefinition : public Wrappers::PFLeaderboardsLeaderboardDefinitionWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFLeaderboardsLeaderboardDefinition>
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsLeaderboardDefinitionWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFLeaderboardsLeaderboardDefinition const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFLeaderboardsLeaderboardDefinition& model);
    static HRESULT Copy(const PFLeaderboardsLeaderboardDefinition& input, PFLeaderboardsLeaderboardDefinition& output, ModelBuffer& buffer);
};

class ListLeaderboardDefinitionsResponse : public Wrappers::PFLeaderboardsListLeaderboardDefinitionsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFLeaderboardsListLeaderboardDefinitionsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsListLeaderboardDefinitionsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFLeaderboardsListLeaderboardDefinitionsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFLeaderboardsListLeaderboardDefinitionsResponse& model);
    static HRESULT Copy(const PFLeaderboardsListLeaderboardDefinitionsResponse& input, PFLeaderboardsListLeaderboardDefinitionsResponse& output, ModelBuffer& buffer);
};

class UnlinkLeaderboardFromStatisticRequest : public Wrappers::PFLeaderboardsUnlinkLeaderboardFromStatisticRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsUnlinkLeaderboardFromStatisticRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFLeaderboardsUnlinkLeaderboardFromStatisticRequest& input);
};

class LeaderboardEntryUpdate : public Wrappers::PFLeaderboardsLeaderboardEntryUpdateWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsLeaderboardEntryUpdateWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFLeaderboardsLeaderboardEntryUpdate& input);
};

class UpdateLeaderboardEntriesRequest : public Wrappers::PFLeaderboardsUpdateLeaderboardEntriesRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFLeaderboardsUpdateLeaderboardEntriesRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFLeaderboardsUpdateLeaderboardEntriesRequest& input);
};

} // namespace Leaderboards

// Json serialization helpers
namespace JsonUtils
{

JsonValue ToJson(PFExternalFriendSources const* input);

} // namespace JsonUtils

// EnumRange definitions used for Enum (de)serialization
template<typename T> struct EnumRange;

template<> struct EnumRange<PFLeaderboardsLeaderboardSortDirection>
{
    static constexpr PFLeaderboardsLeaderboardSortDirection maxValue = PFLeaderboardsLeaderboardSortDirection::Ascending;
};

template<> struct EnumRange<PFExternalFriendSources>
{
    static constexpr PFExternalFriendSources maxValue = PFExternalFriendSources::All;
};


} // namespace PlayFab
