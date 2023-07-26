// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFCharacterTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFCharacterListUsersCharactersRequestWrapper : public ModelWrapper<PFCharacterListUsersCharactersRequest, Alloc>
{
public:
    using ModelType = PFCharacterListUsersCharactersRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCharacterListUsersCharactersRequestWrapper() = default;

    PFCharacterListUsersCharactersRequestWrapper(const PFCharacterListUsersCharactersRequest& model) :
        ModelWrapper<PFCharacterListUsersCharactersRequest, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFCharacterListUsersCharactersRequestWrapper(const PFCharacterListUsersCharactersRequestWrapper& src) :
        PFCharacterListUsersCharactersRequestWrapper{ src.Model() }
    {
    }

    PFCharacterListUsersCharactersRequestWrapper(PFCharacterListUsersCharactersRequestWrapper&& src) :
        PFCharacterListUsersCharactersRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCharacterListUsersCharactersRequestWrapper& operator=(PFCharacterListUsersCharactersRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCharacterListUsersCharactersRequestWrapper() = default;

    friend void swap(PFCharacterListUsersCharactersRequestWrapper& lhs, PFCharacterListUsersCharactersRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCharacterListUsersCharactersResultWrapper : public ModelWrapper<PFCharacterListUsersCharactersResult, Alloc>
{
public:
    using ModelType = PFCharacterListUsersCharactersResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCharacterListUsersCharactersResultWrapper() = default;

    PFCharacterListUsersCharactersResultWrapper(const PFCharacterListUsersCharactersResult& model) :
        ModelWrapper<PFCharacterListUsersCharactersResult, Alloc>{ model },
        m_characters{ model.characters, model.characters + model.charactersCount }
    {
        SetModelPointers();
    }

    PFCharacterListUsersCharactersResultWrapper(const PFCharacterListUsersCharactersResultWrapper& src) :
        PFCharacterListUsersCharactersResultWrapper{ src.Model() }
    {
    }

    PFCharacterListUsersCharactersResultWrapper(PFCharacterListUsersCharactersResultWrapper&& src) :
        PFCharacterListUsersCharactersResultWrapper{}
    {
        swap(*this, src);
    }

    PFCharacterListUsersCharactersResultWrapper& operator=(PFCharacterListUsersCharactersResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCharacterListUsersCharactersResultWrapper() = default;

    friend void swap(PFCharacterListUsersCharactersResultWrapper& lhs, PFCharacterListUsersCharactersResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_characters, rhs.m_characters);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCharacters(ModelVector<PFCharacterResultWrapper<Alloc>, Alloc> value)
    {
        m_characters = std::move(value);
        this->m_model.characters =  m_characters.empty() ? nullptr : m_characters.data();
        this->m_model.charactersCount =  static_cast<uint32_t>(m_characters.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.characters = m_characters.empty() ? nullptr : m_characters.data();
    }

    ModelVector<PFCharacterResultWrapper<Alloc>, Alloc> m_characters;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCharacterGetCharacterDataRequestWrapper : public ModelWrapper<PFCharacterGetCharacterDataRequest, Alloc>
{
public:
    using ModelType = PFCharacterGetCharacterDataRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCharacterGetCharacterDataRequestWrapper() = default;

    PFCharacterGetCharacterDataRequestWrapper(const PFCharacterGetCharacterDataRequest& model) :
        ModelWrapper<PFCharacterGetCharacterDataRequest, Alloc>{ model },
        m_characterId{ SafeString(model.characterId) },
        m_ifChangedFromDataVersion{ model.ifChangedFromDataVersion ? std::optional<uint32_t>{ *model.ifChangedFromDataVersion } : std::nullopt },
        m_keys{ model.keys, model.keys + model.keysCount },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFCharacterGetCharacterDataRequestWrapper(const PFCharacterGetCharacterDataRequestWrapper& src) :
        PFCharacterGetCharacterDataRequestWrapper{ src.Model() }
    {
    }

    PFCharacterGetCharacterDataRequestWrapper(PFCharacterGetCharacterDataRequestWrapper&& src) :
        PFCharacterGetCharacterDataRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCharacterGetCharacterDataRequestWrapper& operator=(PFCharacterGetCharacterDataRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCharacterGetCharacterDataRequestWrapper() = default;

    friend void swap(PFCharacterGetCharacterDataRequestWrapper& lhs, PFCharacterGetCharacterDataRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_characterId, rhs.m_characterId);
        swap(lhs.m_ifChangedFromDataVersion, rhs.m_ifChangedFromDataVersion);
        swap(lhs.m_keys, rhs.m_keys);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCharacterId(String value)
    {
        m_characterId = std::move(value);
        this->m_model.characterId =  m_characterId.empty() ? nullptr : m_characterId.data();
    }

    void SetIfChangedFromDataVersion(std::optional<uint32_t> value)
    {
        m_ifChangedFromDataVersion = std::move(value);
        this->m_model.ifChangedFromDataVersion = m_ifChangedFromDataVersion ? m_ifChangedFromDataVersion.operator->() : nullptr;
    }

    void SetKeys(CStringVector<Alloc> value)
    {
        m_keys = std::move(value);
        this->m_model.keys =  m_keys.empty() ? nullptr : m_keys.data();
        this->m_model.keysCount =  static_cast<uint32_t>(m_keys.size());
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.characterId = m_characterId.empty() ? nullptr : m_characterId.data();
        this->m_model.ifChangedFromDataVersion = m_ifChangedFromDataVersion ? m_ifChangedFromDataVersion.operator->() : nullptr;
        this->m_model.keys = m_keys.empty() ? nullptr : m_keys.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_characterId;
    std::optional<uint32_t> m_ifChangedFromDataVersion;
    CStringVector<Alloc> m_keys;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCharacterClientGetCharacterDataResultWrapper : public ModelWrapper<PFCharacterClientGetCharacterDataResult, Alloc>
{
public:
    using ModelType = PFCharacterClientGetCharacterDataResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCharacterClientGetCharacterDataResultWrapper() = default;

    PFCharacterClientGetCharacterDataResultWrapper(const PFCharacterClientGetCharacterDataResult& model) :
        ModelWrapper<PFCharacterClientGetCharacterDataResult, Alloc>{ model },
        m_characterId{ SafeString(model.characterId) },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFCharacterClientGetCharacterDataResultWrapper(const PFCharacterClientGetCharacterDataResultWrapper& src) :
        PFCharacterClientGetCharacterDataResultWrapper{ src.Model() }
    {
    }

    PFCharacterClientGetCharacterDataResultWrapper(PFCharacterClientGetCharacterDataResultWrapper&& src) :
        PFCharacterClientGetCharacterDataResultWrapper{}
    {
        swap(*this, src);
    }

    PFCharacterClientGetCharacterDataResultWrapper& operator=(PFCharacterClientGetCharacterDataResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCharacterClientGetCharacterDataResultWrapper() = default;

    friend void swap(PFCharacterClientGetCharacterDataResultWrapper& lhs, PFCharacterClientGetCharacterDataResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_characterId, rhs.m_characterId);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCharacterId(String value)
    {
        m_characterId = std::move(value);
        this->m_model.characterId =  m_characterId.empty() ? nullptr : m_characterId.data();
    }

    void SetData(ModelDictionaryEntryVector<PFUserDataRecordWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

    void SetDataVersion(uint32_t value)
    {
        this->m_model.dataVersion = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.characterId = m_characterId.empty() ? nullptr : m_characterId.data();
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    String m_characterId;
    ModelDictionaryEntryVector<PFUserDataRecordWrapper<Alloc>, Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCharacterClientGrantCharacterToUserRequestWrapper : public ModelWrapper<PFCharacterClientGrantCharacterToUserRequest, Alloc>
{
public:
    using ModelType = PFCharacterClientGrantCharacterToUserRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCharacterClientGrantCharacterToUserRequestWrapper() = default;

    PFCharacterClientGrantCharacterToUserRequestWrapper(const PFCharacterClientGrantCharacterToUserRequest& model) :
        ModelWrapper<PFCharacterClientGrantCharacterToUserRequest, Alloc>{ model },
        m_catalogVersion{ SafeString(model.catalogVersion) },
        m_characterName{ SafeString(model.characterName) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_itemId{ SafeString(model.itemId) }
    {
        SetModelPointers();
    }

    PFCharacterClientGrantCharacterToUserRequestWrapper(const PFCharacterClientGrantCharacterToUserRequestWrapper& src) :
        PFCharacterClientGrantCharacterToUserRequestWrapper{ src.Model() }
    {
    }

    PFCharacterClientGrantCharacterToUserRequestWrapper(PFCharacterClientGrantCharacterToUserRequestWrapper&& src) :
        PFCharacterClientGrantCharacterToUserRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCharacterClientGrantCharacterToUserRequestWrapper& operator=(PFCharacterClientGrantCharacterToUserRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCharacterClientGrantCharacterToUserRequestWrapper() = default;

    friend void swap(PFCharacterClientGrantCharacterToUserRequestWrapper& lhs, PFCharacterClientGrantCharacterToUserRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_catalogVersion, rhs.m_catalogVersion);
        swap(lhs.m_characterName, rhs.m_characterName);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_itemId, rhs.m_itemId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCatalogVersion(String value)
    {
        m_catalogVersion = std::move(value);
        this->m_model.catalogVersion =  m_catalogVersion.empty() ? nullptr : m_catalogVersion.data();
    }

    void SetCharacterName(String value)
    {
        m_characterName = std::move(value);
        this->m_model.characterName =  m_characterName.empty() ? nullptr : m_characterName.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetItemId(String value)
    {
        m_itemId = std::move(value);
        this->m_model.itemId =  m_itemId.empty() ? nullptr : m_itemId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.catalogVersion = m_catalogVersion.empty() ? nullptr : m_catalogVersion.data();
        this->m_model.characterName = m_characterName.empty() ? nullptr : m_characterName.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.itemId = m_itemId.empty() ? nullptr : m_itemId.data();
    }

    String m_catalogVersion;
    String m_characterName;
    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_itemId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCharacterClientGrantCharacterToUserResultWrapper : public ModelWrapper<PFCharacterClientGrantCharacterToUserResult, Alloc>
{
public:
    using ModelType = PFCharacterClientGrantCharacterToUserResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCharacterClientGrantCharacterToUserResultWrapper() = default;

    PFCharacterClientGrantCharacterToUserResultWrapper(const PFCharacterClientGrantCharacterToUserResult& model) :
        ModelWrapper<PFCharacterClientGrantCharacterToUserResult, Alloc>{ model },
        m_characterId{ SafeString(model.characterId) },
        m_characterType{ SafeString(model.characterType) }
    {
        SetModelPointers();
    }

    PFCharacterClientGrantCharacterToUserResultWrapper(const PFCharacterClientGrantCharacterToUserResultWrapper& src) :
        PFCharacterClientGrantCharacterToUserResultWrapper{ src.Model() }
    {
    }

    PFCharacterClientGrantCharacterToUserResultWrapper(PFCharacterClientGrantCharacterToUserResultWrapper&& src) :
        PFCharacterClientGrantCharacterToUserResultWrapper{}
    {
        swap(*this, src);
    }

    PFCharacterClientGrantCharacterToUserResultWrapper& operator=(PFCharacterClientGrantCharacterToUserResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCharacterClientGrantCharacterToUserResultWrapper() = default;

    friend void swap(PFCharacterClientGrantCharacterToUserResultWrapper& lhs, PFCharacterClientGrantCharacterToUserResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_characterId, rhs.m_characterId);
        swap(lhs.m_characterType, rhs.m_characterType);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCharacterId(String value)
    {
        m_characterId = std::move(value);
        this->m_model.characterId =  m_characterId.empty() ? nullptr : m_characterId.data();
    }

    void SetCharacterType(String value)
    {
        m_characterType = std::move(value);
        this->m_model.characterType =  m_characterType.empty() ? nullptr : m_characterType.data();
    }

    void SetResult(bool value)
    {
        this->m_model.result = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.characterId = m_characterId.empty() ? nullptr : m_characterId.data();
        this->m_model.characterType = m_characterType.empty() ? nullptr : m_characterType.data();
    }

    String m_characterId;
    String m_characterType;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCharacterClientUpdateCharacterDataRequestWrapper : public ModelWrapper<PFCharacterClientUpdateCharacterDataRequest, Alloc>
{
public:
    using ModelType = PFCharacterClientUpdateCharacterDataRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCharacterClientUpdateCharacterDataRequestWrapper() = default;

    PFCharacterClientUpdateCharacterDataRequestWrapper(const PFCharacterClientUpdateCharacterDataRequest& model) :
        ModelWrapper<PFCharacterClientUpdateCharacterDataRequest, Alloc>{ model },
        m_characterId{ SafeString(model.characterId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_data{ model.data, model.data + model.dataCount },
        m_keysToRemove{ model.keysToRemove, model.keysToRemove + model.keysToRemoveCount },
        m_permission{ model.permission ? std::optional<PFUserDataPermission>{ *model.permission } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCharacterClientUpdateCharacterDataRequestWrapper(const PFCharacterClientUpdateCharacterDataRequestWrapper& src) :
        PFCharacterClientUpdateCharacterDataRequestWrapper{ src.Model() }
    {
    }

    PFCharacterClientUpdateCharacterDataRequestWrapper(PFCharacterClientUpdateCharacterDataRequestWrapper&& src) :
        PFCharacterClientUpdateCharacterDataRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCharacterClientUpdateCharacterDataRequestWrapper& operator=(PFCharacterClientUpdateCharacterDataRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCharacterClientUpdateCharacterDataRequestWrapper() = default;

    friend void swap(PFCharacterClientUpdateCharacterDataRequestWrapper& lhs, PFCharacterClientUpdateCharacterDataRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_characterId, rhs.m_characterId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_data, rhs.m_data);
        swap(lhs.m_keysToRemove, rhs.m_keysToRemove);
        swap(lhs.m_permission, rhs.m_permission);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCharacterId(String value)
    {
        m_characterId = std::move(value);
        this->m_model.characterId =  m_characterId.empty() ? nullptr : m_characterId.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetData(StringDictionaryEntryVector<Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

    void SetKeysToRemove(CStringVector<Alloc> value)
    {
        m_keysToRemove = std::move(value);
        this->m_model.keysToRemove =  m_keysToRemove.empty() ? nullptr : m_keysToRemove.data();
        this->m_model.keysToRemoveCount =  static_cast<uint32_t>(m_keysToRemove.size());
    }

    void SetPermission(std::optional<PFUserDataPermission> value)
    {
        m_permission = std::move(value);
        this->m_model.permission = m_permission ? m_permission.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.characterId = m_characterId.empty() ? nullptr : m_characterId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
        this->m_model.keysToRemove = m_keysToRemove.empty() ? nullptr : m_keysToRemove.data();
        this->m_model.permission = m_permission ? m_permission.operator->() : nullptr;
    }

    String m_characterId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    StringDictionaryEntryVector<Alloc> m_data;
    CStringVector<Alloc> m_keysToRemove;
    std::optional<PFUserDataPermission> m_permission;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCharacterUpdateCharacterDataResultWrapper : public ModelWrapper<PFCharacterUpdateCharacterDataResult, Alloc>
{
public:
    using ModelType = PFCharacterUpdateCharacterDataResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFCharacterUpdateCharacterDataResult, Alloc>::ModelWrapper;

    void SetDataVersion(uint32_t value)
    {
        this->m_model.dataVersion = value;
    }

private:
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCharacterDeleteCharacterFromUserRequestWrapper : public ModelWrapper<PFCharacterDeleteCharacterFromUserRequest, Alloc>
{
public:
    using ModelType = PFCharacterDeleteCharacterFromUserRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCharacterDeleteCharacterFromUserRequestWrapper() = default;

    PFCharacterDeleteCharacterFromUserRequestWrapper(const PFCharacterDeleteCharacterFromUserRequest& model) :
        ModelWrapper<PFCharacterDeleteCharacterFromUserRequest, Alloc>{ model },
        m_characterId{ SafeString(model.characterId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFCharacterDeleteCharacterFromUserRequestWrapper(const PFCharacterDeleteCharacterFromUserRequestWrapper& src) :
        PFCharacterDeleteCharacterFromUserRequestWrapper{ src.Model() }
    {
    }

    PFCharacterDeleteCharacterFromUserRequestWrapper(PFCharacterDeleteCharacterFromUserRequestWrapper&& src) :
        PFCharacterDeleteCharacterFromUserRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCharacterDeleteCharacterFromUserRequestWrapper& operator=(PFCharacterDeleteCharacterFromUserRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCharacterDeleteCharacterFromUserRequestWrapper() = default;

    friend void swap(PFCharacterDeleteCharacterFromUserRequestWrapper& lhs, PFCharacterDeleteCharacterFromUserRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_characterId, rhs.m_characterId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCharacterId(String value)
    {
        m_characterId = std::move(value);
        this->m_model.characterId =  m_characterId.empty() ? nullptr : m_characterId.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetSaveCharacterInventory(bool value)
    {
        this->m_model.saveCharacterInventory = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.characterId = m_characterId.empty() ? nullptr : m_characterId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_characterId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCharacterServerGetCharacterDataResultWrapper : public ModelWrapper<PFCharacterServerGetCharacterDataResult, Alloc>
{
public:
    using ModelType = PFCharacterServerGetCharacterDataResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCharacterServerGetCharacterDataResultWrapper() = default;

    PFCharacterServerGetCharacterDataResultWrapper(const PFCharacterServerGetCharacterDataResult& model) :
        ModelWrapper<PFCharacterServerGetCharacterDataResult, Alloc>{ model },
        m_characterId{ SafeString(model.characterId) },
        m_data{ model.data, model.data + model.dataCount },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFCharacterServerGetCharacterDataResultWrapper(const PFCharacterServerGetCharacterDataResultWrapper& src) :
        PFCharacterServerGetCharacterDataResultWrapper{ src.Model() }
    {
    }

    PFCharacterServerGetCharacterDataResultWrapper(PFCharacterServerGetCharacterDataResultWrapper&& src) :
        PFCharacterServerGetCharacterDataResultWrapper{}
    {
        swap(*this, src);
    }

    PFCharacterServerGetCharacterDataResultWrapper& operator=(PFCharacterServerGetCharacterDataResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCharacterServerGetCharacterDataResultWrapper() = default;

    friend void swap(PFCharacterServerGetCharacterDataResultWrapper& lhs, PFCharacterServerGetCharacterDataResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_characterId, rhs.m_characterId);
        swap(lhs.m_data, rhs.m_data);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCharacterId(String value)
    {
        m_characterId = std::move(value);
        this->m_model.characterId =  m_characterId.empty() ? nullptr : m_characterId.data();
    }

    void SetData(ModelDictionaryEntryVector<PFUserDataRecordWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

    void SetDataVersion(uint32_t value)
    {
        this->m_model.dataVersion = value;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.characterId = m_characterId.empty() ? nullptr : m_characterId.data();
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_characterId;
    ModelDictionaryEntryVector<PFUserDataRecordWrapper<Alloc>, Alloc> m_data;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCharacterServerGrantCharacterToUserRequestWrapper : public ModelWrapper<PFCharacterServerGrantCharacterToUserRequest, Alloc>
{
public:
    using ModelType = PFCharacterServerGrantCharacterToUserRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCharacterServerGrantCharacterToUserRequestWrapper() = default;

    PFCharacterServerGrantCharacterToUserRequestWrapper(const PFCharacterServerGrantCharacterToUserRequest& model) :
        ModelWrapper<PFCharacterServerGrantCharacterToUserRequest, Alloc>{ model },
        m_characterName{ SafeString(model.characterName) },
        m_characterType{ SafeString(model.characterType) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFCharacterServerGrantCharacterToUserRequestWrapper(const PFCharacterServerGrantCharacterToUserRequestWrapper& src) :
        PFCharacterServerGrantCharacterToUserRequestWrapper{ src.Model() }
    {
    }

    PFCharacterServerGrantCharacterToUserRequestWrapper(PFCharacterServerGrantCharacterToUserRequestWrapper&& src) :
        PFCharacterServerGrantCharacterToUserRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCharacterServerGrantCharacterToUserRequestWrapper& operator=(PFCharacterServerGrantCharacterToUserRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCharacterServerGrantCharacterToUserRequestWrapper() = default;

    friend void swap(PFCharacterServerGrantCharacterToUserRequestWrapper& lhs, PFCharacterServerGrantCharacterToUserRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_characterName, rhs.m_characterName);
        swap(lhs.m_characterType, rhs.m_characterType);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCharacterName(String value)
    {
        m_characterName = std::move(value);
        this->m_model.characterName =  m_characterName.empty() ? nullptr : m_characterName.data();
    }

    void SetCharacterType(String value)
    {
        m_characterType = std::move(value);
        this->m_model.characterType =  m_characterType.empty() ? nullptr : m_characterType.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.characterName = m_characterName.empty() ? nullptr : m_characterName.data();
        this->m_model.characterType = m_characterType.empty() ? nullptr : m_characterType.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_characterName;
    String m_characterType;
    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCharacterServerGrantCharacterToUserResultWrapper : public ModelWrapper<PFCharacterServerGrantCharacterToUserResult, Alloc>
{
public:
    using ModelType = PFCharacterServerGrantCharacterToUserResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCharacterServerGrantCharacterToUserResultWrapper() = default;

    PFCharacterServerGrantCharacterToUserResultWrapper(const PFCharacterServerGrantCharacterToUserResult& model) :
        ModelWrapper<PFCharacterServerGrantCharacterToUserResult, Alloc>{ model },
        m_characterId{ SafeString(model.characterId) }
    {
        SetModelPointers();
    }

    PFCharacterServerGrantCharacterToUserResultWrapper(const PFCharacterServerGrantCharacterToUserResultWrapper& src) :
        PFCharacterServerGrantCharacterToUserResultWrapper{ src.Model() }
    {
    }

    PFCharacterServerGrantCharacterToUserResultWrapper(PFCharacterServerGrantCharacterToUserResultWrapper&& src) :
        PFCharacterServerGrantCharacterToUserResultWrapper{}
    {
        swap(*this, src);
    }

    PFCharacterServerGrantCharacterToUserResultWrapper& operator=(PFCharacterServerGrantCharacterToUserResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCharacterServerGrantCharacterToUserResultWrapper() = default;

    friend void swap(PFCharacterServerGrantCharacterToUserResultWrapper& lhs, PFCharacterServerGrantCharacterToUserResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_characterId, rhs.m_characterId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCharacterId(String value)
    {
        m_characterId = std::move(value);
        this->m_model.characterId =  m_characterId.empty() ? nullptr : m_characterId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.characterId = m_characterId.empty() ? nullptr : m_characterId.data();
    }

    String m_characterId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCharacterServerUpdateCharacterDataRequestWrapper : public ModelWrapper<PFCharacterServerUpdateCharacterDataRequest, Alloc>
{
public:
    using ModelType = PFCharacterServerUpdateCharacterDataRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCharacterServerUpdateCharacterDataRequestWrapper() = default;

    PFCharacterServerUpdateCharacterDataRequestWrapper(const PFCharacterServerUpdateCharacterDataRequest& model) :
        ModelWrapper<PFCharacterServerUpdateCharacterDataRequest, Alloc>{ model },
        m_characterId{ SafeString(model.characterId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_data{ model.data, model.data + model.dataCount },
        m_keysToRemove{ model.keysToRemove, model.keysToRemove + model.keysToRemoveCount },
        m_permission{ model.permission ? std::optional<PFUserDataPermission>{ *model.permission } : std::nullopt },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFCharacterServerUpdateCharacterDataRequestWrapper(const PFCharacterServerUpdateCharacterDataRequestWrapper& src) :
        PFCharacterServerUpdateCharacterDataRequestWrapper{ src.Model() }
    {
    }

    PFCharacterServerUpdateCharacterDataRequestWrapper(PFCharacterServerUpdateCharacterDataRequestWrapper&& src) :
        PFCharacterServerUpdateCharacterDataRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCharacterServerUpdateCharacterDataRequestWrapper& operator=(PFCharacterServerUpdateCharacterDataRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCharacterServerUpdateCharacterDataRequestWrapper() = default;

    friend void swap(PFCharacterServerUpdateCharacterDataRequestWrapper& lhs, PFCharacterServerUpdateCharacterDataRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_characterId, rhs.m_characterId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_data, rhs.m_data);
        swap(lhs.m_keysToRemove, rhs.m_keysToRemove);
        swap(lhs.m_permission, rhs.m_permission);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCharacterId(String value)
    {
        m_characterId = std::move(value);
        this->m_model.characterId =  m_characterId.empty() ? nullptr : m_characterId.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetData(StringDictionaryEntryVector<Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

    void SetKeysToRemove(CStringVector<Alloc> value)
    {
        m_keysToRemove = std::move(value);
        this->m_model.keysToRemove =  m_keysToRemove.empty() ? nullptr : m_keysToRemove.data();
        this->m_model.keysToRemoveCount =  static_cast<uint32_t>(m_keysToRemove.size());
    }

    void SetPermission(std::optional<PFUserDataPermission> value)
    {
        m_permission = std::move(value);
        this->m_model.permission = m_permission ? m_permission.operator->() : nullptr;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.characterId = m_characterId.empty() ? nullptr : m_characterId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
        this->m_model.keysToRemove = m_keysToRemove.empty() ? nullptr : m_keysToRemove.data();
        this->m_model.permission = m_permission ? m_permission.operator->() : nullptr;
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_characterId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    StringDictionaryEntryVector<Alloc> m_data;
    CStringVector<Alloc> m_keysToRemove;
    std::optional<PFUserDataPermission> m_permission;
    String m_playFabId;
};

} // namespace Wrappers
} // namespace PlayFab
