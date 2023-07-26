#pragma once

#include <playfab/services/cpp/CharacterTypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace Character
{

// Character Classes
class ListUsersCharactersRequest : public Wrappers::PFCharacterListUsersCharactersRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCharacterListUsersCharactersRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCharacterListUsersCharactersRequest& input);
};

class ListUsersCharactersResult : public Wrappers::PFCharacterListUsersCharactersResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCharacterListUsersCharactersResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFCharacterListUsersCharactersResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCharacterListUsersCharactersResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCharacterListUsersCharactersResult& model);
    static HRESULT Copy(const PFCharacterListUsersCharactersResult& input, PFCharacterListUsersCharactersResult& output, ModelBuffer& buffer);
};

class GetCharacterDataRequest : public Wrappers::PFCharacterGetCharacterDataRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCharacterGetCharacterDataRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCharacterGetCharacterDataRequest& input);
};

class ClientGetCharacterDataResult : public Wrappers::PFCharacterClientGetCharacterDataResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCharacterClientGetCharacterDataResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFCharacterClientGetCharacterDataResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCharacterClientGetCharacterDataResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCharacterClientGetCharacterDataResult& model);
    static HRESULT Copy(const PFCharacterClientGetCharacterDataResult& input, PFCharacterClientGetCharacterDataResult& output, ModelBuffer& buffer);
};

class ClientGrantCharacterToUserRequest : public Wrappers::PFCharacterClientGrantCharacterToUserRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCharacterClientGrantCharacterToUserRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCharacterClientGrantCharacterToUserRequest& input);
};

class ClientGrantCharacterToUserResult : public Wrappers::PFCharacterClientGrantCharacterToUserResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCharacterClientGrantCharacterToUserResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFCharacterClientGrantCharacterToUserResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCharacterClientGrantCharacterToUserResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCharacterClientGrantCharacterToUserResult& model);
    static HRESULT Copy(const PFCharacterClientGrantCharacterToUserResult& input, PFCharacterClientGrantCharacterToUserResult& output, ModelBuffer& buffer);
};

class ClientUpdateCharacterDataRequest : public Wrappers::PFCharacterClientUpdateCharacterDataRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCharacterClientUpdateCharacterDataRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCharacterClientUpdateCharacterDataRequest& input);
};

class UpdateCharacterDataResult : public Wrappers::PFCharacterUpdateCharacterDataResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCharacterUpdateCharacterDataResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFCharacterUpdateCharacterDataResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCharacterUpdateCharacterDataResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCharacterUpdateCharacterDataResult& model);
    static HRESULT Copy(const PFCharacterUpdateCharacterDataResult& input, PFCharacterUpdateCharacterDataResult& output, ModelBuffer& buffer);
};

class DeleteCharacterFromUserRequest : public Wrappers::PFCharacterDeleteCharacterFromUserRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCharacterDeleteCharacterFromUserRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCharacterDeleteCharacterFromUserRequest& input);
};

class ServerGetCharacterDataResult : public Wrappers::PFCharacterServerGetCharacterDataResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCharacterServerGetCharacterDataResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFCharacterServerGetCharacterDataResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCharacterServerGetCharacterDataResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCharacterServerGetCharacterDataResult& model);
    static HRESULT Copy(const PFCharacterServerGetCharacterDataResult& input, PFCharacterServerGetCharacterDataResult& output, ModelBuffer& buffer);
};

class ServerGrantCharacterToUserRequest : public Wrappers::PFCharacterServerGrantCharacterToUserRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCharacterServerGrantCharacterToUserRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCharacterServerGrantCharacterToUserRequest& input);
};

class ServerGrantCharacterToUserResult : public Wrappers::PFCharacterServerGrantCharacterToUserResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCharacterServerGrantCharacterToUserResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFCharacterServerGrantCharacterToUserResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCharacterServerGrantCharacterToUserResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCharacterServerGrantCharacterToUserResult& model);
    static HRESULT Copy(const PFCharacterServerGrantCharacterToUserResult& input, PFCharacterServerGrantCharacterToUserResult& output, ModelBuffer& buffer);
};

class ServerUpdateCharacterDataRequest : public Wrappers::PFCharacterServerUpdateCharacterDataRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCharacterServerUpdateCharacterDataRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCharacterServerUpdateCharacterDataRequest& input);
};

} // namespace Character
// EnumRange definitions used for Enum (de)serialization
} // namespace PlayFab
