#include "pch.h"
#include "GDK/PlatformUser_GDK.h"
#include <ResultMacros.h>
#include <XAsyncOperation.h>

namespace PlayFab
{
namespace Platform
{

User::User(XUserHandle userHandle) noexcept
    : m_user{ Wrappers::XUser::Wrap(userHandle) }
{
}

Result<User> User::Wrap(XUserHandle userHandle) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(userHandle);
    User user{ userHandle };
    RETURN_IF_FAILED(user.Initialize());
    return user;
}

Result<User> User::Duplicate(XUserHandle userHandle) noexcept
{
    XUserHandle copiedHandle;
    RETURN_IF_FAILED(XUserDuplicateHandle(userHandle, &copiedHandle));

    User copiedUser{ copiedHandle };
    RETURN_IF_FAILED(copiedUser.Initialize());

    return std::move(copiedUser);
}

class XUserAddOperation : public XAsyncOperation<XUser>
{
public:
    XUserAddOperation(XUserAddOptions options, PlayFab::RunContext rc);

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<XUser> GetResult(XAsyncBlock* async) noexcept override;

    XUserAddOptions m_options;
};

XUserAddOperation::XUserAddOperation(XUserAddOptions options, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_options{ options }
{
}

HRESULT XUserAddOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return XUserAddAsync(m_options, async);
}

Result<XUser> XUserAddOperation::GetResult(XAsyncBlock* async) noexcept
{
    XUserHandle userHandle;
    RETURN_IF_FAILED(XUserAddResult(async, &userHandle));
    return XUser::Wrap(userHandle);
}

AsyncOp<User> User::Add(
    XUserAddOptions options,
    RunContext rc
) noexcept
{
    return RunOperation(MakeUnique<XUserAddOperation>(options, rc));
}

HRESULT User::Initialize() noexcept
{
    RETURN_IF_FAILED(XUserGetId(m_user.Handle(), &m_id));
    return S_OK;
}

XUserHandle User::Handle() const noexcept
{
    return m_user.Handle();
}

uint64_t User::Id() const noexcept
{
    return m_id;
}

class XUserGetTokenAndSignatureOperation : public XAsyncOperation<TokenAndSignature>
{
public:
    XUserGetTokenAndSignatureOperation(
        XUser userHandle,
        XUserGetTokenAndSignatureOptions options,
        String httpMethod,
        String url,
        UnorderedMap<String, String> headers,
        Vector<uint8_t> requestBody,
        PlayFab::RunContext runContext
    ) noexcept;

    ~XUserGetTokenAndSignatureOperation() = default;

private:
    // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<TokenAndSignature> GetResult(XAsyncBlock* async) noexcept override;

    XUser m_user;
    XUserGetTokenAndSignatureOptions const m_options;
    String const m_httpMethod;
    String const m_url;
    UnorderedMap<String, String> const m_headers;
    Vector<uint8_t> const m_requestBody;
};

XUserGetTokenAndSignatureOperation::XUserGetTokenAndSignatureOperation(
    XUser user,
    XUserGetTokenAndSignatureOptions options,
    String httpMethod,
    String url,
    UnorderedMap<String, String> headers,
    Vector<uint8_t> requestBody,
    PlayFab::RunContext runContext
) noexcept :
    XAsyncOperation<TokenAndSignature>{ std::move(runContext) },
    m_user{ std::move(user) },
    m_options{ options },
    m_httpMethod{ std::move(httpMethod) },
    m_url{ std::move(url) },
    m_headers{ std::move(headers) },
    m_requestBody{ std::move(requestBody) }
{
}

HRESULT XUserGetTokenAndSignatureOperation::OnStarted(XAsyncBlock* async) noexcept
{
    Vector<XUserGetTokenAndSignatureHttpHeader> headers{};
    for (const auto& header : m_headers)
    {
        headers.push_back(XUserGetTokenAndSignatureHttpHeader{ header.first.data(), header.second.data() });
    }

    return XUserGetTokenAndSignatureAsync(
        m_user.Handle(),
        m_options,
        m_httpMethod.data(),
        m_url.data(),
        headers.size(),
        headers.empty() ? nullptr : headers.data(),
        m_requestBody.size(),
        m_requestBody.empty() ? nullptr : m_requestBody.data(),
        async
    );
}

Result<TokenAndSignature> XUserGetTokenAndSignatureOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t bufferSize{ 0 };
    RETURN_IF_FAILED(XUserGetTokenAndSignatureResultSize(async, &bufferSize));

    TokenAndSignature result;
    Vector<uint8_t> buffer(bufferSize, 0);
    XUserGetTokenAndSignatureData* XUserTokenSignatureData{ nullptr };
    RETURN_IF_FAILED(XUserGetTokenAndSignatureResult(async, bufferSize, buffer.data(), &XUserTokenSignatureData, nullptr));

    result.token = String{ XUserTokenSignatureData->token, XUserTokenSignatureData->tokenSize };
    result.signature = String{ XUserTokenSignatureData->signature, XUserTokenSignatureData->signatureSize };

    return result;
}

AsyncOp<TokenAndSignature> User::GetTokenAndSignature(
    RunContext runContext
) const noexcept
{
    auto duplicateResult = XUser::Duplicate(m_user.Handle());
    RETURN_IF_FAILED(duplicateResult.hr);

    auto operation = MakeUnique<XUserGetTokenAndSignatureOperation>(
        duplicateResult.ExtractPayload(),
        XUserGetTokenAndSignatureOptions::None,
        "POST",
        "https://playfabapi.com/",
        UnorderedMap<String, String>{},
        Vector<uint8_t>{},
        std::move(runContext)
    );

    return RunOperation(std::move(operation));
}

} // namespace Platform
} // namespace PlayFab
