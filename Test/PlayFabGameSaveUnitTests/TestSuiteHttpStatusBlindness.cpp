// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//
// REGRESSION TESTS for Bug 63475452.
//
// The defect (now fixed in HttpRequest.cpp):
//   HCHttpCall::GetResult never converted an HTTP status code into a failing HRESULT when
//   m_isPlayfabCall == false. The status was read only to populate telemetry (HttpResult) and was
//   never consulted. The sole failure path in that branch was ServiceResponse::FromVector, which is
//   unconditional (HttpRequest.cpp:56-61) and always returns S_OK.
//
//   Both extended-manifest blob transfers take that branch:
//     - GameSaveServiceSelector::DownloadFileFromCloudToBytes -> SetIsPlayfabCall(false) (:174)
//     - GameSaveServiceSelector::UploadFileFromStringToCloud   -> SetIsPlayfabCall(false) (:327)
//
//   Consequence: an Azure Blob 403/404/409/503 on an extended-manifest PUT or GET was reported to
//   the GameSave sync state machine as SUCCESS, carrying Azure's XML error document as the payload.
//   A failed upload therefore did not trip UploadStep's failure gate, and the manifest was finalized
//   referencing an extended-manifest blob that had never been written - leaving the player
//   permanently unable to sync.
//
// These tests fail against the unfixed code and pass with the fix. Each one states, inline, what it
// observed before the fix.

#include "pch.h"
#include "actions.h"
#include "HttpRequest.h"
#include "RunContext.h"
#include "XAsyncOperation.h"
#include "Generated/Error.h"

#include <future>

using namespace PlayFab;

namespace
{

// A stand-in for the Azure Blob SAS URL the SDK uses for extended-manifest transfers.
constexpr char kBlobUrl[]{ "https://pfgamesave.blob.core.windows.net/container/extended-7-manifest.json" };

// Verbatim shape of what Azure Blob Storage returns in the body of a 404 on a missing blob.
constexpr char kBlobNotFoundXml[]{
    "\xEF\xBB\xBF<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<Error><Code>BlobNotFound</Code>"
    "<Message>The specified blob does not exist.\n"
    "RequestId:00000000-0000-0000-0000-000000000000\n"
    "Time:2026-02-10T00:00:00.0000000Z</Message></Error>"
};

// What Azure returns when the SAS token in the upload URL has expired.
constexpr char kAuthFailedXml[]{
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    "<Error><Code>AuthenticationFailed</Code>"
    "<Message>Server failed to authenticate the request.</Message></Error>"
};

struct CallOutcome
{
    HRESULT hr{ E_UNEXPECTED };
    int httpCode{ 0 };
    std::string body;
};

// Performs a non-PlayFab HCHttpCall exactly the way GameSaveServiceSelector does for
// extended-manifest blob transfers, and reports what the SDK layer above it would observe.
CallOutcome PerformNonPlayFabCall(const char* method, const char* url, const char* requestBody)
{
    XTaskQueueHandle queue{ nullptr };
    EXPECT_EQ(S_OK, XTaskQueueCreate(
        XTaskQueueDispatchMode::ThreadPool,
        XTaskQueueDispatchMode::ThreadPool,
        &queue));

    CallOutcome outcome{};
    std::promise<void> done;
    std::future<void> doneFuture = done.get_future();

    {
        RunContext rc = RunContext::Root(queue);

        UnorderedMap<String, String> headers;
        headers["x-ms-blob-type"] = "BlockBlob";

        auto requestOp = MakeUnique<HCHttpCall>(
            method,
            url,
            headers,
            requestBody ? requestBody : "",
            rc.Derive()
        );

        // THE line under test. Mirrors GameSaveServiceSelector.cpp:174 (download) and :327 (upload).
        requestOp->SetIsPlayfabCall(false);

        RunOperation(std::move(requestOp))
        .Finally([&outcome, &done](Result<ServiceResponse> result)
        {
            outcome.hr = result.hr;

            // HttpResult telemetry is attached to failing results too (Result.h:43), so the
            // observed status is readable regardless of the HRESULT.
            if (result.httpResult.has_value())
            {
                outcome.httpCode = static_cast<int>(result.httpResult->httpCode);
            }
            if (SUCCEEDED(result.hr))
            {
                const auto& bytes = result.Payload().ResponseBody;
                outcome.body.assign(bytes.begin(), bytes.end());
            }
            done.set_value();
        });

        doneFuture.wait();
    }

    XTaskQueueTerminate(queue, true, nullptr, nullptr);
    XTaskQueueCloseHandle(queue);
    return outcome;
}

} // anonymous namespace

class TestSuiteHttpStatusBlindness : public testing::Test
{
protected:
    void SetUp() override
    {
        reset_all();
        // Brings up PF Core, which initializes libHttpClient so HCMock* is usable.
        AHS(pfgamesave_init(false));
    }

    void TearDown() override
    {
        pfgamesave_shutdown(true);
        reset_all();
    }
};

// ---------------------------------------------------------------------------
// The defect itself.
//
// An HTTP 404 carrying Azure's BlobNotFound XML must be surfaced as a failing HRESULT, and the
// XML error document must never reach the caller as file contents.
//
// BEFORE THE FIX: hr == S_OK, and outcome.body contained the 216-byte XML error document, which
// GameSave would then attempt to parse as an extended manifest.
// ---------------------------------------------------------------------------
TEST_F(TestSuiteHttpStatusBlindness, Download404IsReportedAsFailure)
{
    HttpMock mock{ "GET", kBlobUrl };
    mock.SetResponseHttpStatus(404);
    mock.SetResponseBody(kBlobNotFoundXml);

    CallOutcome outcome = PerformNonPlayFabCall("GET", kBlobUrl, nullptr);

    TEST_COUT << "hr=0x" << std::hex << outcome.hr << std::dec
              << " httpCode=" << outcome.httpCode
              << " bodyBytes=" << outcome.body.size();

    // The observed HTTP status is still reported for telemetry.
    ASSERT_EQ(404, outcome.httpCode);

    // 0x80190194. This is the HRESULT that never once appeared in 30 days of production
    // telemetry, because on this path it could not be generated.
    ASSERT_EQ(HTTP_E_STATUS_NOT_FOUND, outcome.hr)
        << "A 404 on an extended-manifest download must fail the operation.";

    // And Azure's XML error document must not be handed back as blob contents.
    ASSERT_TRUE(outcome.body.empty())
        << "A failed download must not yield a payload; got: " << outcome.body;
}

// ---------------------------------------------------------------------------
// Not specific to 404. No failing status may be laundered into S_OK.
//
// BEFORE THE FIX: every one of these returned S_OK.
// ---------------------------------------------------------------------------
TEST_F(TestSuiteHttpStatusBlindness, AllFailingHttpStatusCodesAreReportedAsFailure)
{
    // 403 = expired/invalid SAS token, 409 = lease conflict, 429/503 = throttling, 500 = server error.
    const uint32_t FAILING_HTTP_CODES[]{ 400, 403, 404, 409, 412, 429, 500, 503 };

    for (uint32_t code : FAILING_HTTP_CODES)
    {
        HttpMock mock{ "GET", kBlobUrl };
        mock.SetResponseHttpStatus(code);
        mock.SetResponseBody(kBlobNotFoundXml);

        CallOutcome outcome = PerformNonPlayFabCall("GET", kBlobUrl, nullptr);

        TEST_COUT << "status " << code << " -> hr=0x" << std::hex << outcome.hr << std::dec;

        EXPECT_EQ(HttpStatusToHR(code), outcome.hr)
            << "HTTP " << code << " must map to its corresponding failing HRESULT.";
        EXPECT_TRUE(FAILED(outcome.hr))
            << "HTTP " << code << " was laundered into a success HRESULT.";
    }
}

// ---------------------------------------------------------------------------
// A 2xx must still succeed. This is the guard against over-correcting: the fix must not
// break the normal path, which is how every healthy extended-manifest transfer completes.
// ---------------------------------------------------------------------------
TEST_F(TestSuiteHttpStatusBlindness, SuccessfulDownloadStillReturnsPayload)
{
    constexpr char kManifestJson[]{ "{\"version\":7,\"files\":[]}" };

    HttpMock mock{ "GET", kBlobUrl };
    mock.SetResponseHttpStatus(200);
    mock.SetResponseBody(kManifestJson);

    CallOutcome outcome = PerformNonPlayFabCall("GET", kBlobUrl, nullptr);

    TEST_COUT << "hr=0x" << std::hex << outcome.hr << std::dec
              << " httpCode=" << outcome.httpCode
              << " bodyBytes=" << outcome.body.size();

    ASSERT_EQ(S_OK, outcome.hr) << "A healthy 200 must not be affected by the status check.";
    ASSERT_EQ(200, outcome.httpCode);
    ASSERT_NE(std::string::npos, outcome.body.find("\"version\":7"))
        << "The response body must still be delivered intact on success.";
}

// ---------------------------------------------------------------------------
// The origin of the wedge.
//
// The PUT that writes extended-<N>-manifest.json is the same non-PlayFab call. A 403 from an
// expired SAS token must fail, so that UploadStep::UploadFileFinally's failure gate
// (UploadStep.cpp:432) trips and the state machine does NOT advance to FinalizeManifest
// (:462-468). Otherwise the service finalizes a manifest whose extended-manifest blob was
// never written - which is the unrecoverable state.
//
// BEFORE THE FIX: hr == S_OK, the gate did not trip, and finalize proceeded.
// ---------------------------------------------------------------------------
TEST_F(TestSuiteHttpStatusBlindness, ExtendedManifestUpload403IsReportedAsFailure)
{
    HttpMock mock{ "PUT", kBlobUrl };
    mock.SetResponseHttpStatus(403);
    mock.SetResponseBody(kAuthFailedXml);

    CallOutcome outcome = PerformNonPlayFabCall("PUT", kBlobUrl, "{\"version\":7,\"files\":[]}");

    TEST_COUT << "PUT hr=0x" << std::hex << outcome.hr << std::dec
              << " httpCode=" << outcome.httpCode;

    ASSERT_EQ(403, outcome.httpCode);
    ASSERT_EQ(HTTP_E_STATUS_FORBIDDEN, outcome.hr)
        << "A rejected blob PUT must be distinguishable from a successful one at this layer.";
}

// ---------------------------------------------------------------------------
// The fix already existed elsewhere in this codebase; HCHttpCall was the outlier.
//
// HCHttpFileDownloadCall::GetResult (HttpFileDownloadRequest.cpp:118-132) is structurally
// identical to the non-PlayFab branch of HCHttpCall::GetResult - same leading comment, same
// XAsyncGetStatus call, same telemetry capture - except for ONE line:
//
//     HttpFileDownloadRequest.cpp:130     RETURN_IF_FAILED(HttpStatusToHR(httpCode));
//
// HttpFileUploadRequest.cpp:148 has the same guard, and HttpArchiveUploadRequest.cpp:107 / :259
// use the alternative idiom 'SUCCEEDED(result.hr) && result.Payload().IsHttpSuccess()'.
//
// So every other blob transfer in the SDK was already protected. The two extended-manifest paths
// in GameSaveServiceSelector (:174 download, :327 upload) were the only ones that were not -
// which is precisely why the extended manifest is the file that got wedged.
//
// This test pins the helper's behavior, since the fix depends on it.
// ---------------------------------------------------------------------------
TEST_F(TestSuiteHttpStatusBlindness, HttpStatusToHRMapsStatusesCorrectly)
{
    EXPECT_TRUE(SUCCEEDED(HttpStatusToHR(200))) << "200 must remain a success.";
    EXPECT_TRUE(SUCCEEDED(HttpStatusToHR(201))) << "201 (blob created) must remain a success.";

    const uint32_t FAILING_HTTP_CODES[]{ 400, 403, 404, 409, 412, 429, 500, 503 };
    for (uint32_t code : FAILING_HTTP_CODES)
    {
        HRESULT hr = HttpStatusToHR(code);
        TEST_COUT << "HttpStatusToHR(" << code << ") = 0x" << std::hex << hr << std::dec;
        EXPECT_TRUE(FAILED(hr)) << code << " must map to a failing HRESULT.";
    }
}

// ---------------------------------------------------------------------------
// The side-by-side that originally exposed the defect.
//
// Identical inputs: same HTTP 404, same unparseable (XML) response body, same HCHttpCall class.
// The ONLY difference is the SetIsPlayfabCall(false) call.
//
// BEFORE THE FIX:  asPlayFabCall=0x80190194   asBlobCall=0x0
// AFTER THE FIX:   both fail identically.
//
// This also proves the mock is being reached, so the tests above are not vacuous.
// ---------------------------------------------------------------------------
TEST_F(TestSuiteHttpStatusBlindness, SameFailureIsFatalForBothPlayFabAndBlobCalls)
{
    constexpr char kUrl[]{ "https://pfgamesave.blob.core.windows.net/container/extended-9-manifest.json" };

    HRESULT asPlayFabCall{ E_UNEXPECTED };
    HRESULT asBlobCall{ E_UNEXPECTED };

    for (bool isPlayfabCall : { true, false })
    {
        HttpMock mock{ "GET", kUrl };
        mock.SetResponseHttpStatus(404);
        mock.SetResponseBody(kBlobNotFoundXml);

        XTaskQueueHandle queue{ nullptr };
        ASSERT_EQ(S_OK, XTaskQueueCreate(
            XTaskQueueDispatchMode::ThreadPool,
            XTaskQueueDispatchMode::ThreadPool,
            &queue));

        HRESULT observedHr{ E_UNEXPECTED };
        std::promise<void> done;
        std::future<void> doneFuture = done.get_future();

        {
            RunContext rc = RunContext::Root(queue);
            UnorderedMap<String, String> headers;
            headers["x-ms-blob-type"] = "BlockBlob";

            auto requestOp = MakeUnique<HCHttpCall>("GET", kUrl, headers, "", rc.Derive());

            if (!isPlayfabCall)
            {
                requestOp->SetIsPlayfabCall(false); // <-- the only difference
            }

            RunOperation(std::move(requestOp))
            .Finally([&observedHr, &done](Result<ServiceResponse> result)
            {
                observedHr = result.hr;
                done.set_value();
            });

            doneFuture.wait();
        }

        XTaskQueueTerminate(queue, true, nullptr, nullptr);
        XTaskQueueCloseHandle(queue);

        (isPlayfabCall ? asPlayFabCall : asBlobCall) = observedHr;
    }

    TEST_COUT << "same 404 + same XML body:  asPlayFabCall=0x" << std::hex << asPlayFabCall
              << "  asBlobCall=0x" << asBlobCall << std::dec;

    // The PlayFab branch has always had a status fallback (HttpRequest.cpp:300) and correctly fails.
    ASSERT_TRUE(FAILED(asPlayFabCall))
        << "If this is S_OK the mock isn't being hit and the other tests prove nothing.";

    // The blob branch must now fail on byte-identical input. This equality IS the fix.
    ASSERT_EQ(HTTP_E_STATUS_NOT_FOUND, asBlobCall)
        << "The non-PlayFab branch must no longer launder the 404 into S_OK.";
}
