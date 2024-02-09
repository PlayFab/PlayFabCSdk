#!/bin/bash
log () {
    echo "***** $1 *****"
}

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

POSITIONAL_ARGS=()

# Default configurations
CONFIGURATION="Release"
BUILD_PF_CORE=true
BUILD_PF_SHARED_INTERNAL=true
BUILD_LHC=true
if [ "$LHC_PATH" == "" ]; then
  LHC_PATH="../libHttpClient"
fi

while [[ $# -gt 0 ]]; do
  case $1 in
    -c|--config)
      CONFIGURATION="$2"
      shift # past argument
      shift # past value
      ;;
    -lhc|--libHttpClientPath)
      LHC_PATH="$2"
      shift # past argument
      shift # past value
      ;;
    -nc|--nocore)
      BUILD_PF_CORE=false
      shift
      ;;
    -ns|--nosharedinternal)
      BUILD_PF_SHARED_INTERNAL=false
      shift
      ;;
    -nlhc|--nolibhttpclient)
      BUILD_LHC=false
      shift
      ;;
    -sg|--skipaptget)
      DO_APTGET=false
      shift
      ;;
    -*|--*)
      echo "Unknown option $1"
      exit 1
      ;;
    *)
      POSITIONAL_ARGS+=("$1") # save positional arg
      shift # past argument
      ;;
  esac
done

set -- "${POSITIONAL_ARGS[@]}" # restore positional parameters

if [ "$DO_APTGET" != "false" ]; then
  sudo hwclock --hctosys
  sudo apt-get update
  sudo apt-get install clang
  sudo apt-get install cmake
  sudo apt-get install make
  sudo apt-get install autoconf
  sudo apt-get install automake
  sudo apt-get install libtool
fi

# make libHttpClient
if [ "$BUILD_LHC" = true ]; then
    log "Building libHttpClient"
    sed -i -e 's/\r$//' "$SCRIPT_DIR"/../../"$LHC_PATH"/Build/libHttpClient.Linux/libHttpClient_Linux.bash
    bash "$SCRIPT_DIR"/../../"$LHC_PATH"/Build/libHttpClient.Linux/libHttpClient_Linux.bash -c "$CONFIGURATION" -sg
fi

# make PlayFabSharedInternal and PlayFabCore
if [ "$BUILD_PF_SHARED_INTERNAL" = true ]; then
    log "Building PlayFabSharedInternal"
    sed -i -e 's/\r$//' "$SCRIPT_DIR"/../PlayFabSharedInternal.Linux/PlayFabSharedInternal_Linux.bash
    bash "$SCRIPT_DIR"/../PlayFabSharedInternal.Linux/PlayFabSharedInternal_Linux.bash -c "$CONFIGURATION" -lhc "$LHC_PATH"
fi

if [ "$BUILD_PF_CORE" = true ]; then
    log "Building PlayFabCore"
    sed -i -e 's/\r$//' "$SCRIPT_DIR"/../PlayFabCore.Linux/PlayFabCore_Linux.bash
    bash "$SCRIPT_DIR"/../PlayFabCore.Linux/PlayFabCore_Linux.bash -c "$CONFIGURATION" -lhc "$LHC_PATH"
fi

log "#### Building PlayFabServices ####" 
log "CONFIGURATION = ${CONFIGURATION}"
log "LHC_PATH = ${LHC_PATH}"
log "BUILD_LHC = ${BUILD_LHC}"
log "BUILD_PF_SHARED_INTERNAL = ${BUILD_PF_SHARED_INTERNAL}"
log "BUILD_PF_CORE = ${BUILD_PF_CORE}"

# make PlayFabServices
sudo cmake -S "$SCRIPT_DIR" -B "$SCRIPT_DIR"/../../Int/CMake/PlayFabServicesLinux -D CMAKE_BUILD_TYPE=$CONFIGURATION -D LHC_PATH=$LHC_PATH
sudo make -C "$SCRIPT_DIR"/../../Int/CMake/PlayFabServicesLinux
