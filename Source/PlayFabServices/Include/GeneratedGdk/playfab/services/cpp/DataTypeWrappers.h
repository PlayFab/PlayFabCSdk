// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFDataTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataAbortFileUploadsRequestWrapper : public ModelWrapper<PFDataAbortFileUploadsRequest, Alloc>
{
public:
    using ModelType = PFDataAbortFileUploadsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataAbortFileUploadsRequestWrapper() = default;

    PFDataAbortFileUploadsRequestWrapper(const PFDataAbortFileUploadsRequest& model) :
        ModelWrapper<PFDataAbortFileUploadsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? *model.entity : decltype(*model.entity){} },
        m_fileNames{ model.fileNames, model.fileNames + model.fileNamesCount },
        m_profileVersion{ model.profileVersion ? std::optional<int32_t>{ *model.profileVersion } : std::nullopt }
    {
        SetModelPointers();
    }

    PFDataAbortFileUploadsRequestWrapper(const PFDataAbortFileUploadsRequestWrapper& src) :
        PFDataAbortFileUploadsRequestWrapper{ src.Model() }
    {
    }

    PFDataAbortFileUploadsRequestWrapper(PFDataAbortFileUploadsRequestWrapper&& src) :
        PFDataAbortFileUploadsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFDataAbortFileUploadsRequestWrapper& operator=(PFDataAbortFileUploadsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataAbortFileUploadsRequestWrapper() = default;

    friend void swap(PFDataAbortFileUploadsRequestWrapper& lhs, PFDataAbortFileUploadsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_fileNames, rhs.m_fileNames);
        swap(lhs.m_profileVersion, rhs.m_profileVersion);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(PFEntityKeyWrapper<Alloc> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = &m_entity.Model();
    }

    void SetFileNames(CStringVector<Alloc> value)
    {
        m_fileNames = std::move(value);
        this->m_model.fileNames =  m_fileNames.empty() ? nullptr : m_fileNames.data();
        this->m_model.fileNamesCount =  static_cast<uint32_t>(m_fileNames.size());
    }

    void SetProfileVersion(std::optional<int32_t> value)
    {
        m_profileVersion = std::move(value);
        this->m_model.profileVersion = m_profileVersion ? m_profileVersion.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = &m_entity.Model();
        this->m_model.fileNames = m_fileNames.empty() ? nullptr : m_fileNames.data();
        this->m_model.profileVersion = m_profileVersion ? m_profileVersion.operator->() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_entity;
    CStringVector<Alloc> m_fileNames;
    std::optional<int32_t> m_profileVersion;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataAbortFileUploadsResponseWrapper : public ModelWrapper<PFDataAbortFileUploadsResponse, Alloc>
{
public:
    using ModelType = PFDataAbortFileUploadsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataAbortFileUploadsResponseWrapper() = default;

    PFDataAbortFileUploadsResponseWrapper(const PFDataAbortFileUploadsResponse& model) :
        ModelWrapper<PFDataAbortFileUploadsResponse, Alloc>{ model },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt }
    {
        SetModelPointers();
    }

    PFDataAbortFileUploadsResponseWrapper(const PFDataAbortFileUploadsResponseWrapper& src) :
        PFDataAbortFileUploadsResponseWrapper{ src.Model() }
    {
    }

    PFDataAbortFileUploadsResponseWrapper(PFDataAbortFileUploadsResponseWrapper&& src) :
        PFDataAbortFileUploadsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFDataAbortFileUploadsResponseWrapper& operator=(PFDataAbortFileUploadsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataAbortFileUploadsResponseWrapper() = default;

    friend void swap(PFDataAbortFileUploadsResponseWrapper& lhs, PFDataAbortFileUploadsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_entity, rhs.m_entity);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    void SetProfileVersion(int32_t value)
    {
        this->m_model.profileVersion = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
    }

    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataDeleteFilesRequestWrapper : public ModelWrapper<PFDataDeleteFilesRequest, Alloc>
{
public:
    using ModelType = PFDataDeleteFilesRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataDeleteFilesRequestWrapper() = default;

    PFDataDeleteFilesRequestWrapper(const PFDataDeleteFilesRequest& model) :
        ModelWrapper<PFDataDeleteFilesRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? *model.entity : decltype(*model.entity){} },
        m_fileNames{ model.fileNames, model.fileNames + model.fileNamesCount },
        m_profileVersion{ model.profileVersion ? std::optional<int32_t>{ *model.profileVersion } : std::nullopt }
    {
        SetModelPointers();
    }

    PFDataDeleteFilesRequestWrapper(const PFDataDeleteFilesRequestWrapper& src) :
        PFDataDeleteFilesRequestWrapper{ src.Model() }
    {
    }

    PFDataDeleteFilesRequestWrapper(PFDataDeleteFilesRequestWrapper&& src) :
        PFDataDeleteFilesRequestWrapper{}
    {
        swap(*this, src);
    }

    PFDataDeleteFilesRequestWrapper& operator=(PFDataDeleteFilesRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataDeleteFilesRequestWrapper() = default;

    friend void swap(PFDataDeleteFilesRequestWrapper& lhs, PFDataDeleteFilesRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_fileNames, rhs.m_fileNames);
        swap(lhs.m_profileVersion, rhs.m_profileVersion);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(PFEntityKeyWrapper<Alloc> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = &m_entity.Model();
    }

    void SetFileNames(CStringVector<Alloc> value)
    {
        m_fileNames = std::move(value);
        this->m_model.fileNames =  m_fileNames.empty() ? nullptr : m_fileNames.data();
        this->m_model.fileNamesCount =  static_cast<uint32_t>(m_fileNames.size());
    }

    void SetProfileVersion(std::optional<int32_t> value)
    {
        m_profileVersion = std::move(value);
        this->m_model.profileVersion = m_profileVersion ? m_profileVersion.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = &m_entity.Model();
        this->m_model.fileNames = m_fileNames.empty() ? nullptr : m_fileNames.data();
        this->m_model.profileVersion = m_profileVersion ? m_profileVersion.operator->() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_entity;
    CStringVector<Alloc> m_fileNames;
    std::optional<int32_t> m_profileVersion;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataDeleteFilesResponseWrapper : public ModelWrapper<PFDataDeleteFilesResponse, Alloc>
{
public:
    using ModelType = PFDataDeleteFilesResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataDeleteFilesResponseWrapper() = default;

    PFDataDeleteFilesResponseWrapper(const PFDataDeleteFilesResponse& model) :
        ModelWrapper<PFDataDeleteFilesResponse, Alloc>{ model },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt }
    {
        SetModelPointers();
    }

    PFDataDeleteFilesResponseWrapper(const PFDataDeleteFilesResponseWrapper& src) :
        PFDataDeleteFilesResponseWrapper{ src.Model() }
    {
    }

    PFDataDeleteFilesResponseWrapper(PFDataDeleteFilesResponseWrapper&& src) :
        PFDataDeleteFilesResponseWrapper{}
    {
        swap(*this, src);
    }

    PFDataDeleteFilesResponseWrapper& operator=(PFDataDeleteFilesResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataDeleteFilesResponseWrapper() = default;

    friend void swap(PFDataDeleteFilesResponseWrapper& lhs, PFDataDeleteFilesResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_entity, rhs.m_entity);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    void SetProfileVersion(int32_t value)
    {
        this->m_model.profileVersion = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
    }

    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataFinalizeFileUploadsRequestWrapper : public ModelWrapper<PFDataFinalizeFileUploadsRequest, Alloc>
{
public:
    using ModelType = PFDataFinalizeFileUploadsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataFinalizeFileUploadsRequestWrapper() = default;

    PFDataFinalizeFileUploadsRequestWrapper(const PFDataFinalizeFileUploadsRequest& model) :
        ModelWrapper<PFDataFinalizeFileUploadsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? *model.entity : decltype(*model.entity){} },
        m_fileNames{ model.fileNames, model.fileNames + model.fileNamesCount }
    {
        SetModelPointers();
    }

    PFDataFinalizeFileUploadsRequestWrapper(const PFDataFinalizeFileUploadsRequestWrapper& src) :
        PFDataFinalizeFileUploadsRequestWrapper{ src.Model() }
    {
    }

    PFDataFinalizeFileUploadsRequestWrapper(PFDataFinalizeFileUploadsRequestWrapper&& src) :
        PFDataFinalizeFileUploadsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFDataFinalizeFileUploadsRequestWrapper& operator=(PFDataFinalizeFileUploadsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataFinalizeFileUploadsRequestWrapper() = default;

    friend void swap(PFDataFinalizeFileUploadsRequestWrapper& lhs, PFDataFinalizeFileUploadsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_fileNames, rhs.m_fileNames);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(PFEntityKeyWrapper<Alloc> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = &m_entity.Model();
    }

    void SetFileNames(CStringVector<Alloc> value)
    {
        m_fileNames = std::move(value);
        this->m_model.fileNames =  m_fileNames.empty() ? nullptr : m_fileNames.data();
        this->m_model.fileNamesCount =  static_cast<uint32_t>(m_fileNames.size());
    }

    void SetProfileVersion(int32_t value)
    {
        this->m_model.profileVersion = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = &m_entity.Model();
        this->m_model.fileNames = m_fileNames.empty() ? nullptr : m_fileNames.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_entity;
    CStringVector<Alloc> m_fileNames;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataGetFileMetadataWrapper : public ModelWrapper<PFDataGetFileMetadata, Alloc>
{
public:
    using ModelType = PFDataGetFileMetadata;
    using DictionaryEntryType = PFDataGetFileMetadataDictionaryEntry;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataGetFileMetadataWrapper() = default;

    PFDataGetFileMetadataWrapper(const PFDataGetFileMetadata& model) :
        ModelWrapper<PFDataGetFileMetadata, Alloc>{ model },
        m_checksum{ SafeString(model.checksum) },
        m_downloadUrl{ SafeString(model.downloadUrl) },
        m_fileName{ SafeString(model.fileName) }
    {
        SetModelPointers();
    }

    PFDataGetFileMetadataWrapper(const PFDataGetFileMetadataWrapper& src) :
        PFDataGetFileMetadataWrapper{ src.Model() }
    {
    }

    PFDataGetFileMetadataWrapper(PFDataGetFileMetadataWrapper&& src) :
        PFDataGetFileMetadataWrapper{}
    {
        swap(*this, src);
    }

    PFDataGetFileMetadataWrapper& operator=(PFDataGetFileMetadataWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataGetFileMetadataWrapper() = default;

    friend void swap(PFDataGetFileMetadataWrapper& lhs, PFDataGetFileMetadataWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_checksum, rhs.m_checksum);
        swap(lhs.m_downloadUrl, rhs.m_downloadUrl);
        swap(lhs.m_fileName, rhs.m_fileName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetChecksum(String value)
    {
        m_checksum = std::move(value);
        this->m_model.checksum =  m_checksum.empty() ? nullptr : m_checksum.data();
    }

    void SetDownloadUrl(String value)
    {
        m_downloadUrl = std::move(value);
        this->m_model.downloadUrl =  m_downloadUrl.empty() ? nullptr : m_downloadUrl.data();
    }

    void SetFileName(String value)
    {
        m_fileName = std::move(value);
        this->m_model.fileName =  m_fileName.empty() ? nullptr : m_fileName.data();
    }

    void SetLastModified(time_t value)
    {
        this->m_model.lastModified = value;
    }

    void SetSize(int32_t value)
    {
        this->m_model.size = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.checksum = m_checksum.empty() ? nullptr : m_checksum.data();
        this->m_model.downloadUrl = m_downloadUrl.empty() ? nullptr : m_downloadUrl.data();
        this->m_model.fileName = m_fileName.empty() ? nullptr : m_fileName.data();
    }

    String m_checksum;
    String m_downloadUrl;
    String m_fileName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataFinalizeFileUploadsResponseWrapper : public ModelWrapper<PFDataFinalizeFileUploadsResponse, Alloc>
{
public:
    using ModelType = PFDataFinalizeFileUploadsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataFinalizeFileUploadsResponseWrapper() = default;

    PFDataFinalizeFileUploadsResponseWrapper(const PFDataFinalizeFileUploadsResponse& model) :
        ModelWrapper<PFDataFinalizeFileUploadsResponse, Alloc>{ model },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_metadata{ model.metadata, model.metadata + model.metadataCount }
    {
        SetModelPointers();
    }

    PFDataFinalizeFileUploadsResponseWrapper(const PFDataFinalizeFileUploadsResponseWrapper& src) :
        PFDataFinalizeFileUploadsResponseWrapper{ src.Model() }
    {
    }

    PFDataFinalizeFileUploadsResponseWrapper(PFDataFinalizeFileUploadsResponseWrapper&& src) :
        PFDataFinalizeFileUploadsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFDataFinalizeFileUploadsResponseWrapper& operator=(PFDataFinalizeFileUploadsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataFinalizeFileUploadsResponseWrapper() = default;

    friend void swap(PFDataFinalizeFileUploadsResponseWrapper& lhs, PFDataFinalizeFileUploadsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_metadata, rhs.m_metadata);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    void SetMetadata(ModelDictionaryEntryVector<PFDataGetFileMetadataWrapper<Alloc>, Alloc> value)
    {
        m_metadata = std::move(value);
        this->m_model.metadata =  m_metadata.empty() ? nullptr : m_metadata.data();
        this->m_model.metadataCount =  static_cast<uint32_t>(m_metadata.size());
    }

    void SetProfileVersion(int32_t value)
    {
        this->m_model.profileVersion = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.metadata = m_metadata.empty() ? nullptr : m_metadata.data();
    }

    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    ModelDictionaryEntryVector<PFDataGetFileMetadataWrapper<Alloc>, Alloc> m_metadata;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataGetFilesRequestWrapper : public ModelWrapper<PFDataGetFilesRequest, Alloc>
{
public:
    using ModelType = PFDataGetFilesRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataGetFilesRequestWrapper() = default;

    PFDataGetFilesRequestWrapper(const PFDataGetFilesRequest& model) :
        ModelWrapper<PFDataGetFilesRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? *model.entity : decltype(*model.entity){} }
    {
        SetModelPointers();
    }

    PFDataGetFilesRequestWrapper(const PFDataGetFilesRequestWrapper& src) :
        PFDataGetFilesRequestWrapper{ src.Model() }
    {
    }

    PFDataGetFilesRequestWrapper(PFDataGetFilesRequestWrapper&& src) :
        PFDataGetFilesRequestWrapper{}
    {
        swap(*this, src);
    }

    PFDataGetFilesRequestWrapper& operator=(PFDataGetFilesRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataGetFilesRequestWrapper() = default;

    friend void swap(PFDataGetFilesRequestWrapper& lhs, PFDataGetFilesRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(PFEntityKeyWrapper<Alloc> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = &m_entity.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = &m_entity.Model();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_entity;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataGetFilesResponseWrapper : public ModelWrapper<PFDataGetFilesResponse, Alloc>
{
public:
    using ModelType = PFDataGetFilesResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataGetFilesResponseWrapper() = default;

    PFDataGetFilesResponseWrapper(const PFDataGetFilesResponse& model) :
        ModelWrapper<PFDataGetFilesResponse, Alloc>{ model },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_metadata{ model.metadata, model.metadata + model.metadataCount }
    {
        SetModelPointers();
    }

    PFDataGetFilesResponseWrapper(const PFDataGetFilesResponseWrapper& src) :
        PFDataGetFilesResponseWrapper{ src.Model() }
    {
    }

    PFDataGetFilesResponseWrapper(PFDataGetFilesResponseWrapper&& src) :
        PFDataGetFilesResponseWrapper{}
    {
        swap(*this, src);
    }

    PFDataGetFilesResponseWrapper& operator=(PFDataGetFilesResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataGetFilesResponseWrapper() = default;

    friend void swap(PFDataGetFilesResponseWrapper& lhs, PFDataGetFilesResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_metadata, rhs.m_metadata);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    void SetMetadata(ModelDictionaryEntryVector<PFDataGetFileMetadataWrapper<Alloc>, Alloc> value)
    {
        m_metadata = std::move(value);
        this->m_model.metadata =  m_metadata.empty() ? nullptr : m_metadata.data();
        this->m_model.metadataCount =  static_cast<uint32_t>(m_metadata.size());
    }

    void SetProfileVersion(int32_t value)
    {
        this->m_model.profileVersion = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.metadata = m_metadata.empty() ? nullptr : m_metadata.data();
    }

    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    ModelDictionaryEntryVector<PFDataGetFileMetadataWrapper<Alloc>, Alloc> m_metadata;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataGetObjectsRequestWrapper : public ModelWrapper<PFDataGetObjectsRequest, Alloc>
{
public:
    using ModelType = PFDataGetObjectsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataGetObjectsRequestWrapper() = default;

    PFDataGetObjectsRequestWrapper(const PFDataGetObjectsRequest& model) :
        ModelWrapper<PFDataGetObjectsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? *model.entity : decltype(*model.entity){} },
        m_escapeObject{ model.escapeObject ? std::optional<bool>{ *model.escapeObject } : std::nullopt }
    {
        SetModelPointers();
    }

    PFDataGetObjectsRequestWrapper(const PFDataGetObjectsRequestWrapper& src) :
        PFDataGetObjectsRequestWrapper{ src.Model() }
    {
    }

    PFDataGetObjectsRequestWrapper(PFDataGetObjectsRequestWrapper&& src) :
        PFDataGetObjectsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFDataGetObjectsRequestWrapper& operator=(PFDataGetObjectsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataGetObjectsRequestWrapper() = default;

    friend void swap(PFDataGetObjectsRequestWrapper& lhs, PFDataGetObjectsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_escapeObject, rhs.m_escapeObject);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(PFEntityKeyWrapper<Alloc> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = &m_entity.Model();
    }

    void SetEscapeObject(std::optional<bool> value)
    {
        m_escapeObject = std::move(value);
        this->m_model.escapeObject = m_escapeObject ? m_escapeObject.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = &m_entity.Model();
        this->m_model.escapeObject = m_escapeObject ? m_escapeObject.operator->() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_entity;
    std::optional<bool> m_escapeObject;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataObjectResultWrapper : public ModelWrapper<PFDataObjectResult, Alloc>
{
public:
    using ModelType = PFDataObjectResult;
    using DictionaryEntryType = PFDataObjectResultDictionaryEntry;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataObjectResultWrapper() = default;

    PFDataObjectResultWrapper(const PFDataObjectResult& model) :
        ModelWrapper<PFDataObjectResult, Alloc>{ model },
        m_dataObject{ model.dataObject },
        m_escapedDataObject{ SafeString(model.escapedDataObject) },
        m_objectName{ SafeString(model.objectName) }
    {
        SetModelPointers();
    }

    PFDataObjectResultWrapper(const PFDataObjectResultWrapper& src) :
        PFDataObjectResultWrapper{ src.Model() }
    {
    }

    PFDataObjectResultWrapper(PFDataObjectResultWrapper&& src) :
        PFDataObjectResultWrapper{}
    {
        swap(*this, src);
    }

    PFDataObjectResultWrapper& operator=(PFDataObjectResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataObjectResultWrapper() = default;

    friend void swap(PFDataObjectResultWrapper& lhs, PFDataObjectResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_dataObject, rhs.m_dataObject);
        swap(lhs.m_escapedDataObject, rhs.m_escapedDataObject);
        swap(lhs.m_objectName, rhs.m_objectName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetDataObject(JsonObject<Alloc> value)
    {
        m_dataObject = std::move(value);
        this->m_model.dataObject.stringValue = m_dataObject.stringValue.empty() ? nullptr : m_dataObject.stringValue.data();
    }

    void SetEscapedDataObject(String value)
    {
        m_escapedDataObject = std::move(value);
        this->m_model.escapedDataObject =  m_escapedDataObject.empty() ? nullptr : m_escapedDataObject.data();
    }

    void SetObjectName(String value)
    {
        m_objectName = std::move(value);
        this->m_model.objectName =  m_objectName.empty() ? nullptr : m_objectName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.dataObject.stringValue = m_dataObject.stringValue.empty() ? nullptr : m_dataObject.stringValue.data();
        this->m_model.escapedDataObject = m_escapedDataObject.empty() ? nullptr : m_escapedDataObject.data();
        this->m_model.objectName = m_objectName.empty() ? nullptr : m_objectName.data();
    }

    JsonObject<Alloc> m_dataObject;
    String m_escapedDataObject;
    String m_objectName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataGetObjectsResponseWrapper : public ModelWrapper<PFDataGetObjectsResponse, Alloc>
{
public:
    using ModelType = PFDataGetObjectsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataGetObjectsResponseWrapper() = default;

    PFDataGetObjectsResponseWrapper(const PFDataGetObjectsResponse& model) :
        ModelWrapper<PFDataGetObjectsResponse, Alloc>{ model },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_objects{ model.objects, model.objects + model.objectsCount }
    {
        SetModelPointers();
    }

    PFDataGetObjectsResponseWrapper(const PFDataGetObjectsResponseWrapper& src) :
        PFDataGetObjectsResponseWrapper{ src.Model() }
    {
    }

    PFDataGetObjectsResponseWrapper(PFDataGetObjectsResponseWrapper&& src) :
        PFDataGetObjectsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFDataGetObjectsResponseWrapper& operator=(PFDataGetObjectsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataGetObjectsResponseWrapper() = default;

    friend void swap(PFDataGetObjectsResponseWrapper& lhs, PFDataGetObjectsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_objects, rhs.m_objects);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    void SetObjects(ModelDictionaryEntryVector<PFDataObjectResultWrapper<Alloc>, Alloc> value)
    {
        m_objects = std::move(value);
        this->m_model.objects =  m_objects.empty() ? nullptr : m_objects.data();
        this->m_model.objectsCount =  static_cast<uint32_t>(m_objects.size());
    }

    void SetProfileVersion(int32_t value)
    {
        this->m_model.profileVersion = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.objects = m_objects.empty() ? nullptr : m_objects.data();
    }

    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    ModelDictionaryEntryVector<PFDataObjectResultWrapper<Alloc>, Alloc> m_objects;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataInitiateFileUploadsRequestWrapper : public ModelWrapper<PFDataInitiateFileUploadsRequest, Alloc>
{
public:
    using ModelType = PFDataInitiateFileUploadsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataInitiateFileUploadsRequestWrapper() = default;

    PFDataInitiateFileUploadsRequestWrapper(const PFDataInitiateFileUploadsRequest& model) :
        ModelWrapper<PFDataInitiateFileUploadsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? *model.entity : decltype(*model.entity){} },
        m_fileNames{ model.fileNames, model.fileNames + model.fileNamesCount },
        m_profileVersion{ model.profileVersion ? std::optional<int32_t>{ *model.profileVersion } : std::nullopt }
    {
        SetModelPointers();
    }

    PFDataInitiateFileUploadsRequestWrapper(const PFDataInitiateFileUploadsRequestWrapper& src) :
        PFDataInitiateFileUploadsRequestWrapper{ src.Model() }
    {
    }

    PFDataInitiateFileUploadsRequestWrapper(PFDataInitiateFileUploadsRequestWrapper&& src) :
        PFDataInitiateFileUploadsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFDataInitiateFileUploadsRequestWrapper& operator=(PFDataInitiateFileUploadsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataInitiateFileUploadsRequestWrapper() = default;

    friend void swap(PFDataInitiateFileUploadsRequestWrapper& lhs, PFDataInitiateFileUploadsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_fileNames, rhs.m_fileNames);
        swap(lhs.m_profileVersion, rhs.m_profileVersion);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(PFEntityKeyWrapper<Alloc> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = &m_entity.Model();
    }

    void SetFileNames(CStringVector<Alloc> value)
    {
        m_fileNames = std::move(value);
        this->m_model.fileNames =  m_fileNames.empty() ? nullptr : m_fileNames.data();
        this->m_model.fileNamesCount =  static_cast<uint32_t>(m_fileNames.size());
    }

    void SetProfileVersion(std::optional<int32_t> value)
    {
        m_profileVersion = std::move(value);
        this->m_model.profileVersion = m_profileVersion ? m_profileVersion.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = &m_entity.Model();
        this->m_model.fileNames = m_fileNames.empty() ? nullptr : m_fileNames.data();
        this->m_model.profileVersion = m_profileVersion ? m_profileVersion.operator->() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_entity;
    CStringVector<Alloc> m_fileNames;
    std::optional<int32_t> m_profileVersion;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataInitiateFileUploadMetadataWrapper : public ModelWrapper<PFDataInitiateFileUploadMetadata, Alloc>
{
public:
    using ModelType = PFDataInitiateFileUploadMetadata;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataInitiateFileUploadMetadataWrapper() = default;

    PFDataInitiateFileUploadMetadataWrapper(const PFDataInitiateFileUploadMetadata& model) :
        ModelWrapper<PFDataInitiateFileUploadMetadata, Alloc>{ model },
        m_fileName{ SafeString(model.fileName) },
        m_uploadUrl{ SafeString(model.uploadUrl) }
    {
        SetModelPointers();
    }

    PFDataInitiateFileUploadMetadataWrapper(const PFDataInitiateFileUploadMetadataWrapper& src) :
        PFDataInitiateFileUploadMetadataWrapper{ src.Model() }
    {
    }

    PFDataInitiateFileUploadMetadataWrapper(PFDataInitiateFileUploadMetadataWrapper&& src) :
        PFDataInitiateFileUploadMetadataWrapper{}
    {
        swap(*this, src);
    }

    PFDataInitiateFileUploadMetadataWrapper& operator=(PFDataInitiateFileUploadMetadataWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataInitiateFileUploadMetadataWrapper() = default;

    friend void swap(PFDataInitiateFileUploadMetadataWrapper& lhs, PFDataInitiateFileUploadMetadataWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_fileName, rhs.m_fileName);
        swap(lhs.m_uploadUrl, rhs.m_uploadUrl);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFileName(String value)
    {
        m_fileName = std::move(value);
        this->m_model.fileName =  m_fileName.empty() ? nullptr : m_fileName.data();
    }

    void SetUploadUrl(String value)
    {
        m_uploadUrl = std::move(value);
        this->m_model.uploadUrl =  m_uploadUrl.empty() ? nullptr : m_uploadUrl.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.fileName = m_fileName.empty() ? nullptr : m_fileName.data();
        this->m_model.uploadUrl = m_uploadUrl.empty() ? nullptr : m_uploadUrl.data();
    }

    String m_fileName;
    String m_uploadUrl;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataInitiateFileUploadsResponseWrapper : public ModelWrapper<PFDataInitiateFileUploadsResponse, Alloc>
{
public:
    using ModelType = PFDataInitiateFileUploadsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataInitiateFileUploadsResponseWrapper() = default;

    PFDataInitiateFileUploadsResponseWrapper(const PFDataInitiateFileUploadsResponse& model) :
        ModelWrapper<PFDataInitiateFileUploadsResponse, Alloc>{ model },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_uploadDetails{ model.uploadDetails, model.uploadDetails + model.uploadDetailsCount }
    {
        SetModelPointers();
    }

    PFDataInitiateFileUploadsResponseWrapper(const PFDataInitiateFileUploadsResponseWrapper& src) :
        PFDataInitiateFileUploadsResponseWrapper{ src.Model() }
    {
    }

    PFDataInitiateFileUploadsResponseWrapper(PFDataInitiateFileUploadsResponseWrapper&& src) :
        PFDataInitiateFileUploadsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFDataInitiateFileUploadsResponseWrapper& operator=(PFDataInitiateFileUploadsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataInitiateFileUploadsResponseWrapper() = default;

    friend void swap(PFDataInitiateFileUploadsResponseWrapper& lhs, PFDataInitiateFileUploadsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_uploadDetails, rhs.m_uploadDetails);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    void SetProfileVersion(int32_t value)
    {
        this->m_model.profileVersion = value;
    }

    void SetUploadDetails(ModelVector<PFDataInitiateFileUploadMetadataWrapper<Alloc>, Alloc> value)
    {
        m_uploadDetails = std::move(value);
        this->m_model.uploadDetails =  m_uploadDetails.empty() ? nullptr : m_uploadDetails.data();
        this->m_model.uploadDetailsCount =  static_cast<uint32_t>(m_uploadDetails.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.uploadDetails = m_uploadDetails.empty() ? nullptr : m_uploadDetails.data();
    }

    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    ModelVector<PFDataInitiateFileUploadMetadataWrapper<Alloc>, Alloc> m_uploadDetails;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataSetObjectWrapper : public ModelWrapper<PFDataSetObject, Alloc>
{
public:
    using ModelType = PFDataSetObject;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataSetObjectWrapper() = default;

    PFDataSetObjectWrapper(const PFDataSetObject& model) :
        ModelWrapper<PFDataSetObject, Alloc>{ model },
        m_dataObject{ model.dataObject },
        m_deleteObject{ model.deleteObject ? std::optional<bool>{ *model.deleteObject } : std::nullopt },
        m_escapedDataObject{ SafeString(model.escapedDataObject) },
        m_objectName{ SafeString(model.objectName) }
    {
        SetModelPointers();
    }

    PFDataSetObjectWrapper(const PFDataSetObjectWrapper& src) :
        PFDataSetObjectWrapper{ src.Model() }
    {
    }

    PFDataSetObjectWrapper(PFDataSetObjectWrapper&& src) :
        PFDataSetObjectWrapper{}
    {
        swap(*this, src);
    }

    PFDataSetObjectWrapper& operator=(PFDataSetObjectWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataSetObjectWrapper() = default;

    friend void swap(PFDataSetObjectWrapper& lhs, PFDataSetObjectWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_dataObject, rhs.m_dataObject);
        swap(lhs.m_deleteObject, rhs.m_deleteObject);
        swap(lhs.m_escapedDataObject, rhs.m_escapedDataObject);
        swap(lhs.m_objectName, rhs.m_objectName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetDataObject(JsonObject<Alloc> value)
    {
        m_dataObject = std::move(value);
        this->m_model.dataObject.stringValue = m_dataObject.stringValue.empty() ? nullptr : m_dataObject.stringValue.data();
    }

    void SetDeleteObject(std::optional<bool> value)
    {
        m_deleteObject = std::move(value);
        this->m_model.deleteObject = m_deleteObject ? m_deleteObject.operator->() : nullptr;
    }

    void SetEscapedDataObject(String value)
    {
        m_escapedDataObject = std::move(value);
        this->m_model.escapedDataObject =  m_escapedDataObject.empty() ? nullptr : m_escapedDataObject.data();
    }

    void SetObjectName(String value)
    {
        m_objectName = std::move(value);
        this->m_model.objectName =  m_objectName.empty() ? nullptr : m_objectName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.dataObject.stringValue = m_dataObject.stringValue.empty() ? nullptr : m_dataObject.stringValue.data();
        this->m_model.deleteObject = m_deleteObject ? m_deleteObject.operator->() : nullptr;
        this->m_model.escapedDataObject = m_escapedDataObject.empty() ? nullptr : m_escapedDataObject.data();
        this->m_model.objectName = m_objectName.empty() ? nullptr : m_objectName.data();
    }

    JsonObject<Alloc> m_dataObject;
    std::optional<bool> m_deleteObject;
    String m_escapedDataObject;
    String m_objectName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataSetObjectsRequestWrapper : public ModelWrapper<PFDataSetObjectsRequest, Alloc>
{
public:
    using ModelType = PFDataSetObjectsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataSetObjectsRequestWrapper() = default;

    PFDataSetObjectsRequestWrapper(const PFDataSetObjectsRequest& model) :
        ModelWrapper<PFDataSetObjectsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? *model.entity : decltype(*model.entity){} },
        m_expectedProfileVersion{ model.expectedProfileVersion ? std::optional<int32_t>{ *model.expectedProfileVersion } : std::nullopt },
        m_objects{ model.objects, model.objects + model.objectsCount }
    {
        SetModelPointers();
    }

    PFDataSetObjectsRequestWrapper(const PFDataSetObjectsRequestWrapper& src) :
        PFDataSetObjectsRequestWrapper{ src.Model() }
    {
    }

    PFDataSetObjectsRequestWrapper(PFDataSetObjectsRequestWrapper&& src) :
        PFDataSetObjectsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFDataSetObjectsRequestWrapper& operator=(PFDataSetObjectsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataSetObjectsRequestWrapper() = default;

    friend void swap(PFDataSetObjectsRequestWrapper& lhs, PFDataSetObjectsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_expectedProfileVersion, rhs.m_expectedProfileVersion);
        swap(lhs.m_objects, rhs.m_objects);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(PFEntityKeyWrapper<Alloc> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = &m_entity.Model();
    }

    void SetExpectedProfileVersion(std::optional<int32_t> value)
    {
        m_expectedProfileVersion = std::move(value);
        this->m_model.expectedProfileVersion = m_expectedProfileVersion ? m_expectedProfileVersion.operator->() : nullptr;
    }

    void SetObjects(ModelVector<PFDataSetObjectWrapper<Alloc>, Alloc> value)
    {
        m_objects = std::move(value);
        this->m_model.objects =  m_objects.empty() ? nullptr : m_objects.data();
        this->m_model.objectsCount =  static_cast<uint32_t>(m_objects.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = &m_entity.Model();
        this->m_model.expectedProfileVersion = m_expectedProfileVersion ? m_expectedProfileVersion.operator->() : nullptr;
        this->m_model.objects = m_objects.empty() ? nullptr : m_objects.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_entity;
    std::optional<int32_t> m_expectedProfileVersion;
    ModelVector<PFDataSetObjectWrapper<Alloc>, Alloc> m_objects;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataSetObjectInfoWrapper : public ModelWrapper<PFDataSetObjectInfo, Alloc>
{
public:
    using ModelType = PFDataSetObjectInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataSetObjectInfoWrapper() = default;

    PFDataSetObjectInfoWrapper(const PFDataSetObjectInfo& model) :
        ModelWrapper<PFDataSetObjectInfo, Alloc>{ model },
        m_objectName{ SafeString(model.objectName) },
        m_operationReason{ SafeString(model.operationReason) },
        m_setResult{ model.setResult ? std::optional<PFOperationTypes>{ *model.setResult } : std::nullopt }
    {
        SetModelPointers();
    }

    PFDataSetObjectInfoWrapper(const PFDataSetObjectInfoWrapper& src) :
        PFDataSetObjectInfoWrapper{ src.Model() }
    {
    }

    PFDataSetObjectInfoWrapper(PFDataSetObjectInfoWrapper&& src) :
        PFDataSetObjectInfoWrapper{}
    {
        swap(*this, src);
    }

    PFDataSetObjectInfoWrapper& operator=(PFDataSetObjectInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataSetObjectInfoWrapper() = default;

    friend void swap(PFDataSetObjectInfoWrapper& lhs, PFDataSetObjectInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_objectName, rhs.m_objectName);
        swap(lhs.m_operationReason, rhs.m_operationReason);
        swap(lhs.m_setResult, rhs.m_setResult);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetObjectName(String value)
    {
        m_objectName = std::move(value);
        this->m_model.objectName =  m_objectName.empty() ? nullptr : m_objectName.data();
    }

    void SetOperationReason(String value)
    {
        m_operationReason = std::move(value);
        this->m_model.operationReason =  m_operationReason.empty() ? nullptr : m_operationReason.data();
    }

    void SetSetResult(std::optional<PFOperationTypes> value)
    {
        m_setResult = std::move(value);
        this->m_model.setResult = m_setResult ? m_setResult.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.objectName = m_objectName.empty() ? nullptr : m_objectName.data();
        this->m_model.operationReason = m_operationReason.empty() ? nullptr : m_operationReason.data();
        this->m_model.setResult = m_setResult ? m_setResult.operator->() : nullptr;
    }

    String m_objectName;
    String m_operationReason;
    std::optional<PFOperationTypes> m_setResult;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFDataSetObjectsResponseWrapper : public ModelWrapper<PFDataSetObjectsResponse, Alloc>
{
public:
    using ModelType = PFDataSetObjectsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFDataSetObjectsResponseWrapper() = default;

    PFDataSetObjectsResponseWrapper(const PFDataSetObjectsResponse& model) :
        ModelWrapper<PFDataSetObjectsResponse, Alloc>{ model },
        m_setResults{ model.setResults, model.setResults + model.setResultsCount }
    {
        SetModelPointers();
    }

    PFDataSetObjectsResponseWrapper(const PFDataSetObjectsResponseWrapper& src) :
        PFDataSetObjectsResponseWrapper{ src.Model() }
    {
    }

    PFDataSetObjectsResponseWrapper(PFDataSetObjectsResponseWrapper&& src) :
        PFDataSetObjectsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFDataSetObjectsResponseWrapper& operator=(PFDataSetObjectsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFDataSetObjectsResponseWrapper() = default;

    friend void swap(PFDataSetObjectsResponseWrapper& lhs, PFDataSetObjectsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_setResults, rhs.m_setResults);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetProfileVersion(int32_t value)
    {
        this->m_model.profileVersion = value;
    }

    void SetSetResults(ModelVector<PFDataSetObjectInfoWrapper<Alloc>, Alloc> value)
    {
        m_setResults = std::move(value);
        this->m_model.setResults =  m_setResults.empty() ? nullptr : m_setResults.data();
        this->m_model.setResultsCount =  static_cast<uint32_t>(m_setResults.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.setResults = m_setResults.empty() ? nullptr : m_setResults.data();
    }

    ModelVector<PFDataSetObjectInfoWrapper<Alloc>, Alloc> m_setResults;
};

} // namespace Wrappers
} // namespace PlayFab
