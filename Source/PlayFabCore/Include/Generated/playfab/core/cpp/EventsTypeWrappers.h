// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/cpp/TypeWrappers.h>
#include <playfab/core/PFEventsTypes.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsDeleteDataConnectionRequestWrapper : public ModelWrapper<PFEventsDeleteDataConnectionRequest, Alloc>
{
public:
    using ModelType = PFEventsDeleteDataConnectionRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsDeleteDataConnectionRequestWrapper() = default;

    PFEventsDeleteDataConnectionRequestWrapper(const PFEventsDeleteDataConnectionRequest& model) :
        ModelWrapper<PFEventsDeleteDataConnectionRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFEventsDeleteDataConnectionRequestWrapper(const PFEventsDeleteDataConnectionRequestWrapper& src) :
        PFEventsDeleteDataConnectionRequestWrapper{ src.Model() }
    {
    }

    PFEventsDeleteDataConnectionRequestWrapper(PFEventsDeleteDataConnectionRequestWrapper&& src) :
        PFEventsDeleteDataConnectionRequestWrapper{}
    {
        swap(*this, src);
    }

    PFEventsDeleteDataConnectionRequestWrapper& operator=(PFEventsDeleteDataConnectionRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsDeleteDataConnectionRequestWrapper() = default;

    friend void swap(PFEventsDeleteDataConnectionRequestWrapper& lhs, PFEventsDeleteDataConnectionRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_name, rhs.m_name);
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

    String const& GetName() const
    {
        return m_name;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsDeleteDataConnectionResponseWrapper : public ModelWrapper<PFEventsDeleteDataConnectionResponse, Alloc>
{
public:
    using ModelType = PFEventsDeleteDataConnectionResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFEventsDeleteDataConnectionResponse, Alloc>::ModelWrapper;

    bool GetWasDeleted() const
    {
        return this->m_model.wasDeleted;
    }

    void SetWasDeleted(bool value)
    {
        this->m_model.wasDeleted = value;
    }

private:
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsGetDataConnectionRequestWrapper : public ModelWrapper<PFEventsGetDataConnectionRequest, Alloc>
{
public:
    using ModelType = PFEventsGetDataConnectionRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsGetDataConnectionRequestWrapper() = default;

    PFEventsGetDataConnectionRequestWrapper(const PFEventsGetDataConnectionRequest& model) :
        ModelWrapper<PFEventsGetDataConnectionRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFEventsGetDataConnectionRequestWrapper(const PFEventsGetDataConnectionRequestWrapper& src) :
        PFEventsGetDataConnectionRequestWrapper{ src.Model() }
    {
    }

    PFEventsGetDataConnectionRequestWrapper(PFEventsGetDataConnectionRequestWrapper&& src) :
        PFEventsGetDataConnectionRequestWrapper{}
    {
        swap(*this, src);
    }

    PFEventsGetDataConnectionRequestWrapper& operator=(PFEventsGetDataConnectionRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsGetDataConnectionRequestWrapper() = default;

    friend void swap(PFEventsGetDataConnectionRequestWrapper& lhs, PFEventsGetDataConnectionRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_name, rhs.m_name);
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

    String const& GetName() const
    {
        return m_name;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsDataConnectionAzureBlobSettingsWrapper : public ModelWrapper<PFEventsDataConnectionAzureBlobSettings, Alloc>
{
public:
    using ModelType = PFEventsDataConnectionAzureBlobSettings;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsDataConnectionAzureBlobSettingsWrapper() = default;

    PFEventsDataConnectionAzureBlobSettingsWrapper(const PFEventsDataConnectionAzureBlobSettings& model) :
        ModelWrapper<PFEventsDataConnectionAzureBlobSettings, Alloc>{ model },
        m_accountName{ SafeString(model.accountName) },
        m_containerName{ SafeString(model.containerName) },
        m_tenantId{ SafeString(model.tenantId) }
    {
        SetModelPointers();
    }

    PFEventsDataConnectionAzureBlobSettingsWrapper(const PFEventsDataConnectionAzureBlobSettingsWrapper& src) :
        PFEventsDataConnectionAzureBlobSettingsWrapper{ src.Model() }
    {
    }

    PFEventsDataConnectionAzureBlobSettingsWrapper(PFEventsDataConnectionAzureBlobSettingsWrapper&& src) :
        PFEventsDataConnectionAzureBlobSettingsWrapper{}
    {
        swap(*this, src);
    }

    PFEventsDataConnectionAzureBlobSettingsWrapper& operator=(PFEventsDataConnectionAzureBlobSettingsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsDataConnectionAzureBlobSettingsWrapper() = default;

    friend void swap(PFEventsDataConnectionAzureBlobSettingsWrapper& lhs, PFEventsDataConnectionAzureBlobSettingsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_accountName, rhs.m_accountName);
        swap(lhs.m_containerName, rhs.m_containerName);
        swap(lhs.m_tenantId, rhs.m_tenantId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetAccountName() const
    {
        return m_accountName;
    }

    void SetAccountName(String value)
    {
        m_accountName = std::move(value);
        this->m_model.accountName =  m_accountName.empty() ? nullptr : m_accountName.data();
    }

    String const& GetContainerName() const
    {
        return m_containerName;
    }

    void SetContainerName(String value)
    {
        m_containerName = std::move(value);
        this->m_model.containerName =  m_containerName.empty() ? nullptr : m_containerName.data();
    }

    String const& GetTenantId() const
    {
        return m_tenantId;
    }

    void SetTenantId(String value)
    {
        m_tenantId = std::move(value);
        this->m_model.tenantId =  m_tenantId.empty() ? nullptr : m_tenantId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.accountName = m_accountName.empty() ? nullptr : m_accountName.data();
        this->m_model.containerName = m_containerName.empty() ? nullptr : m_containerName.data();
        this->m_model.tenantId = m_tenantId.empty() ? nullptr : m_tenantId.data();
    }

    String m_accountName;
    String m_containerName;
    String m_tenantId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsDataConnectionAzureDataExplorerSettingsWrapper : public ModelWrapper<PFEventsDataConnectionAzureDataExplorerSettings, Alloc>
{
public:
    using ModelType = PFEventsDataConnectionAzureDataExplorerSettings;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsDataConnectionAzureDataExplorerSettingsWrapper() = default;

    PFEventsDataConnectionAzureDataExplorerSettingsWrapper(const PFEventsDataConnectionAzureDataExplorerSettings& model) :
        ModelWrapper<PFEventsDataConnectionAzureDataExplorerSettings, Alloc>{ model },
        m_clusterUri{ SafeString(model.clusterUri) },
        m_database{ SafeString(model.database) },
        m_table{ SafeString(model.table) }
    {
        SetModelPointers();
    }

    PFEventsDataConnectionAzureDataExplorerSettingsWrapper(const PFEventsDataConnectionAzureDataExplorerSettingsWrapper& src) :
        PFEventsDataConnectionAzureDataExplorerSettingsWrapper{ src.Model() }
    {
    }

    PFEventsDataConnectionAzureDataExplorerSettingsWrapper(PFEventsDataConnectionAzureDataExplorerSettingsWrapper&& src) :
        PFEventsDataConnectionAzureDataExplorerSettingsWrapper{}
    {
        swap(*this, src);
    }

    PFEventsDataConnectionAzureDataExplorerSettingsWrapper& operator=(PFEventsDataConnectionAzureDataExplorerSettingsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsDataConnectionAzureDataExplorerSettingsWrapper() = default;

    friend void swap(PFEventsDataConnectionAzureDataExplorerSettingsWrapper& lhs, PFEventsDataConnectionAzureDataExplorerSettingsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_clusterUri, rhs.m_clusterUri);
        swap(lhs.m_database, rhs.m_database);
        swap(lhs.m_table, rhs.m_table);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetClusterUri() const
    {
        return m_clusterUri;
    }

    void SetClusterUri(String value)
    {
        m_clusterUri = std::move(value);
        this->m_model.clusterUri =  m_clusterUri.empty() ? nullptr : m_clusterUri.data();
    }

    String const& GetDatabase() const
    {
        return m_database;
    }

    void SetDatabase(String value)
    {
        m_database = std::move(value);
        this->m_model.database =  m_database.empty() ? nullptr : m_database.data();
    }

    String const& GetTable() const
    {
        return m_table;
    }

    void SetTable(String value)
    {
        m_table = std::move(value);
        this->m_model.table =  m_table.empty() ? nullptr : m_table.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.clusterUri = m_clusterUri.empty() ? nullptr : m_clusterUri.data();
        this->m_model.database = m_database.empty() ? nullptr : m_database.data();
        this->m_model.table = m_table.empty() ? nullptr : m_table.data();
    }

    String m_clusterUri;
    String m_database;
    String m_table;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsDataConnectionFabricKQLSettingsWrapper : public ModelWrapper<PFEventsDataConnectionFabricKQLSettings, Alloc>
{
public:
    using ModelType = PFEventsDataConnectionFabricKQLSettings;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsDataConnectionFabricKQLSettingsWrapper() = default;

    PFEventsDataConnectionFabricKQLSettingsWrapper(const PFEventsDataConnectionFabricKQLSettings& model) :
        ModelWrapper<PFEventsDataConnectionFabricKQLSettings, Alloc>{ model },
        m_clusterUri{ SafeString(model.clusterUri) },
        m_database{ SafeString(model.database) },
        m_table{ SafeString(model.table) }
    {
        SetModelPointers();
    }

    PFEventsDataConnectionFabricKQLSettingsWrapper(const PFEventsDataConnectionFabricKQLSettingsWrapper& src) :
        PFEventsDataConnectionFabricKQLSettingsWrapper{ src.Model() }
    {
    }

    PFEventsDataConnectionFabricKQLSettingsWrapper(PFEventsDataConnectionFabricKQLSettingsWrapper&& src) :
        PFEventsDataConnectionFabricKQLSettingsWrapper{}
    {
        swap(*this, src);
    }

    PFEventsDataConnectionFabricKQLSettingsWrapper& operator=(PFEventsDataConnectionFabricKQLSettingsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsDataConnectionFabricKQLSettingsWrapper() = default;

    friend void swap(PFEventsDataConnectionFabricKQLSettingsWrapper& lhs, PFEventsDataConnectionFabricKQLSettingsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_clusterUri, rhs.m_clusterUri);
        swap(lhs.m_database, rhs.m_database);
        swap(lhs.m_table, rhs.m_table);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetClusterUri() const
    {
        return m_clusterUri;
    }

    void SetClusterUri(String value)
    {
        m_clusterUri = std::move(value);
        this->m_model.clusterUri =  m_clusterUri.empty() ? nullptr : m_clusterUri.data();
    }

    String const& GetDatabase() const
    {
        return m_database;
    }

    void SetDatabase(String value)
    {
        m_database = std::move(value);
        this->m_model.database =  m_database.empty() ? nullptr : m_database.data();
    }

    String const& GetTable() const
    {
        return m_table;
    }

    void SetTable(String value)
    {
        m_table = std::move(value);
        this->m_model.table =  m_table.empty() ? nullptr : m_table.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.clusterUri = m_clusterUri.empty() ? nullptr : m_clusterUri.data();
        this->m_model.database = m_database.empty() ? nullptr : m_database.data();
        this->m_model.table = m_table.empty() ? nullptr : m_table.data();
    }

    String m_clusterUri;
    String m_database;
    String m_table;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsDataConnectionSettingsWrapper : public ModelWrapper<PFEventsDataConnectionSettings, Alloc>
{
public:
    using ModelType = PFEventsDataConnectionSettings;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsDataConnectionSettingsWrapper() = default;

    PFEventsDataConnectionSettingsWrapper(const PFEventsDataConnectionSettings& model) :
        ModelWrapper<PFEventsDataConnectionSettings, Alloc>{ model },
        m_azureBlobSettings{ model.azureBlobSettings ? std::optional<PFEventsDataConnectionAzureBlobSettingsWrapper<Alloc>>{ *model.azureBlobSettings } : std::nullopt },
        m_azureDataExplorerSettings{ model.azureDataExplorerSettings ? std::optional<PFEventsDataConnectionAzureDataExplorerSettingsWrapper<Alloc>>{ *model.azureDataExplorerSettings } : std::nullopt },
        m_azureFabricKQLSettings{ model.azureFabricKQLSettings ? std::optional<PFEventsDataConnectionFabricKQLSettingsWrapper<Alloc>>{ *model.azureFabricKQLSettings } : std::nullopt }
    {
        SetModelPointers();
    }

    PFEventsDataConnectionSettingsWrapper(const PFEventsDataConnectionSettingsWrapper& src) :
        PFEventsDataConnectionSettingsWrapper{ src.Model() }
    {
    }

    PFEventsDataConnectionSettingsWrapper(PFEventsDataConnectionSettingsWrapper&& src) :
        PFEventsDataConnectionSettingsWrapper{}
    {
        swap(*this, src);
    }

    PFEventsDataConnectionSettingsWrapper& operator=(PFEventsDataConnectionSettingsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsDataConnectionSettingsWrapper() = default;

    friend void swap(PFEventsDataConnectionSettingsWrapper& lhs, PFEventsDataConnectionSettingsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_azureBlobSettings, rhs.m_azureBlobSettings);
        swap(lhs.m_azureDataExplorerSettings, rhs.m_azureDataExplorerSettings);
        swap(lhs.m_azureFabricKQLSettings, rhs.m_azureFabricKQLSettings);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    std::optional<PFEventsDataConnectionAzureBlobSettingsWrapper<Alloc>> const& GetAzureBlobSettings() const
    {
        return m_azureBlobSettings;
    }

    void SetAzureBlobSettings(std::optional<PFEventsDataConnectionAzureBlobSettingsWrapper<Alloc>> value)
    {
        m_azureBlobSettings = std::move(value);
        this->m_model.azureBlobSettings = m_azureBlobSettings ? &m_azureBlobSettings->Model() : nullptr;
    }

    std::optional<PFEventsDataConnectionAzureDataExplorerSettingsWrapper<Alloc>> const& GetAzureDataExplorerSettings() const
    {
        return m_azureDataExplorerSettings;
    }

    void SetAzureDataExplorerSettings(std::optional<PFEventsDataConnectionAzureDataExplorerSettingsWrapper<Alloc>> value)
    {
        m_azureDataExplorerSettings = std::move(value);
        this->m_model.azureDataExplorerSettings = m_azureDataExplorerSettings ? &m_azureDataExplorerSettings->Model() : nullptr;
    }

    std::optional<PFEventsDataConnectionFabricKQLSettingsWrapper<Alloc>> const& GetAzureFabricKQLSettings() const
    {
        return m_azureFabricKQLSettings;
    }

    void SetAzureFabricKQLSettings(std::optional<PFEventsDataConnectionFabricKQLSettingsWrapper<Alloc>> value)
    {
        m_azureFabricKQLSettings = std::move(value);
        this->m_model.azureFabricKQLSettings = m_azureFabricKQLSettings ? &m_azureFabricKQLSettings->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.azureBlobSettings = m_azureBlobSettings ?  &m_azureBlobSettings->Model() : nullptr;
        this->m_model.azureDataExplorerSettings = m_azureDataExplorerSettings ?  &m_azureDataExplorerSettings->Model() : nullptr;
        this->m_model.azureFabricKQLSettings = m_azureFabricKQLSettings ?  &m_azureFabricKQLSettings->Model() : nullptr;
    }

    std::optional<PFEventsDataConnectionAzureBlobSettingsWrapper<Alloc>> m_azureBlobSettings;
    std::optional<PFEventsDataConnectionAzureDataExplorerSettingsWrapper<Alloc>> m_azureDataExplorerSettings;
    std::optional<PFEventsDataConnectionFabricKQLSettingsWrapper<Alloc>> m_azureFabricKQLSettings;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsDataConnectionStatusDetailsWrapper : public ModelWrapper<PFEventsDataConnectionStatusDetails, Alloc>
{
public:
    using ModelType = PFEventsDataConnectionStatusDetails;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsDataConnectionStatusDetailsWrapper() = default;

    PFEventsDataConnectionStatusDetailsWrapper(const PFEventsDataConnectionStatusDetails& model) :
        ModelWrapper<PFEventsDataConnectionStatusDetails, Alloc>{ model },
        m_error{ SafeString(model.error) },
        m_errorMessage{ SafeString(model.errorMessage) },
        m_mostRecentErrorTime{ model.mostRecentErrorTime ? std::optional<time_t>{ *model.mostRecentErrorTime } : std::nullopt },
        m_state{ model.state ? std::optional<PFEventsDataConnectionErrorState>{ *model.state } : std::nullopt }
    {
        SetModelPointers();
    }

    PFEventsDataConnectionStatusDetailsWrapper(const PFEventsDataConnectionStatusDetailsWrapper& src) :
        PFEventsDataConnectionStatusDetailsWrapper{ src.Model() }
    {
    }

    PFEventsDataConnectionStatusDetailsWrapper(PFEventsDataConnectionStatusDetailsWrapper&& src) :
        PFEventsDataConnectionStatusDetailsWrapper{}
    {
        swap(*this, src);
    }

    PFEventsDataConnectionStatusDetailsWrapper& operator=(PFEventsDataConnectionStatusDetailsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsDataConnectionStatusDetailsWrapper() = default;

    friend void swap(PFEventsDataConnectionStatusDetailsWrapper& lhs, PFEventsDataConnectionStatusDetailsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_error, rhs.m_error);
        swap(lhs.m_errorMessage, rhs.m_errorMessage);
        swap(lhs.m_mostRecentErrorTime, rhs.m_mostRecentErrorTime);
        swap(lhs.m_state, rhs.m_state);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetError() const
    {
        return m_error;
    }

    void SetError(String value)
    {
        m_error = std::move(value);
        this->m_model.error =  m_error.empty() ? nullptr : m_error.data();
    }

    String const& GetErrorMessage() const
    {
        return m_errorMessage;
    }

    void SetErrorMessage(String value)
    {
        m_errorMessage = std::move(value);
        this->m_model.errorMessage =  m_errorMessage.empty() ? nullptr : m_errorMessage.data();
    }

    std::optional<time_t> const& GetMostRecentErrorTime() const
    {
        return m_mostRecentErrorTime;
    }

    void SetMostRecentErrorTime(std::optional<time_t> value)
    {
        m_mostRecentErrorTime = std::move(value);
        this->m_model.mostRecentErrorTime = m_mostRecentErrorTime ? m_mostRecentErrorTime.operator->() : nullptr;
    }

    std::optional<PFEventsDataConnectionErrorState> const& GetState() const
    {
        return m_state;
    }

    void SetState(std::optional<PFEventsDataConnectionErrorState> value)
    {
        m_state = std::move(value);
        this->m_model.state = m_state ? m_state.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.error = m_error.empty() ? nullptr : m_error.data();
        this->m_model.errorMessage = m_errorMessage.empty() ? nullptr : m_errorMessage.data();
        this->m_model.mostRecentErrorTime = m_mostRecentErrorTime ? m_mostRecentErrorTime.operator->() : nullptr;
        this->m_model.state = m_state ? m_state.operator->() : nullptr;
    }

    String m_error;
    String m_errorMessage;
    std::optional<time_t> m_mostRecentErrorTime;
    std::optional<PFEventsDataConnectionErrorState> m_state;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsDataConnectionDetailsWrapper : public ModelWrapper<PFEventsDataConnectionDetails, Alloc>
{
public:
    using ModelType = PFEventsDataConnectionDetails;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsDataConnectionDetailsWrapper() = default;

    PFEventsDataConnectionDetailsWrapper(const PFEventsDataConnectionDetails& model) :
        ModelWrapper<PFEventsDataConnectionDetails, Alloc>{ model },
        m_connectionSettings{ model.connectionSettings ? *model.connectionSettings : decltype(*model.connectionSettings){} },
        m_name{ SafeString(model.name) },
        m_status{ model.status ? std::optional<PFEventsDataConnectionStatusDetailsWrapper<Alloc>>{ *model.status } : std::nullopt }
    {
        SetModelPointers();
    }

    PFEventsDataConnectionDetailsWrapper(const PFEventsDataConnectionDetailsWrapper& src) :
        PFEventsDataConnectionDetailsWrapper{ src.Model() }
    {
    }

    PFEventsDataConnectionDetailsWrapper(PFEventsDataConnectionDetailsWrapper&& src) :
        PFEventsDataConnectionDetailsWrapper{}
    {
        swap(*this, src);
    }

    PFEventsDataConnectionDetailsWrapper& operator=(PFEventsDataConnectionDetailsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsDataConnectionDetailsWrapper() = default;

    friend void swap(PFEventsDataConnectionDetailsWrapper& lhs, PFEventsDataConnectionDetailsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_connectionSettings, rhs.m_connectionSettings);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_status, rhs.m_status);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    PFEventsDataConnectionSettingsWrapper<Alloc> const& GetConnectionSettings() const
    {
        return m_connectionSettings;
    }

    void SetConnectionSettings(PFEventsDataConnectionSettingsWrapper<Alloc> value)
    {
        m_connectionSettings = std::move(value);
        this->m_model.connectionSettings = &m_connectionSettings.Model();
    }

    bool GetIsActive() const
    {
        return this->m_model.isActive;
    }

    void SetIsActive(bool value)
    {
        this->m_model.isActive = value;
    }

    String const& GetName() const
    {
        return m_name;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    std::optional<PFEventsDataConnectionStatusDetailsWrapper<Alloc>> const& GetStatus() const
    {
        return m_status;
    }

    void SetStatus(std::optional<PFEventsDataConnectionStatusDetailsWrapper<Alloc>> value)
    {
        m_status = std::move(value);
        this->m_model.status = m_status ? &m_status->Model() : nullptr;
    }

    PFEventsDataConnectionType GetType() const
    {
        return this->m_model.type;
    }

    void SetType(PFEventsDataConnectionType value)
    {
        this->m_model.type = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.connectionSettings = &m_connectionSettings.Model();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.status = m_status ?  &m_status->Model() : nullptr;
    }

    PFEventsDataConnectionSettingsWrapper<Alloc> m_connectionSettings;
    String m_name;
    std::optional<PFEventsDataConnectionStatusDetailsWrapper<Alloc>> m_status;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsGetDataConnectionResponseWrapper : public ModelWrapper<PFEventsGetDataConnectionResponse, Alloc>
{
public:
    using ModelType = PFEventsGetDataConnectionResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsGetDataConnectionResponseWrapper() = default;

    PFEventsGetDataConnectionResponseWrapper(const PFEventsGetDataConnectionResponse& model) :
        ModelWrapper<PFEventsGetDataConnectionResponse, Alloc>{ model },
        m_dataConnection{ model.dataConnection ? std::optional<PFEventsDataConnectionDetailsWrapper<Alloc>>{ *model.dataConnection } : std::nullopt }
    {
        SetModelPointers();
    }

    PFEventsGetDataConnectionResponseWrapper(const PFEventsGetDataConnectionResponseWrapper& src) :
        PFEventsGetDataConnectionResponseWrapper{ src.Model() }
    {
    }

    PFEventsGetDataConnectionResponseWrapper(PFEventsGetDataConnectionResponseWrapper&& src) :
        PFEventsGetDataConnectionResponseWrapper{}
    {
        swap(*this, src);
    }

    PFEventsGetDataConnectionResponseWrapper& operator=(PFEventsGetDataConnectionResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsGetDataConnectionResponseWrapper() = default;

    friend void swap(PFEventsGetDataConnectionResponseWrapper& lhs, PFEventsGetDataConnectionResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_dataConnection, rhs.m_dataConnection);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    std::optional<PFEventsDataConnectionDetailsWrapper<Alloc>> const& GetDataConnection() const
    {
        return m_dataConnection;
    }

    void SetDataConnection(std::optional<PFEventsDataConnectionDetailsWrapper<Alloc>> value)
    {
        m_dataConnection = std::move(value);
        this->m_model.dataConnection = m_dataConnection ? &m_dataConnection->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.dataConnection = m_dataConnection ?  &m_dataConnection->Model() : nullptr;
    }

    std::optional<PFEventsDataConnectionDetailsWrapper<Alloc>> m_dataConnection;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsListDataConnectionsRequestWrapper : public ModelWrapper<PFEventsListDataConnectionsRequest, Alloc>
{
public:
    using ModelType = PFEventsListDataConnectionsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsListDataConnectionsRequestWrapper() = default;

    PFEventsListDataConnectionsRequestWrapper(const PFEventsListDataConnectionsRequest& model) :
        ModelWrapper<PFEventsListDataConnectionsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFEventsListDataConnectionsRequestWrapper(const PFEventsListDataConnectionsRequestWrapper& src) :
        PFEventsListDataConnectionsRequestWrapper{ src.Model() }
    {
    }

    PFEventsListDataConnectionsRequestWrapper(PFEventsListDataConnectionsRequestWrapper&& src) :
        PFEventsListDataConnectionsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFEventsListDataConnectionsRequestWrapper& operator=(PFEventsListDataConnectionsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsListDataConnectionsRequestWrapper() = default;

    friend void swap(PFEventsListDataConnectionsRequestWrapper& lhs, PFEventsListDataConnectionsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
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

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsListDataConnectionsResponseWrapper : public ModelWrapper<PFEventsListDataConnectionsResponse, Alloc>
{
public:
    using ModelType = PFEventsListDataConnectionsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsListDataConnectionsResponseWrapper() = default;

    PFEventsListDataConnectionsResponseWrapper(const PFEventsListDataConnectionsResponse& model) :
        ModelWrapper<PFEventsListDataConnectionsResponse, Alloc>{ model },
        m_dataConnections{ model.dataConnections, model.dataConnections + model.dataConnectionsCount }
    {
        SetModelPointers();
    }

    PFEventsListDataConnectionsResponseWrapper(const PFEventsListDataConnectionsResponseWrapper& src) :
        PFEventsListDataConnectionsResponseWrapper{ src.Model() }
    {
    }

    PFEventsListDataConnectionsResponseWrapper(PFEventsListDataConnectionsResponseWrapper&& src) :
        PFEventsListDataConnectionsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFEventsListDataConnectionsResponseWrapper& operator=(PFEventsListDataConnectionsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsListDataConnectionsResponseWrapper() = default;

    friend void swap(PFEventsListDataConnectionsResponseWrapper& lhs, PFEventsListDataConnectionsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_dataConnections, rhs.m_dataConnections);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    ModelVector<PFEventsDataConnectionDetailsWrapper<Alloc>, Alloc> const& GetDataConnections() const
    {
        return m_dataConnections;
    }

    void SetDataConnections(ModelVector<PFEventsDataConnectionDetailsWrapper<Alloc>, Alloc> value)
    {
        m_dataConnections = std::move(value);
        this->m_model.dataConnections =  m_dataConnections.empty() ? nullptr : m_dataConnections.data();
        this->m_model.dataConnectionsCount =  static_cast<uint32_t>(m_dataConnections.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.dataConnections = m_dataConnections.empty() ? nullptr : m_dataConnections.data();
    }

    ModelVector<PFEventsDataConnectionDetailsWrapper<Alloc>, Alloc> m_dataConnections;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsSetDataConnectionRequestWrapper : public ModelWrapper<PFEventsSetDataConnectionRequest, Alloc>
{
public:
    using ModelType = PFEventsSetDataConnectionRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsSetDataConnectionRequestWrapper() = default;

    PFEventsSetDataConnectionRequestWrapper(const PFEventsSetDataConnectionRequest& model) :
        ModelWrapper<PFEventsSetDataConnectionRequest, Alloc>{ model },
        m_connectionSettings{ model.connectionSettings ? *model.connectionSettings : decltype(*model.connectionSettings){} },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFEventsSetDataConnectionRequestWrapper(const PFEventsSetDataConnectionRequestWrapper& src) :
        PFEventsSetDataConnectionRequestWrapper{ src.Model() }
    {
    }

    PFEventsSetDataConnectionRequestWrapper(PFEventsSetDataConnectionRequestWrapper&& src) :
        PFEventsSetDataConnectionRequestWrapper{}
    {
        swap(*this, src);
    }

    PFEventsSetDataConnectionRequestWrapper& operator=(PFEventsSetDataConnectionRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsSetDataConnectionRequestWrapper() = default;

    friend void swap(PFEventsSetDataConnectionRequestWrapper& lhs, PFEventsSetDataConnectionRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_connectionSettings, rhs.m_connectionSettings);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_name, rhs.m_name);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    PFEventsDataConnectionSettingsWrapper<Alloc> const& GetConnectionSettings() const
    {
        return m_connectionSettings;
    }

    void SetConnectionSettings(PFEventsDataConnectionSettingsWrapper<Alloc> value)
    {
        m_connectionSettings = std::move(value);
        this->m_model.connectionSettings = &m_connectionSettings.Model();
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

    bool GetIsActive() const
    {
        return this->m_model.isActive;
    }

    void SetIsActive(bool value)
    {
        this->m_model.isActive = value;
    }

    String const& GetName() const
    {
        return m_name;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    PFEventsDataConnectionType GetType() const
    {
        return this->m_model.type;
    }

    void SetType(PFEventsDataConnectionType value)
    {
        this->m_model.type = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.connectionSettings = &m_connectionSettings.Model();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    PFEventsDataConnectionSettingsWrapper<Alloc> m_connectionSettings;
    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsSetDataConnectionResponseWrapper : public ModelWrapper<PFEventsSetDataConnectionResponse, Alloc>
{
public:
    using ModelType = PFEventsSetDataConnectionResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsSetDataConnectionResponseWrapper() = default;

    PFEventsSetDataConnectionResponseWrapper(const PFEventsSetDataConnectionResponse& model) :
        ModelWrapper<PFEventsSetDataConnectionResponse, Alloc>{ model },
        m_dataConnection{ model.dataConnection ? std::optional<PFEventsDataConnectionDetailsWrapper<Alloc>>{ *model.dataConnection } : std::nullopt }
    {
        SetModelPointers();
    }

    PFEventsSetDataConnectionResponseWrapper(const PFEventsSetDataConnectionResponseWrapper& src) :
        PFEventsSetDataConnectionResponseWrapper{ src.Model() }
    {
    }

    PFEventsSetDataConnectionResponseWrapper(PFEventsSetDataConnectionResponseWrapper&& src) :
        PFEventsSetDataConnectionResponseWrapper{}
    {
        swap(*this, src);
    }

    PFEventsSetDataConnectionResponseWrapper& operator=(PFEventsSetDataConnectionResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsSetDataConnectionResponseWrapper() = default;

    friend void swap(PFEventsSetDataConnectionResponseWrapper& lhs, PFEventsSetDataConnectionResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_dataConnection, rhs.m_dataConnection);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    std::optional<PFEventsDataConnectionDetailsWrapper<Alloc>> const& GetDataConnection() const
    {
        return m_dataConnection;
    }

    void SetDataConnection(std::optional<PFEventsDataConnectionDetailsWrapper<Alloc>> value)
    {
        m_dataConnection = std::move(value);
        this->m_model.dataConnection = m_dataConnection ? &m_dataConnection->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.dataConnection = m_dataConnection ?  &m_dataConnection->Model() : nullptr;
    }

    std::optional<PFEventsDataConnectionDetailsWrapper<Alloc>> m_dataConnection;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsSetDataConnectionActiveRequestWrapper : public ModelWrapper<PFEventsSetDataConnectionActiveRequest, Alloc>
{
public:
    using ModelType = PFEventsSetDataConnectionActiveRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsSetDataConnectionActiveRequestWrapper() = default;

    PFEventsSetDataConnectionActiveRequestWrapper(const PFEventsSetDataConnectionActiveRequest& model) :
        ModelWrapper<PFEventsSetDataConnectionActiveRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFEventsSetDataConnectionActiveRequestWrapper(const PFEventsSetDataConnectionActiveRequestWrapper& src) :
        PFEventsSetDataConnectionActiveRequestWrapper{ src.Model() }
    {
    }

    PFEventsSetDataConnectionActiveRequestWrapper(PFEventsSetDataConnectionActiveRequestWrapper&& src) :
        PFEventsSetDataConnectionActiveRequestWrapper{}
    {
        swap(*this, src);
    }

    PFEventsSetDataConnectionActiveRequestWrapper& operator=(PFEventsSetDataConnectionActiveRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsSetDataConnectionActiveRequestWrapper() = default;

    friend void swap(PFEventsSetDataConnectionActiveRequestWrapper& lhs, PFEventsSetDataConnectionActiveRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_name, rhs.m_name);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    bool GetActive() const
    {
        return this->m_model.active;
    }

    void SetActive(bool value)
    {
        this->m_model.active = value;
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

    String const& GetName() const
    {
        return m_name;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsSetDataConnectionActiveResponseWrapper : public ModelWrapper<PFEventsSetDataConnectionActiveResponse, Alloc>
{
public:
    using ModelType = PFEventsSetDataConnectionActiveResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsSetDataConnectionActiveResponseWrapper() = default;

    PFEventsSetDataConnectionActiveResponseWrapper(const PFEventsSetDataConnectionActiveResponse& model) :
        ModelWrapper<PFEventsSetDataConnectionActiveResponse, Alloc>{ model },
        m_dataConnection{ model.dataConnection ? std::optional<PFEventsDataConnectionDetailsWrapper<Alloc>>{ *model.dataConnection } : std::nullopt }
    {
        SetModelPointers();
    }

    PFEventsSetDataConnectionActiveResponseWrapper(const PFEventsSetDataConnectionActiveResponseWrapper& src) :
        PFEventsSetDataConnectionActiveResponseWrapper{ src.Model() }
    {
    }

    PFEventsSetDataConnectionActiveResponseWrapper(PFEventsSetDataConnectionActiveResponseWrapper&& src) :
        PFEventsSetDataConnectionActiveResponseWrapper{}
    {
        swap(*this, src);
    }

    PFEventsSetDataConnectionActiveResponseWrapper& operator=(PFEventsSetDataConnectionActiveResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsSetDataConnectionActiveResponseWrapper() = default;

    friend void swap(PFEventsSetDataConnectionActiveResponseWrapper& lhs, PFEventsSetDataConnectionActiveResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_dataConnection, rhs.m_dataConnection);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    std::optional<PFEventsDataConnectionDetailsWrapper<Alloc>> const& GetDataConnection() const
    {
        return m_dataConnection;
    }

    void SetDataConnection(std::optional<PFEventsDataConnectionDetailsWrapper<Alloc>> value)
    {
        m_dataConnection = std::move(value);
        this->m_model.dataConnection = m_dataConnection ? &m_dataConnection->Model() : nullptr;
    }

    bool GetWasUpdated() const
    {
        return this->m_model.wasUpdated;
    }

    void SetWasUpdated(bool value)
    {
        this->m_model.wasUpdated = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.dataConnection = m_dataConnection ?  &m_dataConnection->Model() : nullptr;
    }

    std::optional<PFEventsDataConnectionDetailsWrapper<Alloc>> m_dataConnection;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsEventContentsWrapper : public ModelWrapper<PFEventsEventContents, Alloc>
{
public:
    using ModelType = PFEventsEventContents;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsEventContentsWrapper() = default;

    PFEventsEventContentsWrapper(const PFEventsEventContents& model) :
        ModelWrapper<PFEventsEventContents, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_eventNamespace{ SafeString(model.eventNamespace) },
        m_name{ SafeString(model.name) },
        m_originalId{ SafeString(model.originalId) },
        m_originalTimestamp{ model.originalTimestamp ? std::optional<time_t>{ *model.originalTimestamp } : std::nullopt },
        m_payload{ model.payload },
        m_payloadJSON{ SafeString(model.payloadJSON) }
    {
        SetModelPointers();
    }

    PFEventsEventContentsWrapper(const PFEventsEventContentsWrapper& src) :
        PFEventsEventContentsWrapper{ src.Model() }
    {
    }

    PFEventsEventContentsWrapper(PFEventsEventContentsWrapper&& src) :
        PFEventsEventContentsWrapper{}
    {
        swap(*this, src);
    }

    PFEventsEventContentsWrapper& operator=(PFEventsEventContentsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsEventContentsWrapper() = default;

    friend void swap(PFEventsEventContentsWrapper& lhs, PFEventsEventContentsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_eventNamespace, rhs.m_eventNamespace);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_originalId, rhs.m_originalId);
        swap(lhs.m_originalTimestamp, rhs.m_originalTimestamp);
        swap(lhs.m_payload, rhs.m_payload);
        swap(lhs.m_payloadJSON, rhs.m_payloadJSON);
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

    String const& GetEventNamespace() const
    {
        return m_eventNamespace;
    }

    void SetEventNamespace(String value)
    {
        m_eventNamespace = std::move(value);
        this->m_model.eventNamespace =  m_eventNamespace.empty() ? nullptr : m_eventNamespace.data();
    }

    String const& GetName() const
    {
        return m_name;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    String const& GetOriginalId() const
    {
        return m_originalId;
    }

    void SetOriginalId(String value)
    {
        m_originalId = std::move(value);
        this->m_model.originalId =  m_originalId.empty() ? nullptr : m_originalId.data();
    }

    std::optional<time_t> const& GetOriginalTimestamp() const
    {
        return m_originalTimestamp;
    }

    void SetOriginalTimestamp(std::optional<time_t> value)
    {
        m_originalTimestamp = std::move(value);
        this->m_model.originalTimestamp = m_originalTimestamp ? m_originalTimestamp.operator->() : nullptr;
    }

    JsonObject<Alloc> const& GetPayload() const
    {
        return m_payload;
    }

    void SetPayload(JsonObject<Alloc> value)
    {
        m_payload = std::move(value);
        this->m_model.payload.stringValue = m_payload.stringValue.empty() ? nullptr : m_payload.stringValue.data();
    }

    String const& GetPayloadJSON() const
    {
        return m_payloadJSON;
    }

    void SetPayloadJSON(String value)
    {
        m_payloadJSON = std::move(value);
        this->m_model.payloadJSON =  m_payloadJSON.empty() ? nullptr : m_payloadJSON.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.eventNamespace = m_eventNamespace.empty() ? nullptr : m_eventNamespace.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.originalId = m_originalId.empty() ? nullptr : m_originalId.data();
        this->m_model.originalTimestamp = m_originalTimestamp ? m_originalTimestamp.operator->() : nullptr;
        this->m_model.payload.stringValue = m_payload.stringValue.empty() ? nullptr : m_payload.stringValue.data();
        this->m_model.payloadJSON = m_payloadJSON.empty() ? nullptr : m_payloadJSON.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_eventNamespace;
    String m_name;
    String m_originalId;
    std::optional<time_t> m_originalTimestamp;
    JsonObject<Alloc> m_payload;
    String m_payloadJSON;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsWriteEventsRequestWrapper : public ModelWrapper<PFEventsWriteEventsRequest, Alloc>
{
public:
    using ModelType = PFEventsWriteEventsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsWriteEventsRequestWrapper() = default;

    PFEventsWriteEventsRequestWrapper(const PFEventsWriteEventsRequest& model) :
        ModelWrapper<PFEventsWriteEventsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_events{ model.events, model.events + model.eventsCount }
    {
        SetModelPointers();
    }

    PFEventsWriteEventsRequestWrapper(const PFEventsWriteEventsRequestWrapper& src) :
        PFEventsWriteEventsRequestWrapper{ src.Model() }
    {
    }

    PFEventsWriteEventsRequestWrapper(PFEventsWriteEventsRequestWrapper&& src) :
        PFEventsWriteEventsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFEventsWriteEventsRequestWrapper& operator=(PFEventsWriteEventsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsWriteEventsRequestWrapper() = default;

    friend void swap(PFEventsWriteEventsRequestWrapper& lhs, PFEventsWriteEventsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_events, rhs.m_events);
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

    ModelVector<PFEventsEventContentsWrapper<Alloc>, Alloc> const& GetEvents() const
    {
        return m_events;
    }

    void SetEvents(ModelVector<PFEventsEventContentsWrapper<Alloc>, Alloc> value)
    {
        m_events = std::move(value);
        this->m_model.events =  m_events.empty() ? nullptr : m_events.data();
        this->m_model.eventsCount =  static_cast<uint32_t>(m_events.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.events = m_events.empty() ? nullptr : m_events.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    ModelVector<PFEventsEventContentsWrapper<Alloc>, Alloc> m_events;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsWriteEventsResponseWrapper : public ModelWrapper<PFEventsWriteEventsResponse, Alloc>
{
public:
    using ModelType = PFEventsWriteEventsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsWriteEventsResponseWrapper() = default;

    PFEventsWriteEventsResponseWrapper(const PFEventsWriteEventsResponse& model) :
        ModelWrapper<PFEventsWriteEventsResponse, Alloc>{ model },
        m_assignedEventIds{ model.assignedEventIds, model.assignedEventIds + model.assignedEventIdsCount }
    {
        SetModelPointers();
    }

    PFEventsWriteEventsResponseWrapper(const PFEventsWriteEventsResponseWrapper& src) :
        PFEventsWriteEventsResponseWrapper{ src.Model() }
    {
    }

    PFEventsWriteEventsResponseWrapper(PFEventsWriteEventsResponseWrapper&& src) :
        PFEventsWriteEventsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFEventsWriteEventsResponseWrapper& operator=(PFEventsWriteEventsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsWriteEventsResponseWrapper() = default;

    friend void swap(PFEventsWriteEventsResponseWrapper& lhs, PFEventsWriteEventsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_assignedEventIds, rhs.m_assignedEventIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    CStringVector<Alloc> const& GetAssignedEventIds() const
    {
        return m_assignedEventIds;
    }

    void SetAssignedEventIds(CStringVector<Alloc> value)
    {
        m_assignedEventIds = std::move(value);
        this->m_model.assignedEventIds =  m_assignedEventIds.empty() ? nullptr : m_assignedEventIds.data();
        this->m_model.assignedEventIdsCount =  static_cast<uint32_t>(m_assignedEventIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.assignedEventIds = m_assignedEventIds.empty() ? nullptr : m_assignedEventIds.data();
    }

    CStringVector<Alloc> m_assignedEventIds;
};

} // namespace Wrappers
} // namespace PlayFab
