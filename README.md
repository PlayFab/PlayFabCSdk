# PlayFabCSdk
Cross-Platform C/C++ PlayFab SDK.

This SDK currently supports the following platforms:
- Windows (Win32) x64
- GDK (Xbox and Windows)
- Linux (eg. Ubuntu 22.04 or Windows Subsystem for Linux)
- iOS
- macOS
- Android
- PlayStation 4/5 (private repo)
- Nintendo Switch (private repo)

For per-platform getting started guides and a complete list of available APIs, check out the [online documentation](https://learn.microsoft.com/gaming/playfab/sdks/c).

To gain access to platforms supported through private repos, [Request Access](https://learn.microsoft.com/gaming/playfab/features/multiplayer/networking/request-access-for-sdks-samples).

## Requirements

### Win32 and GDK
- A [PlayFab developer account](https://developer.playfab.com).
- [Visual Studio 2022](https://visualstudio.microsoft.com/) installed (to build from source).
- (For GDK) [Microsoft Game Development Kit](https://learn.microsoft.com/gaming/gdk/) installed.

### iOS / macOS
- A [PlayFab developer account](https://developer.playfab.com).
- [XCode](https://developer.apple.com/xcode/) IDE installed (to build from source).

### Android
- A [PlayFab developer account](https://developer.playfab.com).
- [Android Studio](https://developer.android.com/studio) IDE installed (to build from source).

## Project setup

### Building with binary release package

Download the most recent release of the SDK from [PlayFabCSdk releases](https://github.com/PlayFab/PlayFabCSdk/releases). 

#### Integrate PlayFab C SDK into your own project - Visual Studio (Win32 and GDK)

Each release package contains the required headers, binaries, and property sheet file (Win32 - PlayFabServicesSDK.Win32.props, GDK - PlayFabServicesSDK.GDK.props) that can be imported to your project to automatically add references.

You can import the props file manually by editing the .vcxproj files directly or by opening the Property Manager window in Visual Studio, right clicking on the project, and choosing __Add Existing Property Sheet__. After importing the appropriate props file, the headers should be available in your include path and references to the PlayFabServices library and its dependecies will be automatically added.

> __Note for GDK__: It is possible for other GDK extensions such as the Xbox Services API or libHttpClient to be included as references automatically in projects.
Any duplicate references will need to be removed manually.
To do this, you can manage extensions in Visual Studio through __Project -> Properties -> Gaming Desktop -> Gaming Extension Libraries__.

#### Integrate PlayFab C SDK into your own project - XCode

##### Add binaries to your game

After getting your binaries by download or building from source you should be able to easily integrate them into your game/app.

Follow these instructions to add them:

1. In XCode, navigate to your desired target and select it.
2. Scroll down and in the "**Frameworks, Libraries, and Embedded Content**" section, click on the "**+**" sign.
3. Search for your **PlayFabServices** / **PlayFabCore** / **HttpClient** binaries. (_If you downloaded the xcframework file you can also select and it will automatically import whatever library is required independently if you're building for devices or simulator._)

##### Add Header search paths

Once binaries have been added you'll need to ensure that header search paths are also correctly set up.

1. Navigate to your project.
2. Click on "**Build Settings**" and then search for "**Header Search Paths**".
3. Update the property value to include the SDK Headers. Please note that if you are using binaries from the downloaded release package you can simply reference the headers found in the **include** folder. However, if you are using binaries built from source you'll need to include the following paths:
    ```
    PlayFab.C/Source/PlayFabServices/Include/Generated/
    PlayFab.C/Source/PlayFabCore/Include/
    PlayFab.C/Source/PlayFabCore/Include/Generated
    libHttpClient/Include
    ```

#### Integrate PlayFab C SDK into your own project - Android Studio

##### Add Shared Objects and Header files

1. Using __target_include_directories__ or another equivalent function, add the headers under "Include" from the PlayFab SDK release:
```
TARGET_INCLUDE_DIRECTORIES(
    ${PROJECT_NAME}
    "Include"
)
```

If you're building from source, add this list of headers from the PlayFab SDK repository:

```
    "PlayFab.C/Source/PlayFabServices/Include"
    "PlayFab.C/Source/PlayFabServices/Include/Generated"
    "PlayFab.C/Source/PlayFabCore/Include"
    "PlayFab.C/Source/PlayFabCore/Include/Generated"
    "libHttpClient/Include"
```

3. Using __target_link_libraries__ or another equivalent function, link the locations of the .so files to your project.

    For example:

```cmake
set(PLAYFAB_SERVICES_PATH "[LOCATION OF YOUR FILE]/libPlayFabServices.Android.so")

set(PLAYFAB_CORE_PATH "[LOCATION OF YOUR FILE]/libPlayFabCore.Android.so")

set(LIBHTTPCLIENT_PATH "[LOCATION OF YOUR FILE]/libHttpClient.Android.so")

TARGET_LINK_LIBRARIES(
    [YOUR PROJECT NAME]
    ${PLAYFAB_SERVICES_PATH}
    ${PLAYFAB_CORE_PATH}
    ${LIBHTTPCLIENT_PATH}
)
```

##### Add Android Archive files

1. Create a libs folder within app level Android project directory.

2. Copy the .aar files into the libs folder.

3. In app level build.gradle file, the one in the same directory as the libs folder, add these lines to the dependencies section. The second line is required as a dependency for libHttpClient.
```gradle
implementation fileTree(dir: 'libs', include: ['*.aar'])
implementation 'com.squareup.okhttp3:okhttp:4.9.1'
```


### Cloning source

This SDK depends on some external third-party open source libraries referenced as git submodules in directory `/External`. Please make sure to use `--recurse-submodules` command line parameter when cloning this git repo:
```
git clone --recurse-submodules https://github.com/PlayFab/PlayFabCSdk
```
or run the following commands to sync the content of submodules if the repo was cloned earlier without the `--recurse-submodules` parameter:

```
git submodule update --init --recursive
```

### Building from source for Windows and GDK

After cloning this repository and its submodules, you can build the libraries by opening the solution file PlayFab.C.vs2022.sln. That solution contains the PlayFabServices project, its dependencies, and a test app. After building, the binaries will be in a subdirectory within \Out.

#### Import Property Sheet (Windows and GDK)

To add a reference to the PlayFabServices projects to your own title, import the \Build\PlayFabServices.import.props file to your project. You can do this manually by editing the .vcxproj files directly or by opening the Property Manager window in Visual Studio, right clicking on the project, and choosing __Add Existing Property Sheet__. Note that the props file differs from the PlayFabServicesSDK.Win32.props mentioned above - it adds references to the projects instead of the prebuilt binaries.

#### Building (Windows and GDK)

To add a reference to the PlayFabServices projects to your own title, import the \Build\PlayFabServices.import.props file to your project. You can do this manually by editing the .vcxproj files directly or by opening the Property Manager window in Visual Studio, right clicking on the project, and choosing __Add Existing Property Sheet__. Note that the props file differs from the PlayFabServicesSDK.Win32.props mentioned above - it adds references to the projects instead of the prebuilt binaries.

#### PlayFabServicesTestApp

The included test app __PlayFabServicesTestApp__ project will build as is, but some additional setup is required before it will run. The __\Test\testTitleData.json__ file needs to be populated with a valid titleId, developer secret key, and connection string. Additionally, some APIs that are disabled by default need to be enabled via the PlayFab developer portal.

Even without making those changes, the __PlayFabServicesTestApp__ source code serves as a good example of basic PlayFabServices SDK usage.

### Building from source for Linux

After cloning this repository and its submodules, you can build the libraries by running

```
./Build/PlayFabServices.Linux/PlayFabServices_Linux.bash
```
For more detail on script options see [PlayFabServices_Linux](Build/PlayFabServices.Linux/README.md)

Once built, the shared library can be used by adding this to your project's CMakeLists.txt:

```
find_package(PlayFabServicesLinux REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC PlayFabServicesLinux)
```

when running cmake, add this option and adjust the path as needed to tell it where package material can be found:

```
-D PlayFabServicesLinux_DIR=./Int/CMake/PlayFabServicesLinux
```

See `.\Test\PlayFabServicesTestApp\Linux` for example CMake usage.

### Building from source for iOS

After cloning this repository and its submodules, you can build the libraries by opening the workspace [PlayFab.C.Apple.xcworkspace](Build/PlayFab.C.Apple/PlayFab.C.Apple.xcworkspace) with XCode, then follow these steps:

1. Select the scheme you want to build.
- If you want to build PlayFabServices and PlayFabCore frameworks, select **PlayFabServices_iOS**/**PlayFabServices_._macOS** scheme and build. This scheme will generate the frameworks required to use PlayFab SDK:
    - PlayFabCore_[iOS/macOS].framework
    - PlayFabServices_[iOS/macOS].framework
    - HttpClient_[iOS/macOS].framework

- If you want to run the Test App, select the **PlayFabServicesTestApp_._iOS**/**PlayFabServicesTestApp_macOS** scheme, build and run the app.

2. Select the run destination. 

- For macOS you should be able to run on your own Mac, however, for iOS, you should select a Simulator running iOS 16.2 (or later) or you can run it on a physical devices attached to your Mac. Be aware that libraries built for simulators won't work on real devices and viceversa, so you need to choose the run destination based on where you want to run your app.

3. Grab the binaries. There are 2 different ways to do it:

    1. Binaries will be placed in XCode default build directory which it's in the DerivedData folder usually located in:

        ``~/Library/Developer/Xcode/DerivedData``

        In there, you can go to the target folder and navigate to the Build/Products folder to get all the binaries.

    2. Another way to get to the binaries if you want to navigate directly to them:
    
        1. In the workspace navigation on XCode go to "**Products**" and expand it.
        2. Right-click on the framework generated and click "**Show on Finder**".

### Building from source for Android

After cloning this repository and its submodules, you can build the libraries from the `PlayFab.C/Build/PlayFabServices.Android.Workspace` directory by running:
```
./gradlew assembleFull
```

This will build the library in two configurations, debug and release, and two Android ABIs, arm64-v8a and x86_64.

If you wish to build separately for a single configuration, ABI, or both, the command can be freely mix and matched for this purpose. 

Some examples of altering the command:

This builds for debug arm64-v8a and debug x86_64.
```
./gradlew assembleFullDebug
```

This builds for release x86_64.
```
./gradlew assemblex86_64Release
```

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

Once you have a __PFServiceConfigHandle__, you can use it to make a player login call. In the SDK, use a __PFAuthenticationLoginWith\*Async__ method like __PFAuthenticationLoginWithCustomIDAsync__. This function allows you to log in a player to PlayFab using a custom string, which is useful during development and testing.

After making a login call, you can check the status of the call with __XAsyncGetStatus__. The status starts as __E_PENDING__ and changes to __S_OK__ after the call completes successfully. If the call fails for some reason, the status reflects that failure. Error handling on all PlayFab Services calls works this way.

Along with an __S_OK__ result, you get back a __PFEntityHandle__. You use this handle to make subsequent PlayFab calls as the logged in player. It includes any material required to authenticate with the PlayFab service as that player.

```cpp
    PFAuthenticationLoginWithCustomIDRequest request{};
    request.createAccount = true;
    request.customId = "player1";

    XAsyncBlock async{};
    HRESULT hr = PFAuthenticationLoginWithCustomIDAsync(serviceConfigHandle, &request, &async); // Add your own error handling when FAILED(hr) == true
    hr = XAsyncGetStatus(&async, true); // This is doing a blocking wait for completion, but you can use the XAsyncBlock to set a callback instead for async style usage

    std::vector<char> loginResultBuffer;
    PFAuthenticationLoginResult const* loginResult;
    size_t bufferSize;
    hr = PFAuthenticationLoginWithCustomIDGetResultSize(&async, &bufferSize);
    loginResultBuffer.resize(bufferSize);

    PFEntityHandle entityHandle{ nullptr };
    hr = PFAuthenticationLoginWithCustomIDGetResult(&async, &entityHandle, loginResultBuffer.size(), loginResultBuffer.data(), &loginResult, nullptr);
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
