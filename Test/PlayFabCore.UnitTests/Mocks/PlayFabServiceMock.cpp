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

rapidjson::Value& PlayFabServiceMock::ResponseBody() noexcept
{
    return m_responseBody;
}

rapidjson::Value& PlayFabServiceMock::ResponseBodyPayload() noexcept
{
    return m_responseBody["data"];
}

void PlayFabServiceMock::SetCallback(Callback callback) noexcept
{
    std::unique_lock<std::mutex> lock{ m_mutex };
    m_callback = std::move(callback);
}

rapidjson::Value const& PlayFabServiceMock::ServiceResponses() const noexcept
{
    return m_responses;
}

rapidjson::Value const& PlayFabServiceMock::ServiceErrors() noexcept
{
    return s_serviceResponses["errors"];
}

PlayFab::JsonAllocator& PlayFabServiceMock::JsonAllocator() noexcept
{
    return s_serviceResponses.GetAllocator();
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

    assert(s_serviceResponses["calls"].HasMember(apiName));
    assert(s_serviceResponses["calls"][apiName].HasMember("url"));
    url << s_serviceResponses["calls"][apiName]["url"].GetString();

    return url.str();
}

rapidjson::Value const& PlayFabServiceMock::GetServiceResponses(const char* apiName) noexcept
{
    assert(s_serviceResponses["calls"][apiName].HasMember("result"));
    const char* resultTypeName = s_serviceResponses["calls"][apiName]["result"].GetString();
    assert(s_serviceResponses["datatypes"].HasMember(resultTypeName));

    return s_serviceResponses["datatypes"][resultTypeName];
}


// TODO doc location, schema, etc of MockResponses file
// TODO we could get fancy here and use sample responses from service API specs but I've just manually populated a separate
// MockResponses.json file for the time being.
rapidjson::Document InitServiceResponses()
{
    constexpr char kMockResponseJsonFile[] = "MockResponses.json";

    rapidjson::Document serviceResponses;

    std::ifstream file{ kMockResponseJsonFile, std::ios::binary | std::ios::ate };
    assert(file.is_open());

    std::vector<char> data(static_cast<size_t>(file.tellg()));
    file.seekg(0);
    file.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(data.size()));
    assert(file.good());

    serviceResponses.Parse(data.data(), data.size());
    // Basic validation. Additional validation for specific mocks done during PlayFabServiceMock construction
    assert(!serviceResponses.HasParseError());
    assert(serviceResponses.HasMember("calls"));
    assert(serviceResponses.HasMember("datatypes"));
    assert(serviceResponses.HasMember("errors"));

    return serviceResponses;
}

rapidjson::Document PlayFabServiceMock::s_serviceResponses = InitServiceResponses();

}
}
