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


// Uploads the test payload to the pre-signed URL returned by InitiateFileUploads.
//
// This used to block on XAsyncGetStatus(&asyncBlock, true) with an unqueued XAsyncBlock. That call
// runs inside a .Then continuation - i.e. on a task-queue worker thread - so it parked a worker
// waiting on work that the same queue has to dispatch, and it had no timeout of its own. When it
// stopped making progress the test simply ran until the 150s harness timeout. The same test is
// already skipped on Android with a "hangs on the pipeline" TODO, which is the same symptom.
//
// Modelled as a normal XAsyncOperation so the PUT is chained like every other call in this file
// and no thread is blocked.
class UploadFileOperation : public XAsyncOperation<void>
{
public:
    UploadFileOperation(String url, PlayFab::RunContext rc) :
        XAsyncOperation{ std::move(rc) },
        m_url{ std::move(url) }
    {
    }

    static AsyncOp<void> Run(String url, PlayFab::RunContext rc) noexcept
    {
        return RunOperation(MakeUnique<UploadFileOperation>(std::move(url), std::move(rc)));
    }

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override
    {
        JsonValue requestBody = JsonValue::object();
        JsonUtils::ObjectAddMember(requestBody, kTestKey, kTestVal);

        HCCallHandle callHandle{ nullptr };
        RETURN_IF_FAILED(HCHttpCallCreate(&callHandle));

        // Close our handle on every exit path. HCHttpCallPerformAsync keeps its own reference for
        // the duration of the call, so releasing here does not cancel an in-flight request.
        auto closeCall = [&callHandle]()
        {
            if (callHandle)
            {
                HCHttpCallCloseHandle(callHandle);
                callHandle = nullptr;
            }
        };

        HRESULT hr = HCHttpCallRequestSetUrl(callHandle, "PUT", m_url.c_str());
        if (SUCCEEDED(hr))
        {
            hr = HCHttpCallRequestSetHeader(callHandle, "Content-Type", "application/json; charset=utf-8", true);
        }
        if (SUCCEEDED(hr))
        {
            hr = HCHttpCallRequestSetRequestBodyString(callHandle, PlayFab::JsonUtils::WriteToString(requestBody).c_str());
        }
        if (SUCCEEDED(hr))
        {
            hr = HCHttpCallPerformAsync(callHandle, async);
        }

        closeCall();
        return hr;
    }

    String m_url;
};

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
    // Hold our own reference for the lifetime of the chain rather than calling
    // DefaultTitlePlayer() from inside each continuation.
    //
    // DefaultTitlePlayer() returns Entity BY VALUE, and Entity's copy constructor is
    // THROW_IF_FAILED(PFEntityDuplicateHandle(...)). If this test times out, the harness ends it
    // and moves on but never cancels this chain; the class then tears down and
    // ServicesTestClass::Uninitialize() does m_defaultTitlePlayer.reset(). A continuation that
    // resumes after that duplicates a closed handle, throws E_PF_INVALIDHANDLE, and - because the
    // throw happens on a task-queue thread - takes the whole process down instead of failing one
    // test. Copying once here keeps the entity alive until the chain finishes.
    Entity titlePlayer = DefaultTitlePlayer();
    PlayFab::RunContext rc = RunContext();

    InitiateFileUploadsOperation::RequestType request;
    request.SetEntity(titlePlayer.EntityKey());
    request.SetFileNames({ kTestName });

    InitiateFileUploadsOperation::Run(titlePlayer, request, rc).Then([&tc, titlePlayer, rc](Result<InitiateFileUploadsOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(titlePlayer.EntityKey().Model().id, model.entity->id, "entity->id");
        tc.AssertEqual(1u, model.uploadDetailsCount, "uploadDetailsCount");
        tc.AssertEqual(kTestName, model.uploadDetails[0]->fileName, "uploadDetails[0]->fileName");
        tc.AssertTrue(model.uploadDetails[0]->uploadUrl, "uploadDetails[0]->uploadUrl");

        return UploadFileOperation::Run(model.uploadDetails[0]->uploadUrl, rc);
    })
    .Then([&tc, titlePlayer, rc](Result<void> result) -> AsyncOp<FinalizeFileUploadsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        FinalizeFileUploadsOperation::RequestType request;
        request.SetEntity(titlePlayer.EntityKey());
        request.SetFileNames({ kTestName });

        return FinalizeFileUploadsOperation::Run(titlePlayer, request, rc);
    })
    .Then([&tc, titlePlayer, rc](Result<FinalizeFileUploadsOperation::ResultType> result) -> AsyncOp<GetFilesOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(titlePlayer.EntityKey().Model().id, model.entity->id, "entity->id");
        tc.AssertEqual(1u, model.metadataCount, "metadataCount");

        GetFilesOperation::RequestType request;
        request.SetEntity(titlePlayer.EntityKey());

        return GetFilesOperation::Run(titlePlayer, request, rc);
    })
    .Then([&tc, titlePlayer](Result<GetFilesOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(titlePlayer.EntityKey().Model().id, model.entity->id, "entity->id");
        tc.AssertEqual(1u, model.metadataCount, "metadataCount");

        return S_OK;
    })
    .Then([&tc, titlePlayer, rc](Result<void> result) -> AsyncOp<DeleteFilesOperation::ResultType>
    {
        tc.RecordResult(std::move(result));

        // Cleanup: delete files
        DeleteFilesOperation::RequestType request;
        request.SetEntity(titlePlayer.EntityKey());
        request.SetFileNames({ kTestName });

        return DeleteFilesOperation::Run(titlePlayer, request, rc);
    })
    .Then([&tc, titlePlayer](Result<DeleteFilesOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(titlePlayer.EntityKey().Model().id, result.Payload().Model().entity->id, "entity->id");

        return S_OK;
    })
    .Finally([&tc](Result<void> result)
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

        JsonDocument expected = JsonValue::parse(kObject.stringValue);
        JsonDocument actual = JsonValue::parse(model.objects[0].value->dataObject.stringValue);
        tc.AssertTrue(actual.contains(kTestKey), "objects[0].value->dataObject.stringValue.key");
        tc.AssertEqual(expected[kTestKey].get<String>(), actual[kTestKey].get<String>(), "objects[0].value->dataObject.stringValue.key");

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
