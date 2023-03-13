#!/bin/bash

if [ -z $SDK_PREFIX ]; then
    echo "Error: SDK_PREFIX is empty."
    exit 1
fi

BUILD=$(pwd)/build
cd $BUILD

$SDK_PREFIX/toolchain/bin/cmake --install .
