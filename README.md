# PlayFabCSdk
Cross-Platform C/C++ PlayFab SDK.

This SDK currently supports the following platforms:
- Windows (Win32) x64
- GDK (Xbox and Windows)

For per-platform getting started guides and a complete list of available APIs, check out the [online documentation](https://learn.microsoft.com/gaming/playfab/sdks/c).

This SDK lets you make REST API calls to the PlayFab service. It depends on some external third-party open source libraries referenced as git submodules in directory `/External`. Please make sure to use `--recurse-submodules` command line parameter when cloning this git repo:
```
git clone --recurse-submodules <git-repo-url>
```
or run the following commands to sync the content of submodules if the repo was cloned earlier without the `--recurse-submodules` parameter:

```
git submodule update --init --recursive
```

---
## Building the SDK
The main solution file references all projects for each platform:
```
PlayFab.C.vs2022
|
|-- GDK
  |
  |-- GDK Projects
  |-- GDK Shared Items
|-- Win32
|-- ...
|-- PlayFabCore Shared Items
|-- PlayFabServices Shared Items
|-- ...
```
The build projects are created for a build tool that is popular or commonly used on that platform (e.g. Visual Studio projects for Windows, Xbox).
There are common Shared Items projects for code that every platform uses and separate Shared Items projects for per-platform code. Each platform's main projects reference all the relevant Shared Items projects for that platform.

### 1. Prerequisites:

* IDE or a build tool specific to platform (Visual Studio 2022 for Windows, Xbox)
* Windows and GDK projects target platform toolset v143 (included in Visual Studio 2022).

### 2. Installation & Configuration Instructions:

Currently this project includes the SDK and test apps.

* Open projects/solution in the IDE/build tool
* Build SDK