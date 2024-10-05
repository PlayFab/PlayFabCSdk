#include "TestAppPch.h"
#include "DataTests.h"
#include "DataOperations.h"
#include <httpClient/httpClient.h>
#include <JsonUtils.h>

namespace PlayFab
{
namespace Test
{

constexpr char kTestName[]{ "testName" };
constexpr char kTestKey[]{ "testKey" };
constexpr char kTestVal[]{ "testVal" };
const PFJsonObject kObject{ "{ \"testKey\": \"testValue\" }" };


HRESULT UploadFileSync(String url)
{
    JsonValue requestBody{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(requestBody, kTestKey, kTestVal);

    HCCallHandle callHandle{ nullptr };

    // Set up HCHttpCallHandle
    RETURN_IF_FAILED(HCHttpCallCreate(&callHandle));
    RETURN_IF_FAILED(HCHttpCallRequestSetUrl(callHandle, "PUT", url.c_str()));

    RETURN_IF_FAILED(HCHttpCallRequestSetHeader(callHandle, "Content-Type", "application/json; charset=utf-8", true));

    RETURN_IF_FAILED(HCHttpCallRequestSetRequestBodyString(callHandle, PlayFab::JsonUtils::WriteToString(requestBody).c_str()));

    XAsyncBlock asyncBlock{};
    RETURN_IF_FAILED(HCHttpCallPerformAsync(callHandle, &asyncBlock));

    RETURN_IF_FAILED(HCHttpCallCloseHandle(callHandle));

    return XAsyncGetStatus(&asyncBlock, true);
}

AsyncOp<void> DataTests::Initialize()
{
    return ServicesTestClass::Initialize();
}

AsyncOp<void> DataTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

void DataTests::TestAbortFileUploads(TestContext& tc)
{
    InitiateFileUploadsOperation::RequestType request;
    request.SetEntity(DefaultTitlePlayer().EntityKey());
    request.SetFileNames({ kTestName });
    
    InitiateFileUploadsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<InitiateFileUploadsOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(DefaultTitlePlayer().EntityKey().Model().id, result.Payload().Model().entity->id, "entity->id");
        tc.AssertEqual(1u, result.Payload().Model().uploadDetailsCount, "uploadDetailsCount");
        tc.AssertEqual(kTestName, result.Payload().Model().uploadDetails[0]->fileName, "uploadDetails[0]->fileName");

        return S_OK;
    })
    .Then([&](Result<void> result) -> AsyncOp<AbortFileUploadsOperation::ResultType>
    {
        tc.RecordResult(std::move(result));

        AbortFileUploadsOperation::RequestType request;
        request.SetEntity(DefaultTitlePlayer().EntityKey());
        request.SetFileNames({ kTestName });

        return AbortFileUploadsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<AbortFileUploadsOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
    
        tc.AssertEqual(DefaultTitlePlayer().EntityKey().Model().id, result.Payload().Model().entity->id, "entity->id");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void DataTests::TestDeleteFiles(TestContext& tc)
{
    // Already covered in TestGetFiles
    tc.EndTest(S_OK);
}

void DataTests::TestFinalizeFileUploads(TestContext& tc)
{
    // Already covered in TestGetFiles
    tc.EndTest(S_OK);
}

void DataTests::TestGetFiles(TestContext& tc)
{
#if HC_PLATFORM == HC_PLATFORM_ANDROID
    // TODO: Find out what's causing this test to hang for android on the pipeline.
    tc.Skip();
    return;
#endif
    InitiateFileUploadsOperation::RequestType request;
    request.SetEntity(DefaultTitlePlayer().EntityKey());
    request.SetFileNames({ kTestName });
    
    InitiateFileUploadsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<InitiateFileUploadsOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
    
        auto& model = result.Payload().Model();
        tc.AssertEqual(DefaultTitlePlayer().EntityKey().Model().id, model.entity->id, "entity->id");
        tc.AssertEqual(1u, model.uploadDetailsCount, "uploadDetailsCount");
        tc.AssertEqual(kTestName, model.uploadDetails[0]->fileName, "uploadDetails[0]->fileName");
        tc.AssertTrue(model.uploadDetails[0]->uploadUrl, "uploadDetails[0]->uploadUrl");

        return UploadFileSync(model.uploadDetails[0]->uploadUrl);
    })
    .Then([&](Result<void> result) -> AsyncOp<FinalizeFileUploadsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        FinalizeFileUploadsOperation::RequestType request;
        request.SetEntity(DefaultTitlePlayer().EntityKey());
        request.SetFileNames({ kTestName });

        return FinalizeFileUploadsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<FinalizeFileUploadsOperation::ResultType> result) -> AsyncOp<GetFilesOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
    
        auto& model = result.Payload().Model();
        tc.AssertEqual(DefaultTitlePlayer().EntityKey().Model().id, model.entity->id, "entity->id");
        tc.AssertEqual(1u, model.metadataCount, "metadataCount");

        GetFilesOperation::RequestType request;
        request.SetEntity(DefaultTitlePlayer().EntityKey());
        
        return GetFilesOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<GetFilesOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
    
        auto& model = result.Payload().Model();
        tc.AssertEqual(DefaultTitlePlayer().EntityKey().Model().id, model.entity->id, "entity->id");
        tc.AssertEqual(1u, model.metadataCount, "metadataCount");

        return S_OK;
    })
    .Then([&](Result<void> result) -> AsyncOp<DeleteFilesOperation::ResultType>
    {
        tc.RecordResult(std::move(result));

        // Cleanup: delete files
        DeleteFilesOperation::RequestType request;
        request.SetEntity(DefaultTitlePlayer().EntityKey());
        request.SetFileNames({ kTestName });
        
        return DeleteFilesOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<DeleteFilesOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(DefaultTitlePlayer().EntityKey().Model().id, result.Payload().Model().entity->id, "entity->id");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void DataTests::TestGetObjects(TestContext& tc)
{
    // Already covered in TestSetObjects
    tc.EndTest(S_OK);
}

void DataTests::TestInitiateFileUploads(TestContext& tc)
{
    // Already covered in TestGetFiles
    tc.EndTest(S_OK);
}

void DataTests::TestSetObjects(TestContext& tc)
{
    SetObjectsOperation::RequestType request;
    request.SetEntity(DefaultTitlePlayer().EntityKey());
    ModelVector<Wrappers::PFDataSetObjectWrapper<Allocator>> objects;
    PFDataSetObject obj{};
    obj.dataObject = kObject;
    obj.objectName = kTestName;
    objects.push_back(obj);
    request.SetObjects(objects);

    SetObjectsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<SetObjectsOperation::ResultType> result) -> AsyncOp<GetObjectsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        GetObjectsOperation::RequestType request;
        request.SetEntity(DefaultTitlePlayer().EntityKey());

        return GetObjectsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<GetObjectsOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(DefaultTitlePlayer().EntityKey().Model().id, model.entity->id, "entity->id");
        tc.AssertEqual(1u, model.objectsCount, "objectsCount");
        tc.AssertEqual(kTestName, model.objects[0].value->objectName, "objects[0].value->objectName");

        JsonDocument expected;
        expected.Parse(kObject.stringValue);
        JsonDocument actual;
        actual.Parse(model.objects[0].value->dataObject.stringValue);
        tc.AssertTrue(actual.HasMember(kTestKey), "objects[0].value->dataObject.stringValue.key");
        tc.AssertEqual(expected[kTestKey].GetString(), actual[kTestKey].GetString(), "objects[0].value->dataObject.stringValue.key");

        return S_OK;
    })
    .Then([&](Result<void> result) -> AsyncOp<SetObjectsOperation::ResultType>
    {
        tc.RecordResult(std::move(result));

        // Cleanup: remove object
        SetObjectsOperation::RequestType request;
        request.SetEntity(DefaultTitlePlayer().EntityKey());
        ModelVector<Wrappers::PFDataSetObjectWrapper<Allocator>> objects;
        bool deleteObject = true;
        PFDataSetObject obj{};
        obj.deleteObject = &deleteObject;
        obj.objectName = kTestName;
        objects.push_back(obj);
        request.SetObjects(objects);

        return SetObjectsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Finally([&](Result<SetObjectsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}

}
}
