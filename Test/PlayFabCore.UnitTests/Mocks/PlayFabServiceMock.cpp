#include "stdafx.h"
#include "PlayFabServiceMock.h"

namespace PlayFab
{
namespace UnitTests
{

PlayFabServiceMock::~PlayFabServiceMock()
{
    // avoid race between destruction and callback invocation
    std::unique_lock<std::mutex> lock{ m_mutex };
}

JsonValue& PlayFabServiceMock::ResponseBody() noexcept
{
    return m_responseBody;
}

JsonValue& PlayFabServiceMock::ResponseBodyPayload() noexcept
{
    return m_responseBody["data"];
}

void PlayFabServiceMock::SetCallback(Callback callback) noexcept
{
    std::unique_lock<std::mutex> lock{ m_mutex };
    m_callback = std::move(callback);
}

JsonValue const& PlayFabServiceMock::ServiceResponses() const noexcept
{
    return m_responses;
}

JsonValue const& PlayFabServiceMock::ServiceErrors() noexcept
{
    return s_serviceResponses["errors"];
}

void PlayFabServiceMock::HttpMockCallback(HttpMock const& /*mock*/, std::string url, std::string requestBody, uint32_t hitCount) noexcept
{
    std::unique_lock<std::mutex> lock{ m_mutex };
    if (m_callback)
    {
        m_callback(*this, std::move(url), std::move(requestBody), hitCount);
    }
    HttpMock::SetResponseBody(m_responseBody);
}

std::string PlayFabServiceMock::GetUrl(const char* apiEndpoint, const char* apiName) noexcept
{
    std::stringstream url;
    url << apiEndpoint;

    assert(s_serviceResponses["calls"].contains(apiName));
    assert(s_serviceResponses["calls"][apiName].contains("url"));
    url << s_serviceResponses["calls"][apiName]["url"].get<String>();

    return url.str();
}

JsonValue const& PlayFabServiceMock::GetServiceResponses(const char* apiName) noexcept
{
    assert(s_serviceResponses["calls"][apiName].contains("result"));
    const char* resultTypeName = s_serviceResponses["calls"][apiName]["result"].get<String>().c_str();
    assert(s_serviceResponses["datatypes"].contains(resultTypeName));

    return s_serviceResponses["datatypes"][resultTypeName];
}


// TODO doc location, schema, etc of MockResponses file
// TODO we could get fancy here and use sample responses from service API specs but I've just manually populated a separate
// MockResponses.json file for the time being.
JsonValue InitServiceResponses()
{
    constexpr char kMockResponseJsonFile[] = "MockResponses.json";

    JsonValue serviceResponses;

    std::ifstream file{ kMockResponseJsonFile, std::ios::binary | std::ios::ate };
    assert(file.is_open());

    std::vector<char> data(static_cast<size_t>(file.tellg()));
    file.seekg(0);
    file.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(data.size()));
    assert(file.good());
    bool parseError = false;
    String parseErrorMsg;

    try
    {
        if (data.data())
        {
            serviceResponses = JsonValue::parse(data.data());
        }
        else
        {
            parseError = true;
        }
    }
    catch (const JsonValue::parse_error& e)
    {
        parseErrorMsg = e.what();
        parseError = true;
    }

    // Basic validation. Additional validation for specific mocks done during PlayFabServiceMock construction
    assert(!parseError);
    assert(serviceResponses.contains("calls"));
    assert(serviceResponses.contains("datatypes"));
    assert(serviceResponses.contains("errors"));

    return serviceResponses;
}

JsonValue PlayFabServiceMock::s_serviceResponses = InitServiceResponses();

}
}
