#!/bin/bash
log () {
    echo "***** $1 *****"
}

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

POSITIONAL_ARGS=()

# Default configurations
CONFIGURATION="Release"
if [ "$LHC_PATH" == "" ]; then
  LHC_PATH="$SCRIPT_DIR/../../../../libHttpClient"
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

log "#### Building TestApp ####" 
log "CONFIGURATION = ${CONFIGURATION}"
log "LHC_PATH = ${LHC_PATH}"

make TestApp
sudo cmake -S "$SCRIPT_DIR" -B "$SCRIPT_DIR/CMakeOut" -D LHC_PATH=$LHC_PATH
sudo make -C "$SCRIPT_DIR/CMakeOut"

