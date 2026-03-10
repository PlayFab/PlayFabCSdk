// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include "InternalPFGameSaveTypes.h"


namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveDeleteManifestRequestWrapper : public ModelWrapper<PFGameSaveDeleteManifestRequest, Alloc>
{
public:
    using ModelType = PFGameSaveDeleteManifestRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveDeleteManifestRequestWrapper() = default;

    PFGameSaveDeleteManifestRequestWrapper(const PFGameSaveDeleteManifestRequest& model) :
        ModelWrapper<PFGameSaveDeleteManifestRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_version{ SafeString(model.version) }
    {
        SetModelPointers();
    }

    PFGameSaveDeleteManifestRequestWrapper(const PFGameSaveDeleteManifestRequestWrapper& src) :
        PFGameSaveDeleteManifestRequestWrapper{ src.Model() }
    {
    }

    PFGameSaveDeleteManifestRequestWrapper(PFGameSaveDeleteManifestRequestWrapper&& src) :
        PFGameSaveDeleteManifestRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveDeleteManifestRequestWrapper& operator=(PFGameSaveDeleteManifestRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveDeleteManifestRequestWrapper() = default;

    friend void swap(PFGameSaveDeleteManifestRequestWrapper& lhs, PFGameSaveDeleteManifestRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_version, rhs.m_version);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    String const& GetVersion() const
    {
        return m_version;
    }

    void SetVersion(String value)
    {
        m_version = std::move(value);
        this->m_model.version =  m_version.empty() ? nullptr : m_version.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.version = m_version.empty() ? nullptr : m_version.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_version;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveFinalizeManifestConflictWrapper : public ModelWrapper<PFGameSaveFinalizeManifestConflict, Alloc>
{
public:
    using ModelType = PFGameSaveFinalizeManifestConflict;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveFinalizeManifestConflictWrapper() = default;

    PFGameSaveFinalizeManifestConflictWrapper(const PFGameSaveFinalizeManifestConflict& model) :
        ModelWrapper<PFGameSaveFinalizeManifestConflict, Alloc>{ model },
        m_conflictingVersion{ SafeString(model.conflictingVersion) }
    {
        SetModelPointers();
    }

    PFGameSaveFinalizeManifestConflictWrapper(const PFGameSaveFinalizeManifestConflictWrapper& src) :
        PFGameSaveFinalizeManifestConflictWrapper{ src.Model() }
    {
    }

    PFGameSaveFinalizeManifestConflictWrapper(PFGameSaveFinalizeManifestConflictWrapper&& src) :
        PFGameSaveFinalizeManifestConflictWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveFinalizeManifestConflictWrapper& operator=(PFGameSaveFinalizeManifestConflictWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveFinalizeManifestConflictWrapper() = default;

    friend void swap(PFGameSaveFinalizeManifestConflictWrapper& lhs, PFGameSaveFinalizeManifestConflictWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_conflictingVersion, rhs.m_conflictingVersion);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetConflictingVersion() const
    {
        return m_conflictingVersion;
    }

    void SetConflictingVersion(String value)
    {
        m_conflictingVersion = std::move(value);
        this->m_model.conflictingVersion =  m_conflictingVersion.empty() ? nullptr : m_conflictingVersion.data();
    }

    bool GetIsWinner() const
    {
        return this->m_model.isWinner;
    }

    void SetIsWinner(bool value)
    {
        this->m_model.isWinner = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.conflictingVersion = m_conflictingVersion.empty() ? nullptr : m_conflictingVersion.data();
    }

    String m_conflictingVersion;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveFinalizedFileDetailsWrapper : public ModelWrapper<PFGameSaveFinalizedFileDetails, Alloc>
{
public:
    using ModelType = PFGameSaveFinalizedFileDetails;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveFinalizedFileDetailsWrapper() = default;

    PFGameSaveFinalizedFileDetailsWrapper(const PFGameSaveFinalizedFileDetails& model) :
        ModelWrapper<PFGameSaveFinalizedFileDetails, Alloc>{ model },
        m_fileName{ SafeString(model.fileName) },
        m_fileSizeBytes{ SafeString(model.fileSizeBytes) }
    {
        SetModelPointers();
    }

    PFGameSaveFinalizedFileDetailsWrapper(const PFGameSaveFinalizedFileDetailsWrapper& src) :
        PFGameSaveFinalizedFileDetailsWrapper{ src.Model() }
    {
    }

    PFGameSaveFinalizedFileDetailsWrapper(PFGameSaveFinalizedFileDetailsWrapper&& src) :
        PFGameSaveFinalizedFileDetailsWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveFinalizedFileDetailsWrapper& operator=(PFGameSaveFinalizedFileDetailsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveFinalizedFileDetailsWrapper() = default;

    friend void swap(PFGameSaveFinalizedFileDetailsWrapper& lhs, PFGameSaveFinalizedFileDetailsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_fileName, rhs.m_fileName);
        swap(lhs.m_fileSizeBytes, rhs.m_fileSizeBytes);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetFileName() const
    {
        return m_fileName;
    }

    void SetFileName(String value)
    {
        m_fileName = std::move(value);
        this->m_model.fileName =  m_fileName.empty() ? nullptr : m_fileName.data();
    }

    String const& GetFileSizeBytes() const
    {
        return m_fileSizeBytes;
    }

    void SetFileSizeBytes(String value)
    {
        m_fileSizeBytes = std::move(value);
        this->m_model.fileSizeBytes =  m_fileSizeBytes.empty() ? nullptr : m_fileSizeBytes.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.fileName = m_fileName.empty() ? nullptr : m_fileName.data();
        this->m_model.fileSizeBytes = m_fileSizeBytes.empty() ? nullptr : m_fileSizeBytes.data();
    }

    String m_fileName;
    String m_fileSizeBytes;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveFinalizeManifestRequestWrapper : public ModelWrapper<PFGameSaveFinalizeManifestRequest, Alloc>
{
public:
    using ModelType = PFGameSaveFinalizeManifestRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveFinalizeManifestRequestWrapper() = default;

    PFGameSaveFinalizeManifestRequestWrapper(const PFGameSaveFinalizeManifestRequest& model) :
        ModelWrapper<PFGameSaveFinalizeManifestRequest, Alloc>{ model },
        m_conflict{ model.conflict ? std::optional<PFGameSaveFinalizeManifestConflictWrapper<Alloc>>{ *model.conflict } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_filesToFinalize{ model.filesToFinalize, model.filesToFinalize + model.filesToFinalizeCount },
        m_manifestDescription{ SafeString(model.manifestDescription) },
        m_markBaseAsKnownGood{ model.markBaseAsKnownGood ? std::optional<bool>{ *model.markBaseAsKnownGood } : std::nullopt },
        m_version{ SafeString(model.version) }
    {
        SetModelPointers();
    }

    PFGameSaveFinalizeManifestRequestWrapper(const PFGameSaveFinalizeManifestRequestWrapper& src) :
        PFGameSaveFinalizeManifestRequestWrapper{ src.Model() }
    {
    }

    PFGameSaveFinalizeManifestRequestWrapper(PFGameSaveFinalizeManifestRequestWrapper&& src) :
        PFGameSaveFinalizeManifestRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveFinalizeManifestRequestWrapper& operator=(PFGameSaveFinalizeManifestRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveFinalizeManifestRequestWrapper() = default;

    friend void swap(PFGameSaveFinalizeManifestRequestWrapper& lhs, PFGameSaveFinalizeManifestRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_conflict, rhs.m_conflict);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_filesToFinalize, rhs.m_filesToFinalize);
        swap(lhs.m_manifestDescription, rhs.m_manifestDescription);
        swap(lhs.m_markBaseAsKnownGood, rhs.m_markBaseAsKnownGood);
        swap(lhs.m_version, rhs.m_version);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    std::optional<PFGameSaveFinalizeManifestConflictWrapper<Alloc>> const& GetConflict() const
    {
        return m_conflict;
    }

    void SetConflict(std::optional<PFGameSaveFinalizeManifestConflictWrapper<Alloc>> value)
    {
        m_conflict = std::move(value);
        this->m_model.conflict = m_conflict ? &m_conflict->Model() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    ModelVector<PFGameSaveFinalizedFileDetailsWrapper<Alloc>, Alloc> const& GetFilesToFinalize() const
    {
        return m_filesToFinalize;
    }

    void SetFilesToFinalize(ModelVector<PFGameSaveFinalizedFileDetailsWrapper<Alloc>, Alloc> value)
    {
        m_filesToFinalize = std::move(value);
        this->m_model.filesToFinalize =  m_filesToFinalize.empty() ? nullptr : m_filesToFinalize.data();
        this->m_model.filesToFinalizeCount =  static_cast<uint32_t>(m_filesToFinalize.size());
    }

    bool GetForce() const
    {
        return this->m_model.force;
    }

    void SetForce(bool value)
    {
        this->m_model.force = value;
    }

    String const& GetManifestDescription() const
    {
        return m_manifestDescription;
    }

    void SetManifestDescription(String value)
    {
        m_manifestDescription = std::move(value);
        this->m_model.manifestDescription =  m_manifestDescription.empty() ? nullptr : m_manifestDescription.data();
    }

    std::optional<bool> const& GetMarkBaseAsKnownGood() const
    {
        return m_markBaseAsKnownGood;
    }

    void SetMarkBaseAsKnownGood(std::optional<bool> value)
    {
        m_markBaseAsKnownGood = std::move(value);
        this->m_model.markBaseAsKnownGood = m_markBaseAsKnownGood ? m_markBaseAsKnownGood.operator->() : nullptr;
    }

    String const& GetVersion() const
    {
        return m_version;
    }

    void SetVersion(String value)
    {
        m_version = std::move(value);
        this->m_model.version =  m_version.empty() ? nullptr : m_version.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.conflict = m_conflict ?  &m_conflict->Model() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.filesToFinalize = m_filesToFinalize.empty() ? nullptr : m_filesToFinalize.data();
        this->m_model.manifestDescription = m_manifestDescription.empty() ? nullptr : m_manifestDescription.data();
        this->m_model.markBaseAsKnownGood = m_markBaseAsKnownGood ? m_markBaseAsKnownGood.operator->() : nullptr;
        this->m_model.version = m_version.empty() ? nullptr : m_version.data();
    }

    std::optional<PFGameSaveFinalizeManifestConflictWrapper<Alloc>> m_conflict;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    ModelVector<PFGameSaveFinalizedFileDetailsWrapper<Alloc>, Alloc> m_filesToFinalize;
    String m_manifestDescription;
    std::optional<bool> m_markBaseAsKnownGood;
    String m_version;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveManifestConflictWrapper : public ModelWrapper<PFGameSaveManifestConflict, Alloc>
{
public:
    using ModelType = PFGameSaveManifestConflict;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveManifestConflictWrapper() = default;

    PFGameSaveManifestConflictWrapper(const PFGameSaveManifestConflict& model) :
        ModelWrapper<PFGameSaveManifestConflict, Alloc>{ model },
        m_conflictingVersion{ SafeString(model.conflictingVersion) }
    {
        SetModelPointers();
    }

    PFGameSaveManifestConflictWrapper(const PFGameSaveManifestConflictWrapper& src) :
        PFGameSaveManifestConflictWrapper{ src.Model() }
    {
    }

    PFGameSaveManifestConflictWrapper(PFGameSaveManifestConflictWrapper&& src) :
        PFGameSaveManifestConflictWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveManifestConflictWrapper& operator=(PFGameSaveManifestConflictWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveManifestConflictWrapper() = default;

    friend void swap(PFGameSaveManifestConflictWrapper& lhs, PFGameSaveManifestConflictWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_conflictingVersion, rhs.m_conflictingVersion);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetConflictingVersion() const
    {
        return m_conflictingVersion;
    }

    void SetConflictingVersion(String value)
    {
        m_conflictingVersion = std::move(value);
        this->m_model.conflictingVersion =  m_conflictingVersion.empty() ? nullptr : m_conflictingVersion.data();
    }

    time_t GetConflictResolutionTimestamp() const
    {
        return this->m_model.conflictResolutionTimestamp;
    }

    void SetConflictResolutionTimestamp(time_t value)
    {
        this->m_model.conflictResolutionTimestamp = value;
    }

    bool GetIsWinner() const
    {
        return this->m_model.isWinner;
    }

    void SetIsWinner(bool value)
    {
        this->m_model.isWinner = value;
    }

    time_t GetRetainedUntilTimestamp() const
    {
        return this->m_model.retainedUntilTimestamp;
    }

    void SetRetainedUntilTimestamp(time_t value)
    {
        this->m_model.retainedUntilTimestamp = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.conflictingVersion = m_conflictingVersion.empty() ? nullptr : m_conflictingVersion.data();
    }

    String m_conflictingVersion;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveFileMetadataWrapper : public ModelWrapper<PFGameSaveFileMetadata, Alloc>
{
public:
    using ModelType = PFGameSaveFileMetadata;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveFileMetadataWrapper() = default;

    PFGameSaveFileMetadataWrapper(const PFGameSaveFileMetadata& model) :
        ModelWrapper<PFGameSaveFileMetadata, Alloc>{ model },
        m_fileName{ SafeString(model.fileName) }
    {
        SetModelPointers();
    }

    PFGameSaveFileMetadataWrapper(const PFGameSaveFileMetadataWrapper& src) :
        PFGameSaveFileMetadataWrapper{ src.Model() }
    {
    }

    PFGameSaveFileMetadataWrapper(PFGameSaveFileMetadataWrapper&& src) :
        PFGameSaveFileMetadataWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveFileMetadataWrapper& operator=(PFGameSaveFileMetadataWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveFileMetadataWrapper() = default;

    friend void swap(PFGameSaveFileMetadataWrapper& lhs, PFGameSaveFileMetadataWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_fileName, rhs.m_fileName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetFileName() const
    {
        return m_fileName;
    }

    void SetFileName(String value)
    {
        m_fileName = std::move(value);
        this->m_model.fileName =  m_fileName.empty() ? nullptr : m_fileName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.fileName = m_fileName.empty() ? nullptr : m_fileName.data();
    }

    String m_fileName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveManifestMetadataWrapper : public ModelWrapper<PFGameSaveManifestMetadata, Alloc>
{
public:
    using ModelType = PFGameSaveManifestMetadata;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveManifestMetadataWrapper() = default;

    PFGameSaveManifestMetadataWrapper(const PFGameSaveManifestMetadata& model) :
        ModelWrapper<PFGameSaveManifestMetadata, Alloc>{ model },
        m_deviceId{ SafeString(model.deviceId) },
        m_deviceName{ SafeString(model.deviceName) },
        m_deviceType{ SafeString(model.deviceType) },
        m_deviceVersion{ SafeString(model.deviceVersion) }
    {
        SetModelPointers();
    }

    PFGameSaveManifestMetadataWrapper(const PFGameSaveManifestMetadataWrapper& src) :
        PFGameSaveManifestMetadataWrapper{ src.Model() }
    {
    }

    PFGameSaveManifestMetadataWrapper(PFGameSaveManifestMetadataWrapper&& src) :
        PFGameSaveManifestMetadataWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveManifestMetadataWrapper& operator=(PFGameSaveManifestMetadataWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveManifestMetadataWrapper() = default;

    friend void swap(PFGameSaveManifestMetadataWrapper& lhs, PFGameSaveManifestMetadataWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_deviceId, rhs.m_deviceId);
        swap(lhs.m_deviceName, rhs.m_deviceName);
        swap(lhs.m_deviceType, rhs.m_deviceType);
        swap(lhs.m_deviceVersion, rhs.m_deviceVersion);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetDeviceId() const
    {
        return m_deviceId;
    }

    void SetDeviceId(String value)
    {
        m_deviceId = std::move(value);
        this->m_model.deviceId =  m_deviceId.empty() ? nullptr : m_deviceId.data();
    }

    String const& GetDeviceName() const
    {
        return m_deviceName;
    }

    void SetDeviceName(String value)
    {
        m_deviceName = std::move(value);
        this->m_model.deviceName =  m_deviceName.empty() ? nullptr : m_deviceName.data();
    }

    String const& GetDeviceType() const
    {
        return m_deviceType;
    }

    void SetDeviceType(String value)
    {
        m_deviceType = std::move(value);
        this->m_model.deviceType =  m_deviceType.empty() ? nullptr : m_deviceType.data();
    }

    String const& GetDeviceVersion() const
    {
        return m_deviceVersion;
    }

    void SetDeviceVersion(String value)
    {
        m_deviceVersion = std::move(value);
        this->m_model.deviceVersion =  m_deviceVersion.empty() ? nullptr : m_deviceVersion.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.deviceId = m_deviceId.empty() ? nullptr : m_deviceId.data();
        this->m_model.deviceName = m_deviceName.empty() ? nullptr : m_deviceName.data();
        this->m_model.deviceType = m_deviceType.empty() ? nullptr : m_deviceType.data();
        this->m_model.deviceVersion = m_deviceVersion.empty() ? nullptr : m_deviceVersion.data();
    }

    String m_deviceId;
    String m_deviceName;
    String m_deviceType;
    String m_deviceVersion;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveUploadProgressWrapper : public ModelWrapper<PFGameSaveUploadProgress, Alloc>
{
public:
    using ModelType = PFGameSaveUploadProgress;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveUploadProgressWrapper() = default;

    PFGameSaveUploadProgressWrapper(const PFGameSaveUploadProgress& model) :
        ModelWrapper<PFGameSaveUploadProgress, Alloc>{ model },
        m_totalBytes{ SafeString(model.totalBytes) },
        m_uploadedBytes{ SafeString(model.uploadedBytes) }
    {
        SetModelPointers();
    }

    PFGameSaveUploadProgressWrapper(const PFGameSaveUploadProgressWrapper& src) :
        PFGameSaveUploadProgressWrapper{ src.Model() }
    {
    }

    PFGameSaveUploadProgressWrapper(PFGameSaveUploadProgressWrapper&& src) :
        PFGameSaveUploadProgressWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveUploadProgressWrapper& operator=(PFGameSaveUploadProgressWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveUploadProgressWrapper() = default;

    friend void swap(PFGameSaveUploadProgressWrapper& lhs, PFGameSaveUploadProgressWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_totalBytes, rhs.m_totalBytes);
        swap(lhs.m_uploadedBytes, rhs.m_uploadedBytes);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetTotalBytes() const
    {
        return m_totalBytes;
    }

    void SetTotalBytes(String value)
    {
        m_totalBytes = std::move(value);
        this->m_model.totalBytes =  m_totalBytes.empty() ? nullptr : m_totalBytes.data();
    }

    String const& GetUploadedBytes() const
    {
        return m_uploadedBytes;
    }

    void SetUploadedBytes(String value)
    {
        m_uploadedBytes = std::move(value);
        this->m_model.uploadedBytes =  m_uploadedBytes.empty() ? nullptr : m_uploadedBytes.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.totalBytes = m_totalBytes.empty() ? nullptr : m_totalBytes.data();
        this->m_model.uploadedBytes = m_uploadedBytes.empty() ? nullptr : m_uploadedBytes.data();
    }

    String m_totalBytes;
    String m_uploadedBytes;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveManifestWrapper : public ModelWrapper<PFGameSaveManifest, Alloc>
{
public:
    using ModelType = PFGameSaveManifest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveManifestWrapper() = default;

    PFGameSaveManifestWrapper(const PFGameSaveManifest& model) :
        ModelWrapper<PFGameSaveManifest, Alloc>{ model },
        m_baseVersion{ SafeString(model.baseVersion) },
        m_conflict{ model.conflict ? std::optional<PFGameSaveManifestConflictWrapper<Alloc>>{ *model.conflict } : std::nullopt },
        m_files{ model.files, model.files + model.filesCount },
        m_finalizationTimestamp{ model.finalizationTimestamp ? std::optional<time_t>{ *model.finalizationTimestamp } : std::nullopt },
        m_isKnownGood{ model.isKnownGood ? std::optional<bool>{ *model.isKnownGood } : std::nullopt },
        m_manifestDescription{ SafeString(model.manifestDescription) },
        m_metadata{ model.metadata ? std::optional<PFGameSaveManifestMetadataWrapper<Alloc>>{ *model.metadata } : std::nullopt },
        m_rollbackReason{ SafeString(model.rollbackReason) },
        m_status{ SafeString(model.status) },
        m_uploadProgress{ model.uploadProgress ? std::optional<PFGameSaveUploadProgressWrapper<Alloc>>{ *model.uploadProgress } : std::nullopt },
        m_version{ SafeString(model.version) }
    {
        SetModelPointers();
    }

    PFGameSaveManifestWrapper(const PFGameSaveManifestWrapper& src) :
        PFGameSaveManifestWrapper{ src.Model() }
    {
    }

    PFGameSaveManifestWrapper(PFGameSaveManifestWrapper&& src) :
        PFGameSaveManifestWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveManifestWrapper& operator=(PFGameSaveManifestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveManifestWrapper() = default;

    friend void swap(PFGameSaveManifestWrapper& lhs, PFGameSaveManifestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_baseVersion, rhs.m_baseVersion);
        swap(lhs.m_conflict, rhs.m_conflict);
        swap(lhs.m_files, rhs.m_files);
        swap(lhs.m_finalizationTimestamp, rhs.m_finalizationTimestamp);
        swap(lhs.m_isKnownGood, rhs.m_isKnownGood);
        swap(lhs.m_manifestDescription, rhs.m_manifestDescription);
        swap(lhs.m_metadata, rhs.m_metadata);
        swap(lhs.m_rollbackReason, rhs.m_rollbackReason);
        swap(lhs.m_status, rhs.m_status);
        swap(lhs.m_uploadProgress, rhs.m_uploadProgress);
        swap(lhs.m_version, rhs.m_version);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetBaseVersion() const
    {
        return m_baseVersion;
    }

    void SetBaseVersion(String value)
    {
        m_baseVersion = std::move(value);
        this->m_model.baseVersion =  m_baseVersion.empty() ? nullptr : m_baseVersion.data();
    }

    std::optional<PFGameSaveManifestConflictWrapper<Alloc>> const& GetConflict() const
    {
        return m_conflict;
    }

    void SetConflict(std::optional<PFGameSaveManifestConflictWrapper<Alloc>> value)
    {
        m_conflict = std::move(value);
        this->m_model.conflict = m_conflict ? &m_conflict->Model() : nullptr;
    }

    time_t GetCreationTimestamp() const
    {
        return this->m_model.creationTimestamp;
    }

    void SetCreationTimestamp(time_t value)
    {
        this->m_model.creationTimestamp = value;
    }

    ModelVector<PFGameSaveFileMetadataWrapper<Alloc>, Alloc> const& GetFiles() const
    {
        return m_files;
    }

    void SetFiles(ModelVector<PFGameSaveFileMetadataWrapper<Alloc>, Alloc> value)
    {
        m_files = std::move(value);
        this->m_model.files =  m_files.empty() ? nullptr : m_files.data();
        this->m_model.filesCount =  static_cast<uint32_t>(m_files.size());
    }

    std::optional<time_t> const& GetFinalizationTimestamp() const
    {
        return m_finalizationTimestamp;
    }

    void SetFinalizationTimestamp(std::optional<time_t> value)
    {
        m_finalizationTimestamp = std::move(value);
        this->m_model.finalizationTimestamp = m_finalizationTimestamp ? m_finalizationTimestamp.operator->() : nullptr;
    }

    bool GetGeneratedByRollback() const
    {
        return this->m_model.generatedByRollback;
    }

    void SetGeneratedByRollback(bool value)
    {
        this->m_model.generatedByRollback = value;
    }

    std::optional<bool> const& GetIsKnownGood() const
    {
        return m_isKnownGood;
    }

    void SetIsKnownGood(std::optional<bool> value)
    {
        m_isKnownGood = std::move(value);
        this->m_model.isKnownGood = m_isKnownGood ? m_isKnownGood.operator->() : nullptr;
    }

    time_t GetLastUpdateTimestamp() const
    {
        return this->m_model.lastUpdateTimestamp;
    }

    void SetLastUpdateTimestamp(time_t value)
    {
        this->m_model.lastUpdateTimestamp = value;
    }

    String const& GetManifestDescription() const
    {
        return m_manifestDescription;
    }

    void SetManifestDescription(String value)
    {
        m_manifestDescription = std::move(value);
        this->m_model.manifestDescription =  m_manifestDescription.empty() ? nullptr : m_manifestDescription.data();
    }

    std::optional<PFGameSaveManifestMetadataWrapper<Alloc>> const& GetMetadata() const
    {
        return m_metadata;
    }

    void SetMetadata(std::optional<PFGameSaveManifestMetadataWrapper<Alloc>> value)
    {
        m_metadata = std::move(value);
        this->m_model.metadata = m_metadata ? &m_metadata->Model() : nullptr;
    }

    String const& GetRollbackReason() const
    {
        return m_rollbackReason;
    }

    void SetRollbackReason(String value)
    {
        m_rollbackReason = std::move(value);
        this->m_model.rollbackReason =  m_rollbackReason.empty() ? nullptr : m_rollbackReason.data();
    }

    String const& GetStatus() const
    {
        return m_status;
    }

    void SetStatus(String value)
    {
        m_status = std::move(value);
        this->m_model.status =  m_status.empty() ? nullptr : m_status.data();
    }

    std::optional<PFGameSaveUploadProgressWrapper<Alloc>> const& GetUploadProgress() const
    {
        return m_uploadProgress;
    }

    void SetUploadProgress(std::optional<PFGameSaveUploadProgressWrapper<Alloc>> value)
    {
        m_uploadProgress = std::move(value);
        this->m_model.uploadProgress = m_uploadProgress ? &m_uploadProgress->Model() : nullptr;
    }

    String const& GetVersion() const
    {
        return m_version;
    }

    void SetVersion(String value)
    {
        m_version = std::move(value);
        this->m_model.version =  m_version.empty() ? nullptr : m_version.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.baseVersion = m_baseVersion.empty() ? nullptr : m_baseVersion.data();
        this->m_model.conflict = m_conflict ?  &m_conflict->Model() : nullptr;
        this->m_model.files = m_files.empty() ? nullptr : m_files.data();
        this->m_model.finalizationTimestamp = m_finalizationTimestamp ? m_finalizationTimestamp.operator->() : nullptr;
        this->m_model.isKnownGood = m_isKnownGood ? m_isKnownGood.operator->() : nullptr;
        this->m_model.manifestDescription = m_manifestDescription.empty() ? nullptr : m_manifestDescription.data();
        this->m_model.metadata = m_metadata ?  &m_metadata->Model() : nullptr;
        this->m_model.rollbackReason = m_rollbackReason.empty() ? nullptr : m_rollbackReason.data();
        this->m_model.status = m_status.empty() ? nullptr : m_status.data();
        this->m_model.uploadProgress = m_uploadProgress ?  &m_uploadProgress->Model() : nullptr;
        this->m_model.version = m_version.empty() ? nullptr : m_version.data();
    }

    String m_baseVersion;
    std::optional<PFGameSaveManifestConflictWrapper<Alloc>> m_conflict;
    ModelVector<PFGameSaveFileMetadataWrapper<Alloc>, Alloc> m_files;
    std::optional<time_t> m_finalizationTimestamp;
    std::optional<bool> m_isKnownGood;
    String m_manifestDescription;
    std::optional<PFGameSaveManifestMetadataWrapper<Alloc>> m_metadata;
    String m_rollbackReason;
    String m_status;
    std::optional<PFGameSaveUploadProgressWrapper<Alloc>> m_uploadProgress;
    String m_version;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveFinalizeManifestResponseWrapper : public ModelWrapper<PFGameSaveFinalizeManifestResponse, Alloc>
{
public:
    using ModelType = PFGameSaveFinalizeManifestResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveFinalizeManifestResponseWrapper() = default;

    PFGameSaveFinalizeManifestResponseWrapper(const PFGameSaveFinalizeManifestResponse& model) :
        ModelWrapper<PFGameSaveFinalizeManifestResponse, Alloc>{ model },
        m_manifest{ model.manifest ? std::optional<PFGameSaveManifestWrapper<Alloc>>{ *model.manifest } : std::nullopt }
    {
        SetModelPointers();
    }

    PFGameSaveFinalizeManifestResponseWrapper(const PFGameSaveFinalizeManifestResponseWrapper& src) :
        PFGameSaveFinalizeManifestResponseWrapper{ src.Model() }
    {
    }

    PFGameSaveFinalizeManifestResponseWrapper(PFGameSaveFinalizeManifestResponseWrapper&& src) :
        PFGameSaveFinalizeManifestResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveFinalizeManifestResponseWrapper& operator=(PFGameSaveFinalizeManifestResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveFinalizeManifestResponseWrapper() = default;

    friend void swap(PFGameSaveFinalizeManifestResponseWrapper& lhs, PFGameSaveFinalizeManifestResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_manifest, rhs.m_manifest);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    std::optional<PFGameSaveManifestWrapper<Alloc>> const& GetManifest() const
    {
        return m_manifest;
    }

    void SetManifest(std::optional<PFGameSaveManifestWrapper<Alloc>> value)
    {
        m_manifest = std::move(value);
        this->m_model.manifest = m_manifest ? &m_manifest->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.manifest = m_manifest ?  &m_manifest->Model() : nullptr;
    }

    std::optional<PFGameSaveManifestWrapper<Alloc>> m_manifest;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveGetConfigForTitleRequestWrapper : public ModelWrapper<PFGameSaveGetConfigForTitleRequest, Alloc>
{
public:
    using ModelType = PFGameSaveGetConfigForTitleRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveGetConfigForTitleRequestWrapper() = default;

    PFGameSaveGetConfigForTitleRequestWrapper(const PFGameSaveGetConfigForTitleRequest& model) :
        ModelWrapper<PFGameSaveGetConfigForTitleRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt }
    {
        SetModelPointers();
    }

    PFGameSaveGetConfigForTitleRequestWrapper(const PFGameSaveGetConfigForTitleRequestWrapper& src) :
        PFGameSaveGetConfigForTitleRequestWrapper{ src.Model() }
    {
    }

    PFGameSaveGetConfigForTitleRequestWrapper(PFGameSaveGetConfigForTitleRequestWrapper&& src) :
        PFGameSaveGetConfigForTitleRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveGetConfigForTitleRequestWrapper& operator=(PFGameSaveGetConfigForTitleRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveGetConfigForTitleRequestWrapper() = default;

    friend void swap(PFGameSaveGetConfigForTitleRequestWrapper& lhs, PFGameSaveGetConfigForTitleRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveGetConfigForTitleResponseWrapper : public ModelWrapper<PFGameSaveGetConfigForTitleResponse, Alloc>
{
public:
    using ModelType = PFGameSaveGetConfigForTitleResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveGetConfigForTitleResponseWrapper() = default;

    PFGameSaveGetConfigForTitleResponseWrapper(const PFGameSaveGetConfigForTitleResponse& model) :
        ModelWrapper<PFGameSaveGetConfigForTitleResponse, Alloc>{ model },
        m_onboardingStatus{ SafeString(model.onboardingStatus) },
        m_perPlayerQuotaBytes{ SafeString(model.perPlayerQuotaBytes) }
    {
        SetModelPointers();
    }

    PFGameSaveGetConfigForTitleResponseWrapper(const PFGameSaveGetConfigForTitleResponseWrapper& src) :
        PFGameSaveGetConfigForTitleResponseWrapper{ src.Model() }
    {
    }

    PFGameSaveGetConfigForTitleResponseWrapper(PFGameSaveGetConfigForTitleResponseWrapper&& src) :
        PFGameSaveGetConfigForTitleResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveGetConfigForTitleResponseWrapper& operator=(PFGameSaveGetConfigForTitleResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveGetConfigForTitleResponseWrapper() = default;

    friend void swap(PFGameSaveGetConfigForTitleResponseWrapper& lhs, PFGameSaveGetConfigForTitleResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_onboardingStatus, rhs.m_onboardingStatus);
        swap(lhs.m_perPlayerQuotaBytes, rhs.m_perPlayerQuotaBytes);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    bool GetDisableClientRollbackToLastConflictLoser() const
    {
        return this->m_model.disableClientRollbackToLastConflictLoser;
    }

    void SetDisableClientRollbackToLastConflictLoser(bool value)
    {
        this->m_model.disableClientRollbackToLastConflictLoser = value;
    }

    bool GetDisableClientRollbackToLastKnownGood() const
    {
        return this->m_model.disableClientRollbackToLastKnownGood;
    }

    void SetDisableClientRollbackToLastKnownGood(bool value)
    {
        this->m_model.disableClientRollbackToLastKnownGood = value;
    }

    bool GetDisableUnsignaledOutOfProcessUpload() const
    {
        return this->m_model.disableUnsignaledOutOfProcessUpload;
    }

    void SetDisableUnsignaledOutOfProcessUpload(bool value)
    {
        this->m_model.disableUnsignaledOutOfProcessUpload = value;
    }

    String const& GetOnboardingStatus() const
    {
        return m_onboardingStatus;
    }

    void SetOnboardingStatus(String value)
    {
        m_onboardingStatus = std::move(value);
        this->m_model.onboardingStatus =  m_onboardingStatus.empty() ? nullptr : m_onboardingStatus.data();
    }

    String const& GetPerPlayerQuotaBytes() const
    {
        return m_perPlayerQuotaBytes;
    }

    void SetPerPlayerQuotaBytes(String value)
    {
        m_perPlayerQuotaBytes = std::move(value);
        this->m_model.perPlayerQuotaBytes =  m_perPlayerQuotaBytes.empty() ? nullptr : m_perPlayerQuotaBytes.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.onboardingStatus = m_onboardingStatus.empty() ? nullptr : m_onboardingStatus.data();
        this->m_model.perPlayerQuotaBytes = m_perPlayerQuotaBytes.empty() ? nullptr : m_perPlayerQuotaBytes.data();
    }

    String m_onboardingStatus;
    String m_perPlayerQuotaBytes;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveGetManifestDownloadDetailsRequestWrapper : public ModelWrapper<PFGameSaveGetManifestDownloadDetailsRequest, Alloc>
{
public:
    using ModelType = PFGameSaveGetManifestDownloadDetailsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveGetManifestDownloadDetailsRequestWrapper() = default;

    PFGameSaveGetManifestDownloadDetailsRequestWrapper(const PFGameSaveGetManifestDownloadDetailsRequest& model) :
        ModelWrapper<PFGameSaveGetManifestDownloadDetailsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_version{ SafeString(model.version) }
    {
        SetModelPointers();
    }

    PFGameSaveGetManifestDownloadDetailsRequestWrapper(const PFGameSaveGetManifestDownloadDetailsRequestWrapper& src) :
        PFGameSaveGetManifestDownloadDetailsRequestWrapper{ src.Model() }
    {
    }

    PFGameSaveGetManifestDownloadDetailsRequestWrapper(PFGameSaveGetManifestDownloadDetailsRequestWrapper&& src) :
        PFGameSaveGetManifestDownloadDetailsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveGetManifestDownloadDetailsRequestWrapper& operator=(PFGameSaveGetManifestDownloadDetailsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveGetManifestDownloadDetailsRequestWrapper() = default;

    friend void swap(PFGameSaveGetManifestDownloadDetailsRequestWrapper& lhs, PFGameSaveGetManifestDownloadDetailsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_version, rhs.m_version);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    String const& GetVersion() const
    {
        return m_version;
    }

    void SetVersion(String value)
    {
        m_version = std::move(value);
        this->m_model.version =  m_version.empty() ? nullptr : m_version.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.version = m_version.empty() ? nullptr : m_version.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_version;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveFileDownloadDetailsWrapper : public ModelWrapper<PFGameSaveFileDownloadDetails, Alloc>
{
public:
    using ModelType = PFGameSaveFileDownloadDetails;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveFileDownloadDetailsWrapper() = default;

    PFGameSaveFileDownloadDetailsWrapper(const PFGameSaveFileDownloadDetails& model) :
        ModelWrapper<PFGameSaveFileDownloadDetails, Alloc>{ model },
        m_downloadUrl{ SafeString(model.downloadUrl) },
        m_fileName{ SafeString(model.fileName) }
    {
        SetModelPointers();
    }

    PFGameSaveFileDownloadDetailsWrapper(const PFGameSaveFileDownloadDetailsWrapper& src) :
        PFGameSaveFileDownloadDetailsWrapper{ src.Model() }
    {
    }

    PFGameSaveFileDownloadDetailsWrapper(PFGameSaveFileDownloadDetailsWrapper&& src) :
        PFGameSaveFileDownloadDetailsWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveFileDownloadDetailsWrapper& operator=(PFGameSaveFileDownloadDetailsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveFileDownloadDetailsWrapper() = default;

    friend void swap(PFGameSaveFileDownloadDetailsWrapper& lhs, PFGameSaveFileDownloadDetailsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_downloadUrl, rhs.m_downloadUrl);
        swap(lhs.m_fileName, rhs.m_fileName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    time_t GetAccessTokenExpirationTime() const
    {
        return this->m_model.accessTokenExpirationTime;
    }

    void SetAccessTokenExpirationTime(time_t value)
    {
        this->m_model.accessTokenExpirationTime = value;
    }

    String const& GetDownloadUrl() const
    {
        return m_downloadUrl;
    }

    void SetDownloadUrl(String value)
    {
        m_downloadUrl = std::move(value);
        this->m_model.downloadUrl =  m_downloadUrl.empty() ? nullptr : m_downloadUrl.data();
    }

    String const& GetFileName() const
    {
        return m_fileName;
    }

    void SetFileName(String value)
    {
        m_fileName = std::move(value);
        this->m_model.fileName =  m_fileName.empty() ? nullptr : m_fileName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.downloadUrl = m_downloadUrl.empty() ? nullptr : m_downloadUrl.data();
        this->m_model.fileName = m_fileName.empty() ? nullptr : m_fileName.data();
    }

    String m_downloadUrl;
    String m_fileName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveGetManifestDownloadDetailsResponseWrapper : public ModelWrapper<PFGameSaveGetManifestDownloadDetailsResponse, Alloc>
{
public:
    using ModelType = PFGameSaveGetManifestDownloadDetailsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveGetManifestDownloadDetailsResponseWrapper() = default;

    PFGameSaveGetManifestDownloadDetailsResponseWrapper(const PFGameSaveGetManifestDownloadDetailsResponse& model) :
        ModelWrapper<PFGameSaveGetManifestDownloadDetailsResponse, Alloc>{ model },
        m_files{ model.files, model.files + model.filesCount }
    {
        SetModelPointers();
    }

    PFGameSaveGetManifestDownloadDetailsResponseWrapper(const PFGameSaveGetManifestDownloadDetailsResponseWrapper& src) :
        PFGameSaveGetManifestDownloadDetailsResponseWrapper{ src.Model() }
    {
    }

    PFGameSaveGetManifestDownloadDetailsResponseWrapper(PFGameSaveGetManifestDownloadDetailsResponseWrapper&& src) : 
        PFGameSaveGetManifestDownloadDetailsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveGetManifestDownloadDetailsResponseWrapper& operator=(PFGameSaveGetManifestDownloadDetailsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveGetManifestDownloadDetailsResponseWrapper() = default;

    friend void swap(PFGameSaveGetManifestDownloadDetailsResponseWrapper& lhs, PFGameSaveGetManifestDownloadDetailsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_files, rhs.m_files);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    ModelVector<PFGameSaveFileDownloadDetailsWrapper<Alloc>, Alloc> const& GetFiles() const
    {
        return m_files;
    }

    void SetFiles(ModelVector<PFGameSaveFileDownloadDetailsWrapper<Alloc>, Alloc> value)
    {
        m_files = std::move(value);
        this->m_model.files =  m_files.empty() ? nullptr : m_files.data();
        this->m_model.filesCount =  static_cast<uint32_t>(m_files.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.files = m_files.empty() ? nullptr : m_files.data();
    }

    ModelVector<PFGameSaveFileDownloadDetailsWrapper<Alloc>, Alloc> m_files;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveGetQuotaForPlayerRequestWrapper : public ModelWrapper<PFGameSaveGetQuotaForPlayerRequest, Alloc>
{
public:
    using ModelType = PFGameSaveGetQuotaForPlayerRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveGetQuotaForPlayerRequestWrapper() = default;

    PFGameSaveGetQuotaForPlayerRequestWrapper(const PFGameSaveGetQuotaForPlayerRequest& model) :
        ModelWrapper<PFGameSaveGetQuotaForPlayerRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt }
    {
        SetModelPointers();
    }

    PFGameSaveGetQuotaForPlayerRequestWrapper(const PFGameSaveGetQuotaForPlayerRequestWrapper& src) :
        PFGameSaveGetQuotaForPlayerRequestWrapper{ src.Model() }
    {
    }

    PFGameSaveGetQuotaForPlayerRequestWrapper(PFGameSaveGetQuotaForPlayerRequestWrapper&& src) :
        PFGameSaveGetQuotaForPlayerRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveGetQuotaForPlayerRequestWrapper& operator=(PFGameSaveGetQuotaForPlayerRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveGetQuotaForPlayerRequestWrapper() = default;

    friend void swap(PFGameSaveGetQuotaForPlayerRequestWrapper& lhs, PFGameSaveGetQuotaForPlayerRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveGetQuotaForPlayerResponseWrapper : public ModelWrapper<PFGameSaveGetQuotaForPlayerResponse, Alloc>
{
public:
    using ModelType = PFGameSaveGetQuotaForPlayerResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveGetQuotaForPlayerResponseWrapper() = default;

    PFGameSaveGetQuotaForPlayerResponseWrapper(const PFGameSaveGetQuotaForPlayerResponse& model) :
        ModelWrapper<PFGameSaveGetQuotaForPlayerResponse, Alloc>{ model },
        m_availableBytes{ SafeString(model.availableBytes) },
        m_totalBytes{ SafeString(model.totalBytes) }
    {
        SetModelPointers();
    }

    PFGameSaveGetQuotaForPlayerResponseWrapper(const PFGameSaveGetQuotaForPlayerResponseWrapper& src) :
        PFGameSaveGetQuotaForPlayerResponseWrapper{ src.Model() }
    {
    }

    PFGameSaveGetQuotaForPlayerResponseWrapper(PFGameSaveGetQuotaForPlayerResponseWrapper&& src) :
        PFGameSaveGetQuotaForPlayerResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveGetQuotaForPlayerResponseWrapper& operator=(PFGameSaveGetQuotaForPlayerResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveGetQuotaForPlayerResponseWrapper() = default;

    friend void swap(PFGameSaveGetQuotaForPlayerResponseWrapper& lhs, PFGameSaveGetQuotaForPlayerResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_availableBytes, rhs.m_availableBytes);
        swap(lhs.m_totalBytes, rhs.m_totalBytes);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetAvailableBytes() const
    {
        return m_availableBytes;
    }

    void SetAvailableBytes(String value)
    {
        m_availableBytes = std::move(value);
        this->m_model.availableBytes =  m_availableBytes.empty() ? nullptr : m_availableBytes.data();
    }

    String const& GetTotalBytes() const
    {
        return m_totalBytes;
    }

    void SetTotalBytes(String value)
    {
        m_totalBytes = std::move(value);
        this->m_model.totalBytes =  m_totalBytes.empty() ? nullptr : m_totalBytes.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.availableBytes = m_availableBytes.empty() ? nullptr : m_availableBytes.data();
        this->m_model.totalBytes = m_totalBytes.empty() ? nullptr : m_totalBytes.data();
    }

    String m_availableBytes;
    String m_totalBytes;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveInitializeManifestRequestWrapper : public ModelWrapper<PFGameSaveInitializeManifestRequest, Alloc>
{
public:
    using ModelType = PFGameSaveInitializeManifestRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveInitializeManifestRequestWrapper() = default;

    PFGameSaveInitializeManifestRequestWrapper(const PFGameSaveInitializeManifestRequest& model) :
        ModelWrapper<PFGameSaveInitializeManifestRequest, Alloc>{ model },
        m_baseVersion{ SafeString(model.baseVersion) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_metadata{ model.metadata ? *model.metadata : decltype(*model.metadata){} },
        m_version{ SafeString(model.version) }
    {
        SetModelPointers();
    }

    PFGameSaveInitializeManifestRequestWrapper(const PFGameSaveInitializeManifestRequestWrapper& src) :
        PFGameSaveInitializeManifestRequestWrapper{ src.Model() }
    {
    }

    PFGameSaveInitializeManifestRequestWrapper(PFGameSaveInitializeManifestRequestWrapper&& src) :
        PFGameSaveInitializeManifestRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveInitializeManifestRequestWrapper& operator=(PFGameSaveInitializeManifestRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveInitializeManifestRequestWrapper() = default;

    friend void swap(PFGameSaveInitializeManifestRequestWrapper& lhs, PFGameSaveInitializeManifestRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_baseVersion, rhs.m_baseVersion);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_metadata, rhs.m_metadata);
        swap(lhs.m_version, rhs.m_version);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetBaseVersion() const
    {
        return m_baseVersion;
    }

    void SetBaseVersion(String value)
    {
        m_baseVersion = std::move(value);
        this->m_model.baseVersion =  m_baseVersion.empty() ? nullptr : m_baseVersion.data();
    }

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    PFGameSaveManifestMetadataWrapper<Alloc> const& GetMetadata() const
    {
        return m_metadata;
    }

    void SetMetadata(PFGameSaveManifestMetadataWrapper<Alloc> value)
    {
        m_metadata = std::move(value);
        this->m_model.metadata = &m_metadata.Model();
    }

    String const& GetVersion() const
    {
        return m_version;
    }

    void SetVersion(String value)
    {
        m_version = std::move(value);
        this->m_model.version =  m_version.empty() ? nullptr : m_version.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.baseVersion = m_baseVersion.empty() ? nullptr : m_baseVersion.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.metadata = &m_metadata.Model();
        this->m_model.version = m_version.empty() ? nullptr : m_version.data();
    }

    String m_baseVersion;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    PFGameSaveManifestMetadataWrapper<Alloc> m_metadata;
    String m_version;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveInitializeManifestResponseWrapper : public ModelWrapper<PFGameSaveInitializeManifestResponse, Alloc>
{
public:
    using ModelType = PFGameSaveInitializeManifestResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveInitializeManifestResponseWrapper() = default;

    PFGameSaveInitializeManifestResponseWrapper(const PFGameSaveInitializeManifestResponse& model) :
        ModelWrapper<PFGameSaveInitializeManifestResponse, Alloc>{ model },
        m_manifest{ model.manifest ? std::optional<PFGameSaveManifestWrapper<Alloc>>{ *model.manifest } : std::nullopt }
    {
        SetModelPointers();
    }

    PFGameSaveInitializeManifestResponseWrapper(const PFGameSaveInitializeManifestResponseWrapper& src) :
        PFGameSaveInitializeManifestResponseWrapper{ src.Model() }
    {
    }

    PFGameSaveInitializeManifestResponseWrapper(PFGameSaveInitializeManifestResponseWrapper&& src) :
        PFGameSaveInitializeManifestResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveInitializeManifestResponseWrapper& operator=(PFGameSaveInitializeManifestResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveInitializeManifestResponseWrapper() = default;

    friend void swap(PFGameSaveInitializeManifestResponseWrapper& lhs, PFGameSaveInitializeManifestResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_manifest, rhs.m_manifest);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    std::optional<PFGameSaveManifestWrapper<Alloc>> const& GetManifest() const
    {
        return m_manifest;
    }

    void SetManifest(std::optional<PFGameSaveManifestWrapper<Alloc>> value)
    {
        m_manifest = std::move(value);
        this->m_model.manifest = m_manifest ? &m_manifest->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.manifest = m_manifest ?  &m_manifest->Model() : nullptr;
    }

    std::optional<PFGameSaveManifestWrapper<Alloc>> m_manifest;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveFileToUploadWrapper : public ModelWrapper<PFGameSaveFileToUpload, Alloc>
{
public:
    using ModelType = PFGameSaveFileToUpload;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveFileToUploadWrapper() = default;

    PFGameSaveFileToUploadWrapper(const PFGameSaveFileToUpload& model) :
        ModelWrapper<PFGameSaveFileToUpload, Alloc>{ model },
        m_fileName{ SafeString(model.fileName) }
    {
        SetModelPointers();
    }

    PFGameSaveFileToUploadWrapper(const PFGameSaveFileToUploadWrapper& src) :
        PFGameSaveFileToUploadWrapper{ src.Model() }
    {
    }

    PFGameSaveFileToUploadWrapper(PFGameSaveFileToUploadWrapper&& src) :
        PFGameSaveFileToUploadWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveFileToUploadWrapper& operator=(PFGameSaveFileToUploadWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveFileToUploadWrapper() = default;

    friend void swap(PFGameSaveFileToUploadWrapper& lhs, PFGameSaveFileToUploadWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_fileName, rhs.m_fileName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetFileName() const
    {
        return m_fileName;
    }

    void SetFileName(String value)
    {
        m_fileName = std::move(value);
        this->m_model.fileName =  m_fileName.empty() ? nullptr : m_fileName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.fileName = m_fileName.empty() ? nullptr : m_fileName.data();
    }

    String m_fileName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveInitiateUploadRequestWrapper : public ModelWrapper<PFGameSaveInitiateUploadRequest, Alloc>
{
public:
    using ModelType = PFGameSaveInitiateUploadRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveInitiateUploadRequestWrapper() = default;

    PFGameSaveInitiateUploadRequestWrapper(const PFGameSaveInitiateUploadRequest& model) :
        ModelWrapper<PFGameSaveInitiateUploadRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_files{ model.files, model.files + model.filesCount },
        m_version{ SafeString(model.version) }
    {
        SetModelPointers();
    }

    PFGameSaveInitiateUploadRequestWrapper(const PFGameSaveInitiateUploadRequestWrapper& src) :
        PFGameSaveInitiateUploadRequestWrapper{ src.Model() }
    {
    }

    PFGameSaveInitiateUploadRequestWrapper(PFGameSaveInitiateUploadRequestWrapper&& src) :
        PFGameSaveInitiateUploadRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveInitiateUploadRequestWrapper& operator=(PFGameSaveInitiateUploadRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveInitiateUploadRequestWrapper() = default;

    friend void swap(PFGameSaveInitiateUploadRequestWrapper& lhs, PFGameSaveInitiateUploadRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_files, rhs.m_files);
        swap(lhs.m_version, rhs.m_version);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    ModelVector<PFGameSaveFileToUploadWrapper<Alloc>, Alloc> const& GetFiles() const
    {
        return m_files;
    }

    void SetFiles(ModelVector<PFGameSaveFileToUploadWrapper<Alloc>, Alloc> value)
    {
        m_files = std::move(value);
        this->m_model.files =  m_files.empty() ? nullptr : m_files.data();
        this->m_model.filesCount =  static_cast<uint32_t>(m_files.size());
    }

    String const& GetVersion() const
    {
        return m_version;
    }

    void SetVersion(String value)
    {
        m_version = std::move(value);
        this->m_model.version =  m_version.empty() ? nullptr : m_version.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.files = m_files.empty() ? nullptr : m_files.data();
        this->m_model.version = m_version.empty() ? nullptr : m_version.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    ModelVector<PFGameSaveFileToUploadWrapper<Alloc>, Alloc> m_files;
    String m_version;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveAllocatedFileWrapper : public ModelWrapper<PFGameSaveAllocatedFile, Alloc>
{
public:
    using ModelType = PFGameSaveAllocatedFile;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveAllocatedFileWrapper() = default;

    PFGameSaveAllocatedFileWrapper(const PFGameSaveAllocatedFile& model) :
        ModelWrapper<PFGameSaveAllocatedFile, Alloc>{ model },
        m_fileName{ SafeString(model.fileName) },
        m_uploadUrl{ SafeString(model.uploadUrl) }
    {
        SetModelPointers();
    }

    PFGameSaveAllocatedFileWrapper(const PFGameSaveAllocatedFileWrapper& src) :
        PFGameSaveAllocatedFileWrapper{ src.Model() }
    {
    }

    PFGameSaveAllocatedFileWrapper(PFGameSaveAllocatedFileWrapper&& src) :
        PFGameSaveAllocatedFileWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveAllocatedFileWrapper& operator=(PFGameSaveAllocatedFileWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveAllocatedFileWrapper() = default;

    friend void swap(PFGameSaveAllocatedFileWrapper& lhs, PFGameSaveAllocatedFileWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_fileName, rhs.m_fileName);
        swap(lhs.m_uploadUrl, rhs.m_uploadUrl);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    time_t GetAccessTokenExpirationTime() const
    {
        return this->m_model.accessTokenExpirationTime;
    }

    void SetAccessTokenExpirationTime(time_t value)
    {
        this->m_model.accessTokenExpirationTime = value;
    }

    String const& GetFileName() const
    {
        return m_fileName;
    }

    void SetFileName(String value)
    {
        m_fileName = std::move(value);
        this->m_model.fileName =  m_fileName.empty() ? nullptr : m_fileName.data();
    }

    String const& GetUploadUrl() const
    {
        return m_uploadUrl;
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
class PFGameSaveInitiateUploadResponseWrapper : public ModelWrapper<PFGameSaveInitiateUploadResponse, Alloc>
{
public:
    using ModelType = PFGameSaveInitiateUploadResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveInitiateUploadResponseWrapper() = default;

    PFGameSaveInitiateUploadResponseWrapper(const PFGameSaveInitiateUploadResponse& model) :
        ModelWrapper<PFGameSaveInitiateUploadResponse, Alloc>{ model },
        m_files{ model.files, model.files + model.filesCount }
    {
        SetModelPointers();
    }

    PFGameSaveInitiateUploadResponseWrapper(const PFGameSaveInitiateUploadResponseWrapper& src) :
        PFGameSaveInitiateUploadResponseWrapper{ src.Model() }
    {
    }

    PFGameSaveInitiateUploadResponseWrapper(PFGameSaveInitiateUploadResponseWrapper&& src) :
        PFGameSaveInitiateUploadResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveInitiateUploadResponseWrapper& operator=(PFGameSaveInitiateUploadResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveInitiateUploadResponseWrapper() = default;

    friend void swap(PFGameSaveInitiateUploadResponseWrapper& lhs, PFGameSaveInitiateUploadResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_files, rhs.m_files);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    ModelVector<PFGameSaveAllocatedFileWrapper<Alloc>, Alloc> const& GetFiles() const
    {
        return m_files;
    }

    void SetFiles(ModelVector<PFGameSaveAllocatedFileWrapper<Alloc>, Alloc> value)
    {
        m_files = std::move(value);
        this->m_model.files =  m_files.empty() ? nullptr : m_files.data();
        this->m_model.filesCount =  static_cast<uint32_t>(m_files.size());
    }

    int32_t GetRecommendedChunkSizeBytes() const
    {
        return this->m_model.recommendedChunkSizeBytes;
    }

    void SetRecommendedChunkSizeBytes(int32_t value)
    {
        this->m_model.recommendedChunkSizeBytes = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.files = m_files.empty() ? nullptr : m_files.data();
    }

    ModelVector<PFGameSaveAllocatedFileWrapper<Alloc>, Alloc> m_files;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveListManifestsRequestWrapper : public ModelWrapper<PFGameSaveListManifestsRequest, Alloc>
{
public:
    using ModelType = PFGameSaveListManifestsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveListManifestsRequestWrapper() = default;

    PFGameSaveListManifestsRequestWrapper(const PFGameSaveListManifestsRequest& model) :
        ModelWrapper<PFGameSaveListManifestsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt }
    {
        SetModelPointers();
    }

    PFGameSaveListManifestsRequestWrapper(const PFGameSaveListManifestsRequestWrapper& src) :
        PFGameSaveListManifestsRequestWrapper{ src.Model() }
    {
    }

    PFGameSaveListManifestsRequestWrapper(PFGameSaveListManifestsRequestWrapper&& src) :
        PFGameSaveListManifestsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveListManifestsRequestWrapper& operator=(PFGameSaveListManifestsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveListManifestsRequestWrapper() = default;

    friend void swap(PFGameSaveListManifestsRequestWrapper& lhs, PFGameSaveListManifestsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    bool GetIncludeUnavailable() const
    {
        return this->m_model.includeUnavailable;
    }

    void SetIncludeUnavailable(bool value)
    {
        this->m_model.includeUnavailable = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveTitleConfigurationWrapper : public ModelWrapper<PFGameSaveTitleConfiguration, Alloc>
{
public:
    using ModelType = PFGameSaveTitleConfiguration;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveTitleConfigurationWrapper() = default;

    PFGameSaveTitleConfigurationWrapper(const PFGameSaveTitleConfiguration& model) :
        ModelWrapper<PFGameSaveTitleConfiguration, Alloc>{ model },
        m_perPlayerQuotaBytes{ SafeString(model.perPlayerQuotaBytes) }
    {
        SetModelPointers();
    }

    PFGameSaveTitleConfigurationWrapper(const PFGameSaveTitleConfigurationWrapper& src) :
        PFGameSaveTitleConfigurationWrapper{ src.Model() }
    {
    }

    PFGameSaveTitleConfigurationWrapper(PFGameSaveTitleConfigurationWrapper&& src) :
        PFGameSaveTitleConfigurationWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveTitleConfigurationWrapper& operator=(PFGameSaveTitleConfigurationWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveTitleConfigurationWrapper() = default;

    friend void swap(PFGameSaveTitleConfigurationWrapper& lhs, PFGameSaveTitleConfigurationWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_perPlayerQuotaBytes, rhs.m_perPlayerQuotaBytes);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    bool GetDisableClientRollbackToLastConflictLoser() const
    {
        return this->m_model.disableClientRollbackToLastConflictLoser;
    }

    void SetDisableClientRollbackToLastConflictLoser(bool value)
    {
        this->m_model.disableClientRollbackToLastConflictLoser = value;
    }

    bool GetDisableClientRollbackToLastKnownGood() const
    {
        return this->m_model.disableClientRollbackToLastKnownGood;
    }

    void SetDisableClientRollbackToLastKnownGood(bool value)
    {
        this->m_model.disableClientRollbackToLastKnownGood = value;
    }

    bool GetDisableUnsignaledOutOfProcessUpload() const
    {
        return this->m_model.disableUnsignaledOutOfProcessUpload;
    }

    void SetDisableUnsignaledOutOfProcessUpload(bool value)
    {
        this->m_model.disableUnsignaledOutOfProcessUpload = value;
    }

    String const& GetPerPlayerQuotaBytes() const
    {
        return m_perPlayerQuotaBytes;
    }

    void SetPerPlayerQuotaBytes(String value)
    {
        m_perPlayerQuotaBytes = std::move(value);
        this->m_model.perPlayerQuotaBytes =  m_perPlayerQuotaBytes.empty() ? nullptr : m_perPlayerQuotaBytes.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.perPlayerQuotaBytes = m_perPlayerQuotaBytes.empty() ? nullptr : m_perPlayerQuotaBytes.data();
    }

    String m_perPlayerQuotaBytes;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveListManifestsResponseWrapper : public ModelWrapper<PFGameSaveListManifestsResponse, Alloc>
{
public:
    using ModelType = PFGameSaveListManifestsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveListManifestsResponseWrapper() = default;

    PFGameSaveListManifestsResponseWrapper(const PFGameSaveListManifestsResponse& model) :
        ModelWrapper<PFGameSaveListManifestsResponse, Alloc>{ model },
        m_manifests{ model.manifests, model.manifests + model.manifestsCount },
        m_nextAvailableVersion{ SafeString(model.nextAvailableVersion) },
        m_titleConfiguration{ model.titleConfiguration ? std::optional<PFGameSaveTitleConfigurationWrapper<Alloc>>{ *model.titleConfiguration } : std::nullopt }
    {
        SetModelPointers();
    }

    PFGameSaveListManifestsResponseWrapper(const PFGameSaveListManifestsResponseWrapper& src) :
        PFGameSaveListManifestsResponseWrapper{ src.Model() }
    {
    }

    PFGameSaveListManifestsResponseWrapper(PFGameSaveListManifestsResponseWrapper&& src) :
        PFGameSaveListManifestsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveListManifestsResponseWrapper& operator=(PFGameSaveListManifestsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveListManifestsResponseWrapper() = default;

    friend void swap(PFGameSaveListManifestsResponseWrapper& lhs, PFGameSaveListManifestsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_manifests, rhs.m_manifests);
        swap(lhs.m_nextAvailableVersion, rhs.m_nextAvailableVersion);
        swap(lhs.m_titleConfiguration, rhs.m_titleConfiguration);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    ModelVector<PFGameSaveManifestWrapper<Alloc>, Alloc> const& GetManifests() const
    {
        return m_manifests;
    }

    void SetManifests(ModelVector<PFGameSaveManifestWrapper<Alloc>, Alloc> value)
    {
        m_manifests = std::move(value);
        this->m_model.manifests =  m_manifests.empty() ? nullptr : m_manifests.data();
        this->m_model.manifestsCount =  static_cast<uint32_t>(m_manifests.size());
    }

    String const& GetNextAvailableVersion() const
    {
        return m_nextAvailableVersion;
    }

    void SetNextAvailableVersion(String value)
    {
        m_nextAvailableVersion = std::move(value);
        this->m_model.nextAvailableVersion =  m_nextAvailableVersion.empty() ? nullptr : m_nextAvailableVersion.data();
    }

    std::optional<PFGameSaveTitleConfigurationWrapper<Alloc>> const& GetTitleConfiguration() const
    {
        return m_titleConfiguration;
    }

    void SetTitleConfiguration(std::optional<PFGameSaveTitleConfigurationWrapper<Alloc>> value)
    {
        m_titleConfiguration = std::move(value);
        this->m_model.titleConfiguration = m_titleConfiguration ? &m_titleConfiguration->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.manifests = m_manifests.empty() ? nullptr : m_manifests.data();
        this->m_model.nextAvailableVersion = m_nextAvailableVersion.empty() ? nullptr : m_nextAvailableVersion.data();
        this->m_model.titleConfiguration = m_titleConfiguration ?  &m_titleConfiguration->Model() : nullptr;
    }

    ModelVector<PFGameSaveManifestWrapper<Alloc>, Alloc> m_manifests;
    String m_nextAvailableVersion;
    std::optional<PFGameSaveTitleConfigurationWrapper<Alloc>> m_titleConfiguration;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveRequestOnboardingRequestWrapper : public ModelWrapper<PFGameSaveRequestOnboardingRequest, Alloc>
{
public:
    using ModelType = PFGameSaveRequestOnboardingRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveRequestOnboardingRequestWrapper() = default;

    PFGameSaveRequestOnboardingRequestWrapper(const PFGameSaveRequestOnboardingRequest& model) :
        ModelWrapper<PFGameSaveRequestOnboardingRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt }
    {
        SetModelPointers();
    }

    PFGameSaveRequestOnboardingRequestWrapper(const PFGameSaveRequestOnboardingRequestWrapper& src) :
        PFGameSaveRequestOnboardingRequestWrapper{ src.Model() }
    {
    }

    PFGameSaveRequestOnboardingRequestWrapper(PFGameSaveRequestOnboardingRequestWrapper&& src) :
        PFGameSaveRequestOnboardingRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveRequestOnboardingRequestWrapper& operator=(PFGameSaveRequestOnboardingRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveRequestOnboardingRequestWrapper() = default;

    friend void swap(PFGameSaveRequestOnboardingRequestWrapper& lhs, PFGameSaveRequestOnboardingRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveRollbackToManifestRequestWrapper : public ModelWrapper<PFGameSaveRollbackToManifestRequest, Alloc>
{
public:
    using ModelType = PFGameSaveRollbackToManifestRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveRollbackToManifestRequestWrapper() = default;

    PFGameSaveRollbackToManifestRequestWrapper(const PFGameSaveRollbackToManifestRequest& model) :
        ModelWrapper<PFGameSaveRollbackToManifestRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_rollbackReason{ SafeString(model.rollbackReason) },
        m_rollbackVersion{ SafeString(model.rollbackVersion) }
    {
        SetModelPointers();
    }

    PFGameSaveRollbackToManifestRequestWrapper(const PFGameSaveRollbackToManifestRequestWrapper& src) :
        PFGameSaveRollbackToManifestRequestWrapper{ src.Model() }
    {
    }

    PFGameSaveRollbackToManifestRequestWrapper(PFGameSaveRollbackToManifestRequestWrapper&& src) :
        PFGameSaveRollbackToManifestRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveRollbackToManifestRequestWrapper& operator=(PFGameSaveRollbackToManifestRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveRollbackToManifestRequestWrapper() = default;

    friend void swap(PFGameSaveRollbackToManifestRequestWrapper& lhs, PFGameSaveRollbackToManifestRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_rollbackReason, rhs.m_rollbackReason);
        swap(lhs.m_rollbackVersion, rhs.m_rollbackVersion);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    String const& GetRollbackReason() const
    {
        return m_rollbackReason;
    }

    void SetRollbackReason(String value)
    {
        m_rollbackReason = std::move(value);
        this->m_model.rollbackReason =  m_rollbackReason.empty() ? nullptr : m_rollbackReason.data();
    }

    String const& GetRollbackVersion() const
    {
        return m_rollbackVersion;
    }

    void SetRollbackVersion(String value)
    {
        m_rollbackVersion = std::move(value);
        this->m_model.rollbackVersion =  m_rollbackVersion.empty() ? nullptr : m_rollbackVersion.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.rollbackReason = m_rollbackReason.empty() ? nullptr : m_rollbackReason.data();
        this->m_model.rollbackVersion = m_rollbackVersion.empty() ? nullptr : m_rollbackVersion.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_rollbackReason;
    String m_rollbackVersion;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveRollbackToManifestResponseWrapper : public ModelWrapper<PFGameSaveRollbackToManifestResponse, Alloc>
{
public:
    using ModelType = PFGameSaveRollbackToManifestResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveRollbackToManifestResponseWrapper() = default;

    PFGameSaveRollbackToManifestResponseWrapper(const PFGameSaveRollbackToManifestResponse& model) :
        ModelWrapper<PFGameSaveRollbackToManifestResponse, Alloc>{ model },
        m_manifest{ model.manifest ? std::optional<PFGameSaveManifestWrapper<Alloc>>{ *model.manifest } : std::nullopt }
    {
        SetModelPointers();
    }

    PFGameSaveRollbackToManifestResponseWrapper(const PFGameSaveRollbackToManifestResponseWrapper& src) :
        PFGameSaveRollbackToManifestResponseWrapper{ src.Model() }
    {
    }

    PFGameSaveRollbackToManifestResponseWrapper(PFGameSaveRollbackToManifestResponseWrapper&& src) :
        PFGameSaveRollbackToManifestResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveRollbackToManifestResponseWrapper& operator=(PFGameSaveRollbackToManifestResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveRollbackToManifestResponseWrapper() = default;

    friend void swap(PFGameSaveRollbackToManifestResponseWrapper& lhs, PFGameSaveRollbackToManifestResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_manifest, rhs.m_manifest);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    std::optional<PFGameSaveManifestWrapper<Alloc>> const& GetManifest() const
    {
        return m_manifest;
    }

    void SetManifest(std::optional<PFGameSaveManifestWrapper<Alloc>> value)
    {
        m_manifest = std::move(value);
        this->m_model.manifest = m_manifest ? &m_manifest->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.manifest = m_manifest ?  &m_manifest->Model() : nullptr;
    }

    std::optional<PFGameSaveManifestWrapper<Alloc>> m_manifest;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveUpdateConfigForTitleRequestWrapper : public ModelWrapper<PFGameSaveUpdateConfigForTitleRequest, Alloc>
{
public:
    using ModelType = PFGameSaveUpdateConfigForTitleRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveUpdateConfigForTitleRequestWrapper() = default;

    PFGameSaveUpdateConfigForTitleRequestWrapper(const PFGameSaveUpdateConfigForTitleRequest& model) :
        ModelWrapper<PFGameSaveUpdateConfigForTitleRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_disableClientRollbackToLastConflictLoser{ model.disableClientRollbackToLastConflictLoser ? std::optional<bool>{ *model.disableClientRollbackToLastConflictLoser } : std::nullopt },
        m_disableClientRollbackToLastKnownGood{ model.disableClientRollbackToLastKnownGood ? std::optional<bool>{ *model.disableClientRollbackToLastKnownGood } : std::nullopt },
        m_disableUnsignaledOutOfProcessUpload{ model.disableUnsignaledOutOfProcessUpload ? std::optional<bool>{ *model.disableUnsignaledOutOfProcessUpload } : std::nullopt },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt }
    {
        SetModelPointers();
    }

    PFGameSaveUpdateConfigForTitleRequestWrapper(const PFGameSaveUpdateConfigForTitleRequestWrapper& src) :
        PFGameSaveUpdateConfigForTitleRequestWrapper{ src.Model() }
    {
    }

    PFGameSaveUpdateConfigForTitleRequestWrapper(PFGameSaveUpdateConfigForTitleRequestWrapper&& src) :
        PFGameSaveUpdateConfigForTitleRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveUpdateConfigForTitleRequestWrapper& operator=(PFGameSaveUpdateConfigForTitleRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveUpdateConfigForTitleRequestWrapper() = default;

    friend void swap(PFGameSaveUpdateConfigForTitleRequestWrapper& lhs, PFGameSaveUpdateConfigForTitleRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_disableClientRollbackToLastConflictLoser, rhs.m_disableClientRollbackToLastConflictLoser);
        swap(lhs.m_disableClientRollbackToLastKnownGood, rhs.m_disableClientRollbackToLastKnownGood);
        swap(lhs.m_disableUnsignaledOutOfProcessUpload, rhs.m_disableUnsignaledOutOfProcessUpload);
        swap(lhs.m_entity, rhs.m_entity);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    std::optional<bool> const& GetDisableClientRollbackToLastConflictLoser() const
    {
        return m_disableClientRollbackToLastConflictLoser;
    }

    void SetDisableClientRollbackToLastConflictLoser(std::optional<bool> value)
    {
        m_disableClientRollbackToLastConflictLoser = std::move(value);
        this->m_model.disableClientRollbackToLastConflictLoser = m_disableClientRollbackToLastConflictLoser ? m_disableClientRollbackToLastConflictLoser.operator->() : nullptr;
    }

    std::optional<bool> const& GetDisableClientRollbackToLastKnownGood() const
    {
        return m_disableClientRollbackToLastKnownGood;
    }

    void SetDisableClientRollbackToLastKnownGood(std::optional<bool> value)
    {
        m_disableClientRollbackToLastKnownGood = std::move(value);
        this->m_model.disableClientRollbackToLastKnownGood = m_disableClientRollbackToLastKnownGood ? m_disableClientRollbackToLastKnownGood.operator->() : nullptr;
    }

    std::optional<bool> const& GetDisableUnsignaledOutOfProcessUpload() const
    {
        return m_disableUnsignaledOutOfProcessUpload;
    }

    void SetDisableUnsignaledOutOfProcessUpload(std::optional<bool> value)
    {
        m_disableUnsignaledOutOfProcessUpload = std::move(value);
        this->m_model.disableUnsignaledOutOfProcessUpload = m_disableUnsignaledOutOfProcessUpload ? m_disableUnsignaledOutOfProcessUpload.operator->() : nullptr;
    }

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.disableClientRollbackToLastConflictLoser = m_disableClientRollbackToLastConflictLoser ? m_disableClientRollbackToLastConflictLoser.operator->() : nullptr;
        this->m_model.disableClientRollbackToLastKnownGood = m_disableClientRollbackToLastKnownGood ? m_disableClientRollbackToLastKnownGood.operator->() : nullptr;
        this->m_model.disableUnsignaledOutOfProcessUpload = m_disableUnsignaledOutOfProcessUpload ? m_disableUnsignaledOutOfProcessUpload.operator->() : nullptr;
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_disableClientRollbackToLastConflictLoser;
    std::optional<bool> m_disableClientRollbackToLastKnownGood;
    std::optional<bool> m_disableUnsignaledOutOfProcessUpload;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveUpdateConfigForTitleResponseWrapper : public ModelWrapper<PFGameSaveUpdateConfigForTitleResponse, Alloc>
{
public:
    using ModelType = PFGameSaveUpdateConfigForTitleResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveUpdateConfigForTitleResponseWrapper() = default;

    PFGameSaveUpdateConfigForTitleResponseWrapper(const PFGameSaveUpdateConfigForTitleResponse& model) :
        ModelWrapper<PFGameSaveUpdateConfigForTitleResponse, Alloc>{ model },
        m_onboardingStatus{ SafeString(model.onboardingStatus) },
        m_perPlayerQuotaBytes{ SafeString(model.perPlayerQuotaBytes) }
    {
        SetModelPointers();
    }

    PFGameSaveUpdateConfigForTitleResponseWrapper(const PFGameSaveUpdateConfigForTitleResponseWrapper& src) :
        PFGameSaveUpdateConfigForTitleResponseWrapper{ src.Model() }
    {
    }

    PFGameSaveUpdateConfigForTitleResponseWrapper(PFGameSaveUpdateConfigForTitleResponseWrapper&& src) :
        PFGameSaveUpdateConfigForTitleResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveUpdateConfigForTitleResponseWrapper& operator=(PFGameSaveUpdateConfigForTitleResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveUpdateConfigForTitleResponseWrapper() = default;

    friend void swap(PFGameSaveUpdateConfigForTitleResponseWrapper& lhs, PFGameSaveUpdateConfigForTitleResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_onboardingStatus, rhs.m_onboardingStatus);
        swap(lhs.m_perPlayerQuotaBytes, rhs.m_perPlayerQuotaBytes);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    bool GetDisableClientRollbackToLastConflictLoser() const
    {
        return this->m_model.disableClientRollbackToLastConflictLoser;
    }

    void SetDisableClientRollbackToLastConflictLoser(bool value)
    {
        this->m_model.disableClientRollbackToLastConflictLoser = value;
    }

    bool GetDisableClientRollbackToLastKnownGood() const
    {
        return this->m_model.disableClientRollbackToLastKnownGood;
    }

    void SetDisableClientRollbackToLastKnownGood(bool value)
    {
        this->m_model.disableClientRollbackToLastKnownGood = value;
    }

    bool GetDisableUnsignaledOutOfProcessUpload() const
    {
        return this->m_model.disableUnsignaledOutOfProcessUpload;
    }

    void SetDisableUnsignaledOutOfProcessUpload(bool value)
    {
        this->m_model.disableUnsignaledOutOfProcessUpload = value;
    }

    String const& GetOnboardingStatus() const
    {
        return m_onboardingStatus;
    }

    void SetOnboardingStatus(String value)
    {
        m_onboardingStatus = std::move(value);
        this->m_model.onboardingStatus =  m_onboardingStatus.empty() ? nullptr : m_onboardingStatus.data();
    }

    String const& GetPerPlayerQuotaBytes() const
    {
        return m_perPlayerQuotaBytes;
    }

    void SetPerPlayerQuotaBytes(String value)
    {
        m_perPlayerQuotaBytes = std::move(value);
        this->m_model.perPlayerQuotaBytes =  m_perPlayerQuotaBytes.empty() ? nullptr : m_perPlayerQuotaBytes.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.onboardingStatus = m_onboardingStatus.empty() ? nullptr : m_onboardingStatus.data();
        this->m_model.perPlayerQuotaBytes = m_perPlayerQuotaBytes.empty() ? nullptr : m_perPlayerQuotaBytes.data();
    }

    String m_onboardingStatus;
    String m_perPlayerQuotaBytes;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveUpdateManifestRequestWrapper : public ModelWrapper<PFGameSaveUpdateManifestRequest, Alloc>
{
public:
    using ModelType = PFGameSaveUpdateManifestRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveUpdateManifestRequestWrapper() = default;

    PFGameSaveUpdateManifestRequestWrapper(const PFGameSaveUpdateManifestRequest& model) :
        ModelWrapper<PFGameSaveUpdateManifestRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_manifestDescription{ SafeString(model.manifestDescription) },
        m_markAsKnownGood{ model.markAsKnownGood ? std::optional<bool>{ *model.markAsKnownGood } : std::nullopt },
        m_uploadProgress{ model.uploadProgress ? std::optional<PFGameSaveUploadProgressWrapper<Alloc>>{ *model.uploadProgress } : std::nullopt },
        m_version{ SafeString(model.version) }
    {
        SetModelPointers();
    }

    PFGameSaveUpdateManifestRequestWrapper(const PFGameSaveUpdateManifestRequestWrapper& src) :
        PFGameSaveUpdateManifestRequestWrapper{ src.Model() }
    {
    }

    PFGameSaveUpdateManifestRequestWrapper(PFGameSaveUpdateManifestRequestWrapper&& src) :
        PFGameSaveUpdateManifestRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveUpdateManifestRequestWrapper& operator=(PFGameSaveUpdateManifestRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveUpdateManifestRequestWrapper() = default;

    friend void swap(PFGameSaveUpdateManifestRequestWrapper& lhs, PFGameSaveUpdateManifestRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_manifestDescription, rhs.m_manifestDescription);
        swap(lhs.m_markAsKnownGood, rhs.m_markAsKnownGood);
        swap(lhs.m_uploadProgress, rhs.m_uploadProgress);
        swap(lhs.m_version, rhs.m_version);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    String const& GetManifestDescription() const
    {
        return m_manifestDescription;
    }

    void SetManifestDescription(String value)
    {
        m_manifestDescription = std::move(value);
        this->m_model.manifestDescription =  m_manifestDescription.empty() ? nullptr : m_manifestDescription.data();
    }

    std::optional<bool> const& GetMarkAsKnownGood() const
    {
        return m_markAsKnownGood;
    }

    void SetMarkAsKnownGood(std::optional<bool> value)
    {
        m_markAsKnownGood = std::move(value);
        this->m_model.markAsKnownGood = m_markAsKnownGood ? m_markAsKnownGood.operator->() : nullptr;
    }

    std::optional<PFGameSaveUploadProgressWrapper<Alloc>> const& GetUploadProgress() const
    {
        return m_uploadProgress;
    }

    void SetUploadProgress(std::optional<PFGameSaveUploadProgressWrapper<Alloc>> value)
    {
        m_uploadProgress = std::move(value);
        this->m_model.uploadProgress = m_uploadProgress ? &m_uploadProgress->Model() : nullptr;
    }

    String const& GetVersion() const
    {
        return m_version;
    }

    void SetVersion(String value)
    {
        m_version = std::move(value);
        this->m_model.version =  m_version.empty() ? nullptr : m_version.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.manifestDescription = m_manifestDescription.empty() ? nullptr : m_manifestDescription.data();
        this->m_model.markAsKnownGood = m_markAsKnownGood ? m_markAsKnownGood.operator->() : nullptr;
        this->m_model.uploadProgress = m_uploadProgress ?  &m_uploadProgress->Model() : nullptr;
        this->m_model.version = m_version.empty() ? nullptr : m_version.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_manifestDescription;
    std::optional<bool> m_markAsKnownGood;
    std::optional<PFGameSaveUploadProgressWrapper<Alloc>> m_uploadProgress;
    String m_version;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGameSaveUpdateManifestResponseWrapper : public ModelWrapper<PFGameSaveUpdateManifestResponse, Alloc>
{
public:
    using ModelType = PFGameSaveUpdateManifestResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGameSaveUpdateManifestResponseWrapper() = default;

    PFGameSaveUpdateManifestResponseWrapper(const PFGameSaveUpdateManifestResponse& model) :
        ModelWrapper<PFGameSaveUpdateManifestResponse, Alloc>{ model },
        m_manifest{ model.manifest ? std::optional<PFGameSaveManifestWrapper<Alloc>>{ *model.manifest } : std::nullopt }
    {
        SetModelPointers();
    }

    PFGameSaveUpdateManifestResponseWrapper(const PFGameSaveUpdateManifestResponseWrapper& src) :
        PFGameSaveUpdateManifestResponseWrapper{ src.Model() }
    {
    }

    PFGameSaveUpdateManifestResponseWrapper(PFGameSaveUpdateManifestResponseWrapper&& src) :
        PFGameSaveUpdateManifestResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGameSaveUpdateManifestResponseWrapper& operator=(PFGameSaveUpdateManifestResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGameSaveUpdateManifestResponseWrapper() = default;

    friend void swap(PFGameSaveUpdateManifestResponseWrapper& lhs, PFGameSaveUpdateManifestResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_manifest, rhs.m_manifest);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    std::optional<PFGameSaveManifestWrapper<Alloc>> const& GetManifest() const
    {
        return m_manifest;
    }

    void SetManifest(std::optional<PFGameSaveManifestWrapper<Alloc>> value)
    {
        m_manifest = std::move(value);
        this->m_model.manifest = m_manifest ? &m_manifest->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.manifest = m_manifest ?  &m_manifest->Model() : nullptr;
    }

    std::optional<PFGameSaveManifestWrapper<Alloc>> m_manifest;
};

} // namespace Wrappers
} // namespace PlayFab
