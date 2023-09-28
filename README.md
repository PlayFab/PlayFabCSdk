# PlayFabCSdk
Cross-Platform C/C++ PlayFab SDK.

This SDK currently supports the following platforms:
- Windows (Win32) x64
- GDK (Xbox and Windows)

For per-platform getting started guides and a complete list of available APIs, check out the [online documentation](https://learn.microsoft.com/gaming/playfab/sdks/c).

## Requirements

- A [PlayFab developer account](https://developer.playfab.com).  
- [Visual Studio 2022](https://visualstudio.microsoft.com/) installed (to build from source).

## Project setup

### Building with binary release package

Download the most recent release of the SDK from [PlayFabCSdk releases](https://github.com/PlayFab/PlayFabCSdk/releases). The release package contains the required headers, binaries, and a PlayFabServicesSDK.Win32.props file that can imported to your project to automatically add references to them.

You can import the props file manually by editing the .vcxproj files directly or by opening the Property Manager window in Visual Studio, right clicking on the project, and choosing __Add Existing Property Sheet__. After importing PlayFabServicesSDK.Win32.props, the headers should be available in your include path and references to the PlayFabServices library and its dependecies will be automatically added.

### Building from source

This SDK depends on some external third-party open source libraries referenced as git submodules in directory `/External`. Please make sure to use `--recurse-submodules` command line parameter when cloning this git repo:
```
git clone --recurse-submodules https://github.com/PlayFab/PlayFabCSdk
```
or run the following commands to sync the content of submodules if the repo was cloned earlier without the `--recurse-submodules` parameter:

```
git submodule update --init --recursive
```
After cloning this repository and its submodules, you can build the libraries by opening the solution file PlayFab.C.vs2022.sln. That solution contains the PlayFabServices project, its dependencies, and a test app. After building, the binaries will be in a subdirectory within \Out.

#### Import Property Sheet

To add a reference to the PlayFabServices projects to your own title, import the \Build\PlayFabServices.import.props file to your project. You can do this manually by editing the .vcxproj files directly or by opening the Property Manager window in Visual Studio, right clicking on the project, and choosing __Add Existing Property Sheet__. Note that the props file differs from the PlayFabServicesSDK.Win32.props mentioned above - it adds references to the projects instead of the prebuilt binaries.

#### PlayFabServicesTestApp

The included test app __PlayFabServicesTestApp__ project will build as is, but some additional setup is required before it will run. The __\Test\testTitleData.json__ file needs to be populated with a valid titleId, developer secret key, and connection string. Additionally, some APIs that are disabled by default need to be enabled via the PlayFab developer portal.

Even without making those changes, the __PlayFabServicesTestApp__ source code serves as a good example of basic PlayFabServices SDK usage.

## Init and Logging in

### Headers

Include __PFServices.h__ to get access to all included PlayFab functionality:

```cpp
#include <playfab/services/PFServices.h>
```

### Initialization

PlayFab initialization requires two function calls: __PFServicesInitialize__ and __PFServiceConfigCreateHandle__. The result of this initialization is a __PFServiceConfigHandle__. You provide this handle to a subsequent login call, directing the call to the correct title in the PlayFab backend.

```cpp
    HRESULT hr = PFServicesInitialize(nullptr); // Add your own error handling when FAILED(hr) == true

    PFServiceConfigHandle serviceConfigHandle{ nullptr };

    hr = PFServiceConfigCreateHandle(
            "https://ABCDEF.playfabapi.com",    // PlayFab API endpoint - obtained in the Game Manager
            "ABCDEF",                           // PlayFab Title id - obtained in the Game Manager
            &serviceConfigHandle);
```

### Logging in

Once you have a __PFServiceConfigHandle__, you can use it to make a player login call. In the SDK, use a __PFAuthenticationLoginWith\*Async__ method like __PFAuthenticationLoginWithCustomIdAsync__. This function allows you to log in a player to PlayFab using a custom string, which is useful during development and testing.

After making a login call, you can check the status of the call with __XAsyncGetStatus__. The status starts as __E_PENDING__ and changes to __S_OK__ after the call completes successfully. If the call fails for some reason, the status reflects that failure. Error handling on all PlayFab Services calls works this way.

Along with an __S_OK__ result, you get back a __PFEntityHandle__. You use this handle to make subsequent PlayFab calls as the logged in player. It includes any material required to authenticate with the PlayFab service as that player.

```cpp
    PFAuthenticationLoginWithSteamRequest request{};
    request.createAccount = true;
    request.customId = "player1";

    XAsyncBlock async{};
    HRESULT hr = PFAuthenticationLoginWithCustomIdAsync(serviceConfigHandle, &request, &async); // Add your own error handling when FAILED(hr) == true
    hr = XAsyncGetStatus(&async, true); // This is doing a blocking wait for completion, but you can use the XAsyncBlock to set a callback instead for async style usage

    std::vector<char> loginResultBuffer;
    PFAuthenticationLoginResult const* loginResult;
    size_t bufferSize;
    hr = PFAuthenticationLoginWithSteamGetResultSize(&async, &bufferSize);
    loginResultBuffer.resize(bufferSize);

    PFEntityHandle entityHandle{ nullptr };
    hr = PFAuthenticationLoginWithSteamGetResult(&async, &entityHandle, loginResultBuffer.size(), loginResultBuffer.data(), &loginResult, nullptr);
```

## Service Calls

After logging the player in, you can now make calls to the PlayFab backend. Here's an example of a call to get files stored in PlayFab for the current player.

### Getting the EntityKey

One thing that can be useful for some calls to PlayFab is knowing the __PFEntityKey__ of the player. Once you have a __PFEntityToken__, you can retrieve a __PFEntityKey__ with __PFEntityGetEntityKey__.

```cpp
    PFEntityKey const* pEntityKey{};
    std::vector<char> entityKeyBuffer;
    size_t size{};
    HRESULT hr = PFEntityGetEntityKeySize(entityHandle, &size); // Add your own error handling when FAILED(hr) == true

    entityKeyBuffer.resize(size);
    hr = PFEntityGetEntityKey(entityHandle, entityKeyBuffer.size(), entityKeyBuffer.data(), &pEntityKey, nullptr);
```

### Calling GetFiles

All PlayFab calls follow a similar pattern of preparing the request object, making the call (using the __PFEntityHandle__ from login), creating an object to receive the response, and then calling a __GetResult__ function to fill the newly created container. 

```cpp
    XAsyncBlock async{};
    PFDataGetFilesRequest requestFiles{};
    requestFiles.entity = pEntityKey;

    HRESULT hr = PFDataGetFilesAsync(entityHandle, &requestFiles, &async); // Add your own error handling when FAILED(hr) == true
    hr = XAsyncGetStatus(&async, true); // This is doing a blocking wait for completion, but you can use the XAsyncBlock to set a callback instead for async style usage

    size_t resultSize;
    hr = PFDataGetFilesGetResultSize(&async, &resultSize);

    std::vector<char> getFilesResultBuffer(resultSize);
    PFDataGetFilesResponse* getFilesResponseResult{ nullptr };
    hr = PFDataGetFilesGetResult(&async, getFilesResultBuffer.size(), getFilesResultBuffer.data(), &getFilesResponseResult, nullptr);
```

## Clean up

When your game is ready to shut down or you need to clean up PlayFab for some other reason, ensure you close all open handles and call __PFServicesUninitializeAsync__.

```cpp
    PFEntityCloseHandle(entityHandle);
    entityHandle = nullptr;

    PFServiceConfigCloseHandle(serviceConfigHandle);
    serviceConfigHandle = nullptr;

    XAsyncBlock async{};
    HRESULT hr = PFServicesUninitializeAsync(&async); // Add your own error handling when FAILED(hr) == true
    hr = XAsyncGetStatus(&async, true); // This is doing a blocking wait for completion, but you can use the XAsyncBlock to set a callback instead for async style usage
```

## Async API pattern

The PlayFab Services SDK follows an [Asynchronous Programming Model](https://learn.microsoft.com/gaming/gdk/_content/gc/system/overviews/async-programming-model). This programming model involves the use of Tasks and Task Queues provided by the [XAsync library](https://learn.microsoft.com/gaming/gdk/_content/gc/system/overviews/async-libraries/async-library-xasync). While it does introduce some complexity, it also brings a high degree of control over asynchronous operations.

This example shows how to make an asynchronous call to __PFDataGetFilesAsync__.

```cpp
    auto async = std::make_unique<XAsyncBlock>();
    async->callback = [](XAsyncBlock* async)
    {
        std::unique_ptr<XAsyncBlock> asyncBlockPtr{ async }; // take ownership of XAsyncBlock

        size_t resultSize;
        HRESULT hr = PFDataGetFilesGetResultSize(async, &resultSize);
        if (SUCCEEDED(hr))
        {
            std::vector<char> getFilesResultBuffer(resultSize);
            PFDataGetFilesResponse* getFilesResponseResult{ nullptr };
            PFDataGetFilesGetResult(async, getFilesResultBuffer.size(), getFilesResultBuffer.data(), &getFilesResponseResult, nullptr);
        }
    };

    PFDataGetFilesRequest requestFiles{};
    requestFiles.entity = m_pEntityKey;
    HRESULT hr = PFDataGetFilesAsync(m_entityHandle, &requestFiles, async.get());
    if (SUCCEEDED(hr))
    {
        async.release(); // at this point, the callback will be called so release the unique ptr
    }

```

## Error handling

Completed **XAsync** operations return HTTP status codes. An error status code manifests as a failure **HRESULT** such as **HTTP_E_STATUS_NOT_FOUND** when calling **XAsyncGetStatus()** or one of the **PF*Get()** APIs.

To see detailed error messages returned by the service see the next section on debugging. These detailed error messages can be useful during development to better understand how the PlayFab service reacts to requests from the client.

## Debugging

The easiest way to see the results and debug any calls in the PlayFab Services SDK is to enable [Debug Tracing](https://learn.microsoft.com/gaming/playfab/sdks/c/tracing). Enabling debug tracing allows you to both see the results in the debugger output window and hook the results into your game's own logs.

## Reference

[API reference documentation](https://learn.microsoft.com/gaming/playfab/api-references/c/pfauthentication/pfauthentication_members)
