// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFTypes.h>
#include <vector>
#include <string>
#include <type_traits>
#include <optional>

namespace PlayFab
{
namespace Detail
{

template <typename T, typename = void>
struct IsDictionaryEntry : std::false_type {};

template <typename T>
struct IsDictionaryEntry<T, std::enable_if_t<std::is_same_v<decltype(T::key), const char*>&& std::is_same_v<decltype(T::value), decltype(T::value)>>> : std::true_type {};

}

namespace Wrappers
{

inline const char* SafeString(const char* input)
{
    static const char* emptyString{ "" };
    return input ? input : emptyString;
}

template<typename ModelT, template<typename AllocT> class Alloc = std::allocator>
class ModelWrapper
{
public:
    ModelWrapper() = default;
    ModelWrapper(const ModelT& src) : m_model{ src } {}
    ModelWrapper(const ModelWrapper&) = default;
    ModelWrapper(ModelWrapper&&) = default;
    ModelWrapper& operator=(const ModelWrapper&) = default;
    ModelWrapper& operator=(ModelWrapper&&) = default;
    virtual ~ModelWrapper() = default;

    ModelT const& Model() const
    {
        return m_model;
    }
protected:
    ModelT m_model{};
};

template<template<typename AllocT> class Alloc = std::allocator>
class JsonObject
{
public:
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;

    JsonObject() = default;
    JsonObject(const PFJsonObject& value) : stringValue{ SafeString(value.stringValue) } {}
    JsonObject(const char* value) : stringValue{ SafeString(value) } {}
    JsonObject(String value) : stringValue{ std::move(value) } {}
    JsonObject(const JsonObject& src) = default;
    JsonObject(JsonObject&& src) = default;
    JsonObject& operator=(const JsonObject& src) = default;
    ~JsonObject() = default;

    String stringValue;
};

template<typename PointerT, typename ValueT, template<typename AllocT> class Alloc = std::allocator>
class PointerVector
{
public:
    using value_vector_type = std::vector<ValueT, Alloc<ValueT>>;
    using pointer_vector_type = std::vector<PointerT const*, Alloc<PointerT const*>>;
    using value_type = typename value_vector_type::value_type;
    using pointer_type = typename pointer_vector_type::value_type;
    using size_type = typename value_vector_type::size_type;

    PointerVector() = default;
    PointerVector(const PointerVector& src) = default;
    PointerVector(PointerVector&& src) = default;
    PointerVector& operator=(const PointerVector& src);
    PointerVector& operator=(PointerVector&&) = default;
    virtual ~PointerVector() = default;

    // element access
    value_type const& operator[](size_type index) const;
    pointer_type const* data() noexcept;
    pointer_type const* const data() const noexcept;

    // capacity
    bool empty() const noexcept;
    size_type size() const noexcept;
    void reserve(size_type capacity) noexcept;

    // modifiers
    void clear() noexcept;
    void push_back(const value_type& value);
    void push_back(value_type&& value);

protected:
    virtual pointer_type GetPointer(const value_type& value) const = 0;

    pointer_vector_type m_pointers;
    value_vector_type m_values;
};

template <typename EntryT, template<typename AllocT> class Alloc = std::allocator>
class DictionaryEntryVector
{
public:
    static_assert(Detail::IsDictionaryEntry<EntryT>::value, "EntryT must have const char* \"key\" and \"value\" properties");

    using entry_vector_type = std::vector<EntryT, Alloc<EntryT>>;
    using entry_type = EntryT;
    using key_type = std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    using value_type = decltype(EntryT::value);
    using size_type = typename entry_vector_type::size_type;

    DictionaryEntryVector() = default;
    DictionaryEntryVector(entry_type const* begin, entry_type const* end);
    DictionaryEntryVector(const DictionaryEntryVector& src);
    DictionaryEntryVector(DictionaryEntryVector&& src) = default;
    DictionaryEntryVector& operator=(const DictionaryEntryVector& src);
    DictionaryEntryVector& operator=(DictionaryEntryVector&& src) = default;
    virtual ~DictionaryEntryVector() = default;

    // element access (vector-style and map-style)
    entry_type const& operator[](size_type index) const;
    entry_type const* data() noexcept;
    entry_type const* const data() const noexcept;
    value_type& at(const key_type& key);
    value_type const& at(const key_type& key) const;

    // capacity
    bool empty() const noexcept;
    size_type size() const noexcept;
    void reserve(size_type capacity) noexcept;

    // modifiers (map-style)
    void clear() noexcept;
    std::pair<entry_type const*, bool> insert_or_assign(const key_type& key, value_type value);
    std::pair<entry_type const*, bool> insert_or_assign(const entry_type& entry);

protected:
    entry_vector_type m_entries;
    std::map<key_type, size_type, std::less<key_type>, Alloc<std::pair<const key_type, size_type>>> m_map; // maps keys to entries index
};

template <typename EntryT, typename ValueT, template<typename AllocT> class Alloc = std::allocator>
class PointerDictionaryEntryVector : public DictionaryEntryVector<EntryT, Alloc>
{
public:
    using typename DictionaryEntryVector<EntryT, Alloc>::entry_vector_type;
    using typename DictionaryEntryVector<EntryT, Alloc>::entry_type;
    using typename DictionaryEntryVector<EntryT, Alloc>::key_type;
    using value_type = ValueT;
    using pointer_type = decltype(EntryT::value);
    using typename DictionaryEntryVector<EntryT, Alloc>::size_type;

    PointerDictionaryEntryVector() = default;
    PointerDictionaryEntryVector(const PointerDictionaryEntryVector& src) = default;
    PointerDictionaryEntryVector(PointerDictionaryEntryVector&& src) = default;
    PointerDictionaryEntryVector& operator=(const PointerDictionaryEntryVector& src);
    PointerDictionaryEntryVector& operator=(PointerDictionaryEntryVector&& src) = default;
    virtual ~PointerDictionaryEntryVector() = default;

    value_type const& at(const key_type& key) const;
    void reserve(size_type capacity) noexcept;
    void clear() noexcept;
    std::pair<EntryT const*, bool> insert_or_assign(const key_type& key, value_type&& value);
    template<typename V = ValueT,
             typename PT = pointer_type,
             typename = std::enable_if_t<std::is_constructible_v<V, PT>>*>
    std::pair<EntryT const*, bool> insert_or_assign(const entry_type& entry);

protected:
    virtual pointer_type GetPointer(const value_type& value) const = 0;

    using DictionaryEntryVector<EntryT, Alloc>::m_entries;
    using DictionaryEntryVector<EntryT, Alloc>::m_map;
    std::vector<value_type, Alloc<value_type>> m_values;
};

template<typename ModelWrapperT, template<typename AllocT> class Alloc = std::allocator>
class ModelVector : public PointerVector<typename ModelWrapperT::ModelType, ModelWrapperT, Alloc>
{
public:
    using PointerVectorType = PointerVector<typename ModelWrapperT::ModelType, ModelWrapperT, Alloc>;

    ModelVector() = default;
    ModelVector(typename ModelWrapperT::ModelType const* const* begin, typename ModelWrapperT::ModelType const* const* end);
    ModelVector(const ModelVector& src);
    ModelVector(ModelVector&& src) = default;
    ModelVector& operator=(const ModelVector& src) = default;
    ModelVector& operator=(ModelVector&&) = default;
    ~ModelVector() = default;

private:
    typename PointerVectorType::pointer_type GetPointer(const typename PointerVectorType::value_type& value) const override
    {
        return &value.Model();
    }
};

template<template<typename AllocT> class Alloc = std::allocator>
class CStringVector : public PointerVector<char, std::basic_string<char, std::char_traits<char>, Alloc<char>>, Alloc>
{
public:
    using PointerVectorType = PointerVector<char, std::basic_string<char, std::char_traits<char>, Alloc<char>>, Alloc>;

    using PointerVectorType::PointerVectorType;
    CStringVector() = default;
    CStringVector(const char* const* begin, const char* const* end);
    CStringVector(std::initializer_list<std::basic_string<char, std::char_traits<char>, Alloc<char>>> init);
    CStringVector(const CStringVector& src);

private:
    typename PointerVectorType::pointer_type GetPointer(const typename PointerVectorType::value_type& value) const override
    {
        return value.data();
    }
};

template<typename ModelWrapperT, template<typename AllocT> class Alloc = std::allocator>
class ModelDictionaryEntryVector : public PointerDictionaryEntryVector<typename ModelWrapperT::DictionaryEntryType, ModelWrapperT, Alloc>
{
public:
    using PointerDictionaryEntryVectorType = PointerDictionaryEntryVector<typename ModelWrapperT::DictionaryEntryType, ModelWrapperT, Alloc>;

    using PointerDictionaryEntryVectorType::PointerDictionaryEntryVectorType;
    ModelDictionaryEntryVector() = default;
    ModelDictionaryEntryVector(typename ModelWrapperT::DictionaryEntryType const* begin, typename ModelWrapperT::DictionaryEntryType const* end);
    ModelDictionaryEntryVector(const ModelDictionaryEntryVector& src);

private:
    typename PointerDictionaryEntryVectorType::pointer_type GetPointer(const typename PointerDictionaryEntryVectorType::value_type& value) const override
    {
        return &value.Model();
    }
};

template<template<typename AllocT> class Alloc = std::allocator>
class StringDictionaryEntryVector : public PointerDictionaryEntryVector<PFStringDictionaryEntry, std::basic_string<char, std::char_traits<char>, Alloc<char>>, Alloc>
{
public:
    using PointerDictionaryEntryVectorType = PointerDictionaryEntryVector<PFStringDictionaryEntry, std::basic_string<char, std::char_traits<char>, Alloc<char>>, Alloc>;

    using PointerDictionaryEntryVectorType::PointerDictionaryEntryVectorType;
    StringDictionaryEntryVector() = default;
    StringDictionaryEntryVector(PFStringDictionaryEntry const* begin, PFStringDictionaryEntry const* end);
    StringDictionaryEntryVector(const StringDictionaryEntryVector& src);

private:
    typename PointerDictionaryEntryVectorType::pointer_type GetPointer(const typename PointerDictionaryEntryVectorType::value_type& value) const override
    {
        return value.data();
    }
};

//------------------------------------------------------------------------------
// Template implementations
//------------------------------------------------------------------------------

template<typename PointerT, typename ValueT, template<typename AllocT> class Alloc>
PointerVector<PointerT, ValueT, Alloc>& PointerVector<PointerT, ValueT, Alloc>::operator=(const PointerVector& src)
{
    m_values = src.m_values;
    m_pointers.clear();
    for (auto& value : m_values)
    {
        m_pointers.emplace_back(GetPointer(value));
    }
    return *this;
}

template<typename PointerT, typename ValueT, template<typename AllocT> class Alloc>
typename PointerVector<PointerT, ValueT, Alloc>::value_type const& PointerVector<PointerT, ValueT, Alloc>::operator[](size_type index) const
{
    return m_values[index];
}

template<typename PointerT, typename ValueT, template<typename AllocT> class Alloc>
typename PointerVector<PointerT, ValueT, Alloc>::pointer_type const* PointerVector<PointerT, ValueT, Alloc>::data() noexcept
{
    return m_pointers.data();
}

template<typename PointerT, typename ValueT, template<typename AllocT> class Alloc>
typename PointerVector<PointerT, ValueT, Alloc>::pointer_type const* const PointerVector<PointerT, ValueT, Alloc>::data() const noexcept
{
    return m_pointers.data();
}

template<typename PointerT, typename ValueT, template<typename AllocT> class Alloc>
bool PointerVector<PointerT, ValueT, Alloc>::empty() const noexcept
{
    assert(m_pointers.empty() == m_values.empty());
    return m_pointers.empty();
}


template<typename PointerT, typename ValueT, template<typename AllocT> class Alloc>
typename PointerVector<PointerT, ValueT, Alloc>::size_type PointerVector<PointerT, ValueT, Alloc>::size() const noexcept
{
    assert(m_pointers.size() == m_values.size());
    return m_pointers.size();
}

template<typename PointerT, typename ValueT, template<typename AllocT> class Alloc>
void PointerVector<PointerT, ValueT, Alloc>::reserve(size_type capacity) noexcept
{
    m_pointers.reserve(capacity);
    m_values.reserve(capacity);
}

template<typename PointerT, typename ValueT, template<typename AllocT> class Alloc>
void PointerVector<PointerT, ValueT, Alloc>::clear() noexcept
{
    m_pointers.clear();
    m_values.clear();
}

template<typename PointerT, typename ValueT, template<typename AllocT> class Alloc>
void PointerVector<PointerT, ValueT, Alloc>::push_back(const value_type& value)
{
    auto remainingCapacity = m_values.capacity() - m_values.size();
    m_values.push_back(value);
    if (remainingCapacity)
    {
        m_pointers.push_back(GetPointer(m_values.back()));
    }
    else
    {
        // reallocation happened, pointers invalidated
        m_pointers.emplace_back();
        std::transform(m_values.begin(), m_values.end(), m_pointers.begin(), [this](const ValueT& v) { return this->GetPointer(v); });
    }
}

template<typename PointerT, typename ValueT, template<typename AllocT> class Alloc>
void PointerVector<PointerT, ValueT, Alloc>::push_back(value_type&& value)
{
    auto remainingCapacity = m_values.capacity() - m_values.size();
    m_values.push_back(std::move(value));
    if (remainingCapacity)
    {
        m_pointers.push_back(GetPointer(m_values.back()));
    }
    else
    {
        // reallocation happened, pointers invalidated
        m_pointers.emplace_back();
        std::transform(m_values.begin(), m_values.end(), m_pointers.begin(), [this](const ValueT& v) { return this->GetPointer(v); });
    }
}

template<typename EntryT, template<typename AllocT> class Alloc>
DictionaryEntryVector<EntryT, Alloc>::DictionaryEntryVector(entry_type const* begin, entry_type const* end)
{
    m_entries.reserve(end - begin);
    for (;begin != end; ++begin)
    {
        insert_or_assign(*begin);
    }
}

template<typename EntryT, template<typename AllocT> class Alloc>
DictionaryEntryVector<EntryT, Alloc>::DictionaryEntryVector(const DictionaryEntryVector& src)
    : m_entries{ src.m_entries },
    m_map{ src.m_map }
{
    for (auto& mapEntry : m_map)
    {
        m_entries[mapEntry.second].key = mapEntry.first.data();
    }
}

template<typename EntryT, template<typename AllocT> class Alloc>
DictionaryEntryVector<EntryT, Alloc>& DictionaryEntryVector<EntryT, Alloc>::operator=(const DictionaryEntryVector& src)
{
    m_entries = src.m_entries;
    m_map = src.m_map;

    for (auto& mapEntry : m_map)
    {
        m_entries[mapEntry.second].key = mapEntry.first.data();
    }
    return *this;
}

template<typename EntryT, template<typename AllocT> class Alloc>
typename DictionaryEntryVector<EntryT, Alloc>::entry_type const& DictionaryEntryVector<EntryT, Alloc>::operator[](size_type index) const
{
    return m_entries[index];
}

template<typename EntryT, template<typename AllocT> class Alloc>
typename DictionaryEntryVector<EntryT, Alloc>::entry_type const* DictionaryEntryVector<EntryT, Alloc>::data() noexcept
{
    return m_entries.data();
}

template<typename EntryT, template<typename AllocT> class Alloc>
typename DictionaryEntryVector<EntryT, Alloc>::entry_type const* const DictionaryEntryVector<EntryT, Alloc>::data() const noexcept
{
    return m_entries.data();
}

template<typename EntryT, template<typename AllocT> class Alloc>
typename DictionaryEntryVector<EntryT, Alloc>::value_type& DictionaryEntryVector<EntryT, Alloc>::at(const key_type& key)
{
    auto iter = m_map.find(key);
    if (iter == m_map.end())
    {
        throw std::out_of_range("");
    }
    return m_entries[iter->second].value;
}

template<typename EntryT, template<typename AllocT> class Alloc>
typename DictionaryEntryVector<EntryT, Alloc>::value_type const& DictionaryEntryVector<EntryT, Alloc>::at(const key_type& key) const
{
    auto iter = m_map.find(key);
    if (iter == m_map.end())
    {
        throw std::out_of_range("");
    }
    return m_entries[iter->second].value;
}

template<typename EntryT, template<typename AllocT> class Alloc>
bool DictionaryEntryVector<EntryT, Alloc>::empty() const noexcept
{
    assert(m_entries.empty() == m_map.empty());
    return m_entries.empty();
}

template<typename EntryT, template<typename AllocT> class Alloc>
typename DictionaryEntryVector<EntryT, Alloc>::size_type DictionaryEntryVector<EntryT, Alloc>::size() const noexcept
{
    assert(m_entries.size() == m_map.size());
    return m_entries.size();
}

template<typename EntryT, template<typename AllocT> class Alloc>
void DictionaryEntryVector<EntryT, Alloc>::reserve(size_type capacity) noexcept
{
    m_entries.reserve(capacity);
}

template<typename EntryT, template<typename AllocT> class Alloc>
void DictionaryEntryVector<EntryT, Alloc>::clear() noexcept
{
    m_entries.clear();
    m_map.clear();
}

template<typename EntryT, template<typename AllocT> class Alloc>
std::pair<typename DictionaryEntryVector<EntryT, Alloc>::entry_type const*, bool> DictionaryEntryVector<EntryT, Alloc>::insert_or_assign(const key_type& key, value_type value)
{
    auto pair = m_map.emplace(key, m_entries.size());
    if (pair.second)
    {
        // new entry
        m_entries.emplace_back();
        auto& entry = m_entries.back();
        entry.key = pair.first->first.data();
        entry.value = std::move(value);
        return std::make_pair(&entry, true);
    }
    else
    {
        auto& entry = m_entries[pair.first->second];
        entry.value = std::move(value);
        return std::make_pair(&entry, false);
    }
}
template<typename EntryT, template<typename AllocT> class Alloc>
std::pair<typename DictionaryEntryVector<EntryT, Alloc>::entry_type const*, bool> DictionaryEntryVector<EntryT, Alloc>::insert_or_assign(const entry_type& entry)
{
    return insert_or_assign(entry.key, entry.value);
}

template <typename EntryT, typename ValueT, template<typename AllocT> class Alloc>
PointerDictionaryEntryVector<EntryT, ValueT, Alloc>& PointerDictionaryEntryVector<EntryT, ValueT, Alloc>::operator=(const PointerDictionaryEntryVector& src)
{
    DictionaryEntryVector<EntryT, Alloc>::operator=(src);
    m_values = src.m_values;
    for (auto& mapEntry : this->m_map)
    {
        m_entries[mapEntry.second].value = GetPointer(m_values[mapEntry.second]);
    }
    return *this;
}

template <typename EntryT, typename ValueT, template<typename AllocT> class Alloc>
typename PointerDictionaryEntryVector<EntryT, ValueT, Alloc>::value_type const& PointerDictionaryEntryVector<EntryT, ValueT, Alloc>::at(const key_type& key) const
{
    auto iter = m_map.find(key);
    if (iter == m_map.end())
    {
        throw std::out_of_range("");
    }
    return m_values[iter->second];
}

template <typename EntryT, typename ValueT, template<typename AllocT> class Alloc>
void PointerDictionaryEntryVector<EntryT, ValueT, Alloc>::reserve(size_type capacity) noexcept
{
    m_entries.reserve(capacity);
    m_values.reserve(capacity);
}

template <typename EntryT, typename ValueT, template<typename AllocT> class Alloc>
void PointerDictionaryEntryVector<EntryT, ValueT, Alloc>::clear() noexcept
{
    DictionaryEntryVector<EntryT, Alloc>::clear();
    m_values.clear();
}

template <typename EntryT, typename ValueT, template<typename AllocT> class Alloc>
std::pair<EntryT const*, bool> PointerDictionaryEntryVector<EntryT, ValueT, Alloc>::insert_or_assign(const key_type& key, value_type&& value)
{
    assert(m_entries.size() == m_values.size());
    auto pair = m_map.emplace(key, m_entries.size());
    if (pair.second)
    {
        // new entry
        m_entries.emplace_back();
        auto& entry = m_entries.back();
        entry.key = pair.first->first.data();

        auto remainingCapacity = m_values.capacity() - m_values.size();
        m_values.emplace_back(std::move(value));
        if (remainingCapacity)
        {
            entry.value = GetPointer(m_values.back());
        }
        else
        {
            // reallocation happened, pointers invalidated
            for (auto i = 0u; i < m_entries.size(); ++i)
            {
                m_entries[i].value = GetPointer(m_values[i]);
            }
        }

        return std::make_pair(&entry, true);
    }
    else
    {
        auto& existingValue = m_values[pair.first->second];
        auto& entry = m_entries[pair.first->second];

        existingValue = std::move(value);
        entry.value = GetPointer(existingValue);

        return std::make_pair(&entry, false);
    }
}

template <typename EntryT, typename ValueT, template<typename AllocT> class Alloc> template<typename V, typename PT, typename UU>
std::pair<EntryT const*, bool> PointerDictionaryEntryVector<EntryT, ValueT, Alloc>::insert_or_assign(const entry_type& entry)
{
    return insert_or_assign(entry.key, entry.value);
}

template<typename ModelWrapperT, template<typename AllocT> class Alloc>
ModelVector<ModelWrapperT, Alloc>::ModelVector(typename ModelWrapperT::ModelType const* const* begin, typename ModelWrapperT::ModelType const* const* end)
{
    size_t count = end - begin;
    this->m_pointers.reserve(count);
    this->m_values.reserve(count);
    for (;begin != end; ++begin)
    {
        this->push_back(typename PointerVectorType::value_type{ **begin });
    }
}

template<typename ModelWrapperT, template<typename AllocT> class Alloc>
ModelVector<ModelWrapperT, Alloc>::ModelVector(const ModelVector& src)
    : PointerVectorType{ src }
{
    std::transform(this->m_values.begin(), this->m_values.end(), this->m_pointers.begin(), std::bind(&ModelVector::GetPointer, this, std::placeholders::_1));
}

template<template<typename AllocT> class Alloc>
CStringVector<Alloc>::CStringVector(const char* const* begin, const char* const* end)
{
    size_t count = end - begin;
    this->m_pointers.reserve(count);
    this->m_values.reserve(count);
    for (; begin != end; ++begin)
    {
        this->push_back(SafeString(*begin));
    }
}

template<template<typename AllocT> class Alloc>
CStringVector<Alloc>::CStringVector(std::initializer_list<std::basic_string<char, std::char_traits<char>, Alloc<char>>> init)
{
    size_t count = init.size();
    this->m_pointers.reserve(count);
    this->m_values.reserve(count);
    for (auto&& value : init)
    {
        this->push_back(std::move(value));
    }
}

template<template<typename AllocT> class Alloc>
CStringVector<Alloc>::CStringVector(const CStringVector& src)
    : PointerVectorType{ src }
{
    std::transform(this->m_values.begin(), this->m_values.end(), this->m_pointers.begin(), std::bind(&CStringVector::GetPointer, this, std::placeholders::_1));
}

template<typename ModelWrapperT, template<typename AllocT> class Alloc>
ModelDictionaryEntryVector<ModelWrapperT, Alloc>::ModelDictionaryEntryVector(typename ModelWrapperT::DictionaryEntryType const* begin, typename ModelWrapperT::DictionaryEntryType const* end)
{
    size_t count = end - begin;
    this->m_entries.reserve(count);
    this->m_values.reserve(count);
    for (; begin != end; ++begin)
    {
        this->insert_or_assign(begin->key, *begin->value);
    }
}

template<typename ModelWrapperT, template<typename AllocT> class Alloc>
ModelDictionaryEntryVector<ModelWrapperT, Alloc>::ModelDictionaryEntryVector(const ModelDictionaryEntryVector& src)
    : PointerDictionaryEntryVectorType{ src }
{
    for (auto& mapEntry : this->m_map)
    {
        this->m_entries[mapEntry.second].value = GetPointer(this->m_values[mapEntry.second]);
    }
}

template<template<typename AllocT> class Alloc>
StringDictionaryEntryVector<Alloc>::StringDictionaryEntryVector(PFStringDictionaryEntry const* begin, PFStringDictionaryEntry const* end)
{
    size_t count = end - begin;
    this->m_entries.reserve(count);
    this->m_values.reserve(count);
    for (; begin != end; ++begin)
    {
        this->insert_or_assign(*begin);
    }
}

template<template<typename AllocT> class Alloc>
StringDictionaryEntryVector<Alloc>::StringDictionaryEntryVector(const StringDictionaryEntryVector& src)
    : PointerDictionaryEntryVectorType{ src }
{
    for (auto& mapEntry : this->m_map)
    {
        this->m_entries[mapEntry.second].value = GetPointer(this->m_values[mapEntry.second]);
    }
}

} // Wrappers
} // PlayFab
