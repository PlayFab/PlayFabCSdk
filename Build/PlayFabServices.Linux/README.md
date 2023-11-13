# Building PlayFab for Linux

These scripts will be used to build the static library dependencies for PlayFab and link them against the static library build for PlayFab.

## Dependencies

You will need a Linux machine, a Linux virtual machine, or Windows Subsystem for Linux (WSL) to run the build script.

## PlayFabServices_Linux.bash

Running `PlayFabServices_Linux` can generate a variety of build configurations and binaries for PlayFab and its dependencies. These binaries will be placed at `Binaries/{Configuration}/x64/{Library}`. Example usage is below.

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

The binaries are `PlayFabCore.a`, `PlayFabServices.a`, `PlayFabSharedInternal.a`, `libssl.a`, `libcrypto.a`.

Running the build script with the -nc|--nocore will not generate a binary of PlayFabCore.a.  Might be useful for advanced build scenarios

```
./PlayFabServices_Linux.bash <-nc|--nocore>
```

Running the build script with the -ns|--nosharedinternal will not generate a binary of PlayFabSharedInternal.a.  Might be useful for advanced build scenarios

```
./PlayFabServices_Linux.bash <-ns|--nosharedinternal>
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
