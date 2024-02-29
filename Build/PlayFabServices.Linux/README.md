# Building PlayFab for Linux

Running the `./PlayFabServices_Linux.bash` script will be used to build the PlayFab Services shared library and its dependencies.  

## Dependencies

To run the build script you will need Linux either as installed on physical machine, or a Linux virtual machine, or by using the Windows Subsystem for Linux (WSL).

## PlayFabServices_Linux.bash

Running `./PlayFabServices_Linux.bash` will generate a variety of build configurations and binaries for PlayFab and its dependencies. These binaries will be placed at `/Out/x64/{Configuration}/{Library}`. After the PlayFab Services shared library is built by default it will be found at `/Out/x64/Release/PlayFabServices.Linux/PlayFabServicesLinux.so`

Example usage:

```
./PlayFabServices_Linux.bash
```

Running the build script with no arguments will generate a Release binaries.
Running the build script with the `-c|--config` argument will generate Debug or Release binaries.

```
./PlayFabServices_Linux.bash <-c|--config> <Debug|Release>
```

Running the build script with the `-lhc|--libHttpClientPath` argument will set the path to the libHttpClient repo.  By default its assume to be a sibling of this repo.  You can alternatively set the LHC_PATH enviroment varible to control this, for example: `export LHC_PATH=../libHttpClient`

```
./PlayFabServices_Linux.bash <-lhc|--libHttpClientPath> ../libHttpClient
```

Running the build script with the `-sg|--skipaptget` argument will skip calling apt-get which installs the required components.

Running the build script with the -nc|--nocore will not generate a binary of `PlayFabCore.Linux.so`.  Might be useful for advanced build scenarios

```
./PlayFabServices_Linux.bash <-nc|--nocore>
```

Running the build script with the -ns|--nosharedinternal will not generate a binary of `PlayFabSharedInternal.a`.  Might be useful for advanced build scenarios

```
./PlayFabServices_Linux.bash <-ns|--nosharedinternal>
```

Running the build script with the -nlhc|--nolibhttpclient will not generate a binary of `libHttpClient.Linux.so`.  Might be useful for advanced build scenarios

```
./PlayFabServices_Linux.bash <-nlhc|--nolibhttpclient>
```

## FAQ

If the bash script fails to run and produces the error:
```
/bin/bash^M: bad interpreter
```
This is due to improper line endings.  Running the following command and re-running the script should fix it.
```
sed -i -e 's/\r$//' PlayFabServices_Linux.bash
```
