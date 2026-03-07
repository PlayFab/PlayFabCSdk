#include "stdafx.h"
#include "EventsTypes.h"
#include "JsonUtils.h"

namespace PlayFab
{
namespace Events
{

JsonValue DeleteDataConnectionRequest::ToJson() const
{
    return DeleteDataConnectionRequest::ToJson(this->Model());
}

JsonValue DeleteDataConnectionRequest::ToJson(const PFEventsDeleteDataConnectionRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    return output;
}

HRESULT DeleteDataConnectionResponse::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "WasDeleted", this->m_model.wasDeleted));

    return S_OK;
}

size_t DeleteDataConnectionResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsDeleteDataConnectionResponse const*> DeleteDataConnectionResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<DeleteDataConnectionResponse>(&this->Model());
}

size_t DeleteDataConnectionResponse::RequiredBufferSize(const PFEventsDeleteDataConnectionResponse& model)
{
    UNREFERENCED_PARAMETER(model); // Fixed size
    return sizeof(ModelType);
}

HRESULT DeleteDataConnectionResponse::Copy(const PFEventsDeleteDataConnectionResponse& input, PFEventsDeleteDataConnectionResponse& output, ModelBuffer& buffer)
{
    output = input;
    UNREFERENCED_PARAMETER(buffer); // Fixed size
    return S_OK;
}

JsonValue GetDataConnectionRequest::ToJson() const
{
    return GetDataConnectionRequest::ToJson(this->Model());
}

JsonValue GetDataConnectionRequest::ToJson(const PFEventsGetDataConnectionRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    return output;
}

JsonValue DataConnectionAzureBlobSettings::ToJson() const
{
    return DataConnectionAzureBlobSettings::ToJson(this->Model());
}

JsonValue DataConnectionAzureBlobSettings::ToJson(const PFEventsDataConnectionAzureBlobSettings& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "AccountName", input.accountName);
    JsonUtils::ObjectAddMember(output, "ContainerName", input.containerName);
    JsonUtils::ObjectAddMember(output, "TenantId", input.tenantId);
    return output;
}

HRESULT DataConnectionAzureBlobSettings::FromJson(const JsonValue& input)
{
    String accountName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AccountName", accountName));
    this->SetAccountName(std::move(accountName));

    String containerName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ContainerName", containerName));
    this->SetContainerName(std::move(containerName));

    String tenantId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TenantId", tenantId));
    this->SetTenantId(std::move(tenantId));

    return S_OK;
}

size_t DataConnectionAzureBlobSettings::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsDataConnectionAzureBlobSettings const*> DataConnectionAzureBlobSettings::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<DataConnectionAzureBlobSettings>(&this->Model());
}

size_t DataConnectionAzureBlobSettings::RequiredBufferSize(const PFEventsDataConnectionAzureBlobSettings& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.accountName)
    {
        requiredSize += (std::strlen(model.accountName) + 1);
    }
    if (model.containerName)
    {
        requiredSize += (std::strlen(model.containerName) + 1);
    }
    if (model.tenantId)
    {
        requiredSize += (std::strlen(model.tenantId) + 1);
    }
    return requiredSize;
}

HRESULT DataConnectionAzureBlobSettings::Copy(const PFEventsDataConnectionAzureBlobSettings& input, PFEventsDataConnectionAzureBlobSettings& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.accountName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.accountName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.containerName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.containerName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.tenantId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.tenantId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue DataConnectionAzureDataExplorerSettings::ToJson() const
{
    return DataConnectionAzureDataExplorerSettings::ToJson(this->Model());
}

JsonValue DataConnectionAzureDataExplorerSettings::ToJson(const PFEventsDataConnectionAzureDataExplorerSettings& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "ClusterUri", input.clusterUri);
    JsonUtils::ObjectAddMember(output, "Database", input.database);
    JsonUtils::ObjectAddMember(output, "Table", input.table);
    return output;
}

HRESULT DataConnectionAzureDataExplorerSettings::FromJson(const JsonValue& input)
{
    String clusterUri{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ClusterUri", clusterUri));
    this->SetClusterUri(std::move(clusterUri));

    String database{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Database", database));
    this->SetDatabase(std::move(database));

    String table{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Table", table));
    this->SetTable(std::move(table));

    return S_OK;
}

size_t DataConnectionAzureDataExplorerSettings::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsDataConnectionAzureDataExplorerSettings const*> DataConnectionAzureDataExplorerSettings::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<DataConnectionAzureDataExplorerSettings>(&this->Model());
}

size_t DataConnectionAzureDataExplorerSettings::RequiredBufferSize(const PFEventsDataConnectionAzureDataExplorerSettings& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.clusterUri)
    {
        requiredSize += (std::strlen(model.clusterUri) + 1);
    }
    if (model.database)
    {
        requiredSize += (std::strlen(model.database) + 1);
    }
    if (model.table)
    {
        requiredSize += (std::strlen(model.table) + 1);
    }
    return requiredSize;
}

HRESULT DataConnectionAzureDataExplorerSettings::Copy(const PFEventsDataConnectionAzureDataExplorerSettings& input, PFEventsDataConnectionAzureDataExplorerSettings& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.clusterUri);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.clusterUri = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.database);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.database = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.table);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.table = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue DataConnectionFabricKQLSettings::ToJson() const
{
    return DataConnectionFabricKQLSettings::ToJson(this->Model());
}

JsonValue DataConnectionFabricKQLSettings::ToJson(const PFEventsDataConnectionFabricKQLSettings& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "ClusterUri", input.clusterUri);
    JsonUtils::ObjectAddMember(output, "Database", input.database);
    JsonUtils::ObjectAddMember(output, "Table", input.table);
    return output;
}

HRESULT DataConnectionFabricKQLSettings::FromJson(const JsonValue& input)
{
    String clusterUri{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ClusterUri", clusterUri));
    this->SetClusterUri(std::move(clusterUri));

    String database{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Database", database));
    this->SetDatabase(std::move(database));

    String table{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Table", table));
    this->SetTable(std::move(table));

    return S_OK;
}

size_t DataConnectionFabricKQLSettings::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsDataConnectionFabricKQLSettings const*> DataConnectionFabricKQLSettings::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<DataConnectionFabricKQLSettings>(&this->Model());
}

size_t DataConnectionFabricKQLSettings::RequiredBufferSize(const PFEventsDataConnectionFabricKQLSettings& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.clusterUri)
    {
        requiredSize += (std::strlen(model.clusterUri) + 1);
    }
    if (model.database)
    {
        requiredSize += (std::strlen(model.database) + 1);
    }
    if (model.table)
    {
        requiredSize += (std::strlen(model.table) + 1);
    }
    return requiredSize;
}

HRESULT DataConnectionFabricKQLSettings::Copy(const PFEventsDataConnectionFabricKQLSettings& input, PFEventsDataConnectionFabricKQLSettings& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.clusterUri);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.clusterUri = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.database);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.database = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.table);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.table = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue DataConnectionSettings::ToJson() const
{
    return DataConnectionSettings::ToJson(this->Model());
}

JsonValue DataConnectionSettings::ToJson(const PFEventsDataConnectionSettings& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<DataConnectionAzureBlobSettings>(output, "AzureBlobSettings", input.azureBlobSettings);
    JsonUtils::ObjectAddMember<DataConnectionAzureDataExplorerSettings>(output, "AzureDataExplorerSettings", input.azureDataExplorerSettings);
    JsonUtils::ObjectAddMember<DataConnectionFabricKQLSettings>(output, "AzureFabricKQLSettings", input.azureFabricKQLSettings);
    return output;
}

HRESULT DataConnectionSettings::FromJson(const JsonValue& input)
{
    std::optional<DataConnectionAzureBlobSettings> azureBlobSettings{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AzureBlobSettings", azureBlobSettings));
    if (azureBlobSettings)
    {
        this->SetAzureBlobSettings(std::move(*azureBlobSettings));
    }

    std::optional<DataConnectionAzureDataExplorerSettings> azureDataExplorerSettings{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AzureDataExplorerSettings", azureDataExplorerSettings));
    if (azureDataExplorerSettings)
    {
        this->SetAzureDataExplorerSettings(std::move(*azureDataExplorerSettings));
    }

    std::optional<DataConnectionFabricKQLSettings> azureFabricKQLSettings{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AzureFabricKQLSettings", azureFabricKQLSettings));
    if (azureFabricKQLSettings)
    {
        this->SetAzureFabricKQLSettings(std::move(*azureFabricKQLSettings));
    }

    return S_OK;
}

size_t DataConnectionSettings::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsDataConnectionSettings const*> DataConnectionSettings::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<DataConnectionSettings>(&this->Model());
}

size_t DataConnectionSettings::RequiredBufferSize(const PFEventsDataConnectionSettings& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.azureBlobSettings)
    {
        requiredSize += DataConnectionAzureBlobSettings::RequiredBufferSize(*model.azureBlobSettings);
    }
    if (model.azureDataExplorerSettings)
    {
        requiredSize += DataConnectionAzureDataExplorerSettings::RequiredBufferSize(*model.azureDataExplorerSettings);
    }
    if (model.azureFabricKQLSettings)
    {
        requiredSize += DataConnectionFabricKQLSettings::RequiredBufferSize(*model.azureFabricKQLSettings);
    }
    return requiredSize;
}

HRESULT DataConnectionSettings::Copy(const PFEventsDataConnectionSettings& input, PFEventsDataConnectionSettings& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<DataConnectionAzureBlobSettings>(input.azureBlobSettings);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.azureBlobSettings = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<DataConnectionAzureDataExplorerSettings>(input.azureDataExplorerSettings);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.azureDataExplorerSettings = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<DataConnectionFabricKQLSettings>(input.azureFabricKQLSettings);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.azureFabricKQLSettings = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT DataConnectionStatusDetails::FromJson(const JsonValue& input)
{
    String error{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Error", error));
    this->SetError(std::move(error));

    String errorMessage{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ErrorMessage", errorMessage));
    this->SetErrorMessage(std::move(errorMessage));

    std::optional<time_t> mostRecentErrorTime{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "MostRecentErrorTime", mostRecentErrorTime));
    this->SetMostRecentErrorTime(std::move(mostRecentErrorTime));

    std::optional<PFEventsDataConnectionErrorState> state{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "State", state));
    this->SetState(std::move(state));

    return S_OK;
}

size_t DataConnectionStatusDetails::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsDataConnectionStatusDetails const*> DataConnectionStatusDetails::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<DataConnectionStatusDetails>(&this->Model());
}

size_t DataConnectionStatusDetails::RequiredBufferSize(const PFEventsDataConnectionStatusDetails& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.error)
    {
        requiredSize += (std::strlen(model.error) + 1);
    }
    if (model.errorMessage)
    {
        requiredSize += (std::strlen(model.errorMessage) + 1);
    }
    if (model.mostRecentErrorTime)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    if (model.state)
    {
        requiredSize += (alignof(PFEventsDataConnectionErrorState) + sizeof(PFEventsDataConnectionErrorState));
    }
    return requiredSize;
}

HRESULT DataConnectionStatusDetails::Copy(const PFEventsDataConnectionStatusDetails& input, PFEventsDataConnectionStatusDetails& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.error);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.error = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.errorMessage);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.errorMessage = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.mostRecentErrorTime);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.mostRecentErrorTime = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.state);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.state = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT DataConnectionDetails::FromJson(const JsonValue& input)
{
    DataConnectionSettings connectionSettings{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ConnectionSettings", connectionSettings));
    this->SetConnectionSettings(std::move(connectionSettings));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IsActive", this->m_model.isActive));

    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    std::optional<DataConnectionStatusDetails> status{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Status", status));
    if (status)
    {
        this->SetStatus(std::move(*status));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Type", this->m_model.type));

    return S_OK;
}

size_t DataConnectionDetails::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsDataConnectionDetails const*> DataConnectionDetails::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<DataConnectionDetails>(&this->Model());
}

size_t DataConnectionDetails::RequiredBufferSize(const PFEventsDataConnectionDetails& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.connectionSettings)
    {
        requiredSize += DataConnectionSettings::RequiredBufferSize(*model.connectionSettings);
    }
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    if (model.status)
    {
        requiredSize += DataConnectionStatusDetails::RequiredBufferSize(*model.status);
    }
    return requiredSize;
}

HRESULT DataConnectionDetails::Copy(const PFEventsDataConnectionDetails& input, PFEventsDataConnectionDetails& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<DataConnectionSettings>(input.connectionSettings);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.connectionSettings = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.name);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<DataConnectionStatusDetails>(input.status);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.status = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT GetDataConnectionResponse::FromJson(const JsonValue& input)
{
    std::optional<DataConnectionDetails> dataConnection{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DataConnection", dataConnection));
    if (dataConnection)
    {
        this->SetDataConnection(std::move(*dataConnection));
    }

    return S_OK;
}

size_t GetDataConnectionResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsGetDataConnectionResponse const*> GetDataConnectionResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetDataConnectionResponse>(&this->Model());
}

size_t GetDataConnectionResponse::RequiredBufferSize(const PFEventsGetDataConnectionResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.dataConnection)
    {
        requiredSize += DataConnectionDetails::RequiredBufferSize(*model.dataConnection);
    }
    return requiredSize;
}

HRESULT GetDataConnectionResponse::Copy(const PFEventsGetDataConnectionResponse& input, PFEventsGetDataConnectionResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<DataConnectionDetails>(input.dataConnection);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.dataConnection = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ListDataConnectionsRequest::ToJson() const
{
    return ListDataConnectionsRequest::ToJson(this->Model());
}

JsonValue ListDataConnectionsRequest::ToJson(const PFEventsListDataConnectionsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    return output;
}

HRESULT ListDataConnectionsResponse::FromJson(const JsonValue& input)
{
    ModelVector<DataConnectionDetails> dataConnections{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<DataConnectionDetails>(input, "DataConnections", dataConnections));
    this->SetDataConnections(std::move(dataConnections));

    return S_OK;
}

size_t ListDataConnectionsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsListDataConnectionsResponse const*> ListDataConnectionsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ListDataConnectionsResponse>(&this->Model());
}

size_t ListDataConnectionsResponse::RequiredBufferSize(const PFEventsListDataConnectionsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFEventsDataConnectionDetails*) + sizeof(PFEventsDataConnectionDetails*) * model.dataConnectionsCount);
    for (size_t i = 0; i < model.dataConnectionsCount; ++i)
    {
        requiredSize += DataConnectionDetails::RequiredBufferSize(*model.dataConnections[i]);
    }
    return requiredSize;
}

HRESULT ListDataConnectionsResponse::Copy(const PFEventsListDataConnectionsResponse& input, PFEventsListDataConnectionsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<DataConnectionDetails>(input.dataConnections, input.dataConnectionsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.dataConnections = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue SetDataConnectionRequest::ToJson() const
{
    return SetDataConnectionRequest::ToJson(this->Model());
}

JsonValue SetDataConnectionRequest::ToJson(const PFEventsSetDataConnectionRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<DataConnectionSettings>(output, "ConnectionSettings", input.connectionSettings);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "IsActive", input.isActive);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    JsonUtils::ObjectAddMember(output, "Type", input.type);
    return output;
}

HRESULT SetDataConnectionResponse::FromJson(const JsonValue& input)
{
    std::optional<DataConnectionDetails> dataConnection{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DataConnection", dataConnection));
    if (dataConnection)
    {
        this->SetDataConnection(std::move(*dataConnection));
    }

    return S_OK;
}

size_t SetDataConnectionResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsSetDataConnectionResponse const*> SetDataConnectionResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<SetDataConnectionResponse>(&this->Model());
}

size_t SetDataConnectionResponse::RequiredBufferSize(const PFEventsSetDataConnectionResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.dataConnection)
    {
        requiredSize += DataConnectionDetails::RequiredBufferSize(*model.dataConnection);
    }
    return requiredSize;
}

HRESULT SetDataConnectionResponse::Copy(const PFEventsSetDataConnectionResponse& input, PFEventsSetDataConnectionResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<DataConnectionDetails>(input.dataConnection);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.dataConnection = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue SetDataConnectionActiveRequest::ToJson() const
{
    return SetDataConnectionActiveRequest::ToJson(this->Model());
}

JsonValue SetDataConnectionActiveRequest::ToJson(const PFEventsSetDataConnectionActiveRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "Active", input.active);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    return output;
}

HRESULT SetDataConnectionActiveResponse::FromJson(const JsonValue& input)
{
    std::optional<DataConnectionDetails> dataConnection{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DataConnection", dataConnection));
    if (dataConnection)
    {
        this->SetDataConnection(std::move(*dataConnection));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "WasUpdated", this->m_model.wasUpdated));

    return S_OK;
}

size_t SetDataConnectionActiveResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsSetDataConnectionActiveResponse const*> SetDataConnectionActiveResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<SetDataConnectionActiveResponse>(&this->Model());
}

size_t SetDataConnectionActiveResponse::RequiredBufferSize(const PFEventsSetDataConnectionActiveResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.dataConnection)
    {
        requiredSize += DataConnectionDetails::RequiredBufferSize(*model.dataConnection);
    }
    return requiredSize;
}

HRESULT SetDataConnectionActiveResponse::Copy(const PFEventsSetDataConnectionActiveResponse& input, PFEventsSetDataConnectionActiveResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<DataConnectionDetails>(input.dataConnection);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.dataConnection = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue EventContents::ToJson() const
{
    return EventContents::ToJson(this->Model());
}

JsonValue EventContents::ToJson(const PFEventsEventContents& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "EventNamespace", input.eventNamespace);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    JsonUtils::ObjectAddMember(output, "OriginalId", input.originalId);
    JsonUtils::ObjectAddMemberTime(output, "OriginalTimestamp", input.originalTimestamp);
    JsonUtils::ObjectAddMember(output, "Payload", input.payload);
    JsonUtils::ObjectAddMember(output, "PayloadJSON", input.payloadJSON);
    return output;
}

JsonValue WriteEventsRequest::ToJson() const
{
    return WriteEventsRequest::ToJson(this->Model());
}

JsonValue WriteEventsRequest::ToJson(const PFEventsWriteEventsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMemberArray<EventContents>(output, "Events", input.events, input.eventsCount);
    return output;
}

HRESULT WriteEventsResponse::FromJson(const JsonValue& input)
{
    CStringVector assignedEventIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AssignedEventIds", assignedEventIds));
    this->SetAssignedEventIds(std::move(assignedEventIds));

    return S_OK;
}

size_t WriteEventsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsWriteEventsResponse const*> WriteEventsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<WriteEventsResponse>(&this->Model());
}

size_t WriteEventsResponse::RequiredBufferSize(const PFEventsWriteEventsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(char*) + sizeof(char*) * model.assignedEventIdsCount);
    for (size_t i = 0; i < model.assignedEventIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.assignedEventIds[i]) + 1);
    }
    return requiredSize;
}

HRESULT WriteEventsResponse::Copy(const PFEventsWriteEventsResponse& input, PFEventsWriteEventsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray(input.assignedEventIds, input.assignedEventIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.assignedEventIds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

} // namespace Events
} // namespace PlayFab
