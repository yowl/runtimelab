#!/usr/bin/env bash

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)

source $SCRIPT_DIR/FindWasmHostExecutable.sh "$1"

if [ -n "${WASM_HOST_EXECUTABLE}" ]; then
  echo $WASM_HOST_EXECUTABLE "$WASM_BINARY_TO_EXECUTE" "${@:2}"
  $WASM_HOST_EXECUTABLE "$WASM_BINARY_TO_EXECUTE" "${@:2}"
else
  exit 1
fi
