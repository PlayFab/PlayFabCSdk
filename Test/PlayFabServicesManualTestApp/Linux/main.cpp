#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <rapidjson/document.h>

#include <playfab/services/PFServices.h>
#include <playfab/httpclient/PFHCTrace.h>

using namespace std;

void CALLBACK MyTraceCallback(
    _In_z_ char const* areaName,
    enum PFHCTraceLevel level,
    uint64_t threadId,
    uint64_t timestamp,
    _In_z_ char const* message)
{
    cout << areaName << " " << message << endl;
}

struct TestTitleData
{
    std::string titleId;
    std::string secretKey;
    std::string connectionString;
};


std::vector<char> readFileIntoVector(const std::string &filename) 
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;        
        exit(EXIT_FAILURE);
    }

    std::streampos fileSize = file.tellg();
    if (fileSize > 10000) {
        std::cerr << "File too large" << std::endl;
        exit(EXIT_FAILURE);
    }

    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(fileSize);
    if (!file.read(buffer.data(), fileSize)) {
        std::cerr << "Error reading file" << std::endl;
        exit(EXIT_FAILURE);
    }

    file.close();
    return buffer;
}

HRESULT GetTestTitleData(TestTitleData& testTitleData) noexcept
{
    // the testTitleData JSON file looks something like this:    
    // {
    //  "titleId": "YOUR_TITLE_ID",
    //  "secretKey": "",
    //  "connectionString": "https://YOUR_TITLE_ID.playfabapi.com",
    // }    

     auto fileData = readFileIntoVector("../../testTitleData.json");
     

    // Parse JSON string into output TestTitleData.
    rapidjson::Document titleDataJson;
    titleDataJson.Parse(fileData.data());

    if (titleDataJson.HasParseError())
    {
        return E_FAIL;
    }

    testTitleData.titleId = titleDataJson["titleId"].GetString();
    testTitleData.secretKey = titleDataJson["secretKey"].GetString();
    testTitleData.connectionString = titleDataJson["connectionString"].GetString();

    return S_OK;
}

int main()
{
    PFHCSettingsSetTraceLevel(PFHCTraceLevel::Verbose);
    PFHCTraceSetClientCallback(MyTraceCallback);

    HRESULT hr = PFServicesInitialize(nullptr); 
    cout << "PFServicesInitialize: 0x" << setw(8) << setfill('0') << hex << hr << endl;

    PFServiceConfigHandle serviceConfigHandle{ nullptr };

    TestTitleData testTitleData = {};
    hr = GetTestTitleData(testTitleData);
    cout << "GetTestTitleData: 0x" << setw(8) << setfill('0') << hex << hr << endl;

    hr = PFServiceConfigCreateHandle(
            testTitleData.connectionString.c_str(),
            testTitleData.titleId.c_str(),
            &serviceConfigHandle);
    cout << "PFServiceConfigCreateHandle: 0x" << setw(8) << setfill('0') << hex << hr << endl;

    PFAuthenticationLoginWithCustomIDRequest request{};
    request.createAccount = true;
    request.customId = "player1"; // change to per player custom ID 

    XAsyncBlock async1{};
    hr = PFAuthenticationLoginWithCustomIDAsync(serviceConfigHandle, &request, &async1); // Add your own error handling when FAILED(hr) == true
    cout << "PFAuthenticationLoginWithCustomIDAsync: 0x" << setw(8) << setfill('0') << hex << hr << endl;
    hr = XAsyncGetStatus(&async1, true); // This is doing a blocking wait for completion, but you can use the XAsyncBlock to set a callback instead for async style usage
    cout << "PFAuthenticationLoginWithCustomIDAsync XAsyncGetStatus: 0x" << setw(8) << setfill('0') << hex << hr << endl;

    if( SUCCEEDED(hr) )
    {
        std::vector<char> loginResultBuffer;
        PFAuthenticationLoginResult const* loginResult;
        size_t bufferSize;
        hr = PFAuthenticationLoginWithCustomIDGetResultSize(&async1, &bufferSize);
        cout << "PFAuthenticationLoginWithCustomIDGetResultSize: 0x" << setw(8) << setfill('0') << hex << hr << endl;
        loginResultBuffer.resize(bufferSize);

        PFEntityHandle entityHandle{ nullptr };
        hr = PFAuthenticationLoginWithCustomIDGetResult(&async1, &entityHandle, loginResultBuffer.size(), loginResultBuffer.data(), &loginResult, nullptr);
        cout << "PFAuthenticationLoginWithCustomIDGetResult: 0x" << setw(8) << setfill('0') << hex << hr << endl;

        PFEntityKey const* pEntityKey{};
        std::vector<char> entityKeyBuffer;
        size_t size{};
        hr = PFEntityGetEntityKeySize(entityHandle, &size); // Add your own error handling when FAILED(hr) == true
        cout << "PFEntityGetEntityKeySize: 0x" << setw(8) << setfill('0') << hex << hr << endl;

        entityKeyBuffer.resize(size);
        hr = PFEntityGetEntityKey(entityHandle, entityKeyBuffer.size(), entityKeyBuffer.data(), &pEntityKey, nullptr);
        cout << "PFEntityGetEntityKey: 0x" << setw(8) << setfill('0') << hex << hr << endl;

        if (SUCCEEDED(hr))
        {
            XAsyncBlock async2{};
            PFDataGetFilesRequest requestFiles{};
            requestFiles.entity = pEntityKey;

            hr = PFDataGetFilesAsync(entityHandle, &requestFiles, &async2); // Add your own error handling when FAILED(hr) == true
            cout << "PFDataGetFilesAsync: 0x" << setw(8) << setfill('0') << hex << hr << endl;
            hr = XAsyncGetStatus(&async2, true); // This is doing a blocking wait for completion, but you can use the XAsyncBlock to set a callback instead for async style usage
            cout << "PFDataGetFilesAsync XAsyncGetStatus: 0x" << setw(8) << setfill('0') << hex << hr << endl;

            size_t resultSize;
            hr = PFDataGetFilesGetResultSize(&async2, &resultSize);
            cout << "PFDataGetFilesGetResultSize: 0x" << setw(8) << setfill('0') << hex << hr << endl;

            std::vector<char> getFilesResultBuffer(resultSize);
            PFDataGetFilesResponse* getFilesResponseResult{ nullptr };
            hr = PFDataGetFilesGetResult(&async2, getFilesResultBuffer.size(), getFilesResultBuffer.data(), &getFilesResponseResult, nullptr);            
            cout << "PFDataGetFilesGetResult: 0x" << setw(8) << setfill('0') << hex << hr << endl;
        }

        if (entityHandle)
        {
            PFEntityCloseHandle(entityHandle);
            entityHandle = nullptr;
        }
    }

    if (serviceConfigHandle)
    {
        PFServiceConfigCloseHandle(serviceConfigHandle);
        serviceConfigHandle = nullptr;
    }
    
    XAsyncBlock async3{};
    hr = PFServicesUninitializeAsync(&async3); // Add your own error handling when FAILED(hr) == true
    cout << "PFServicesUninitializeAsync: 0x" << setw(8) << setfill('0') << hex << hr << endl;

    hr = XAsyncGetStatus(&async3, true); // This is doing a blocking wait for completion, but you can use the XAsyncBlock to set a callback instead for async style usage
    cout << "XAsyncGetStatus: 0x" << setw(8) << setfill('0') << hex << hr << endl;

    return 0;
}
