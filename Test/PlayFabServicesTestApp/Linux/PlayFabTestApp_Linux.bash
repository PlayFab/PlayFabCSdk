#!/bin/bash
log () {
    echo "***** $1 *****"
}

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
TEST_TITLE_JSON="${SCRIPT_DIR}/../../testTitleData.json"

POSITIONAL_ARGS=()

# Default configurations
CONFIGURATION="Release"
BUILD_PF=true
if [ "$LHC_PATH" == "" ]; then
  LHC_PATH="${SCRIPT_DIR}/../../../../libHttpClient"
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
  sudo apt-get install zlib1g zlib1g-dev
fi

log "#### Building TestApp ####"
log "CONFIGURATION = ${CONFIGURATION}"
log "LHC_PATH = ${LHC_PATH}"
log "BUILD_PF = ${BUILD_PF}"

make TestApp
sudo cmake -S "$SCRIPT_DIR" -B "$SCRIPT_DIR"/../../../Int/CMake/PlayFabTestApp.Linux -D CMAKE_BUILD_TYPE=$CONFIGURATION -D LHC_PATH=$LHC_PATH
sudo make -C "$SCRIPT_DIR"/../../../Int/CMake/PlayFabTestApp.Linux
sudo cp "$TEST_TITLE_JSON" "${SCRIPT_DIR}/../../../Out/x64/$CONFIGURATION/PlayFabTestApp.Linux/"
