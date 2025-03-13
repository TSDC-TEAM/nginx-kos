#!/bin/bash

usage() {
    echo "Usage: $0 [SDK_PATH] "
    echo ""
    echo "Optional argument:"
    echo "  SDK_PATH - Path to the KasperskyOS SDK."
    echo ""
    echo "Examples:"
    echo "  ./cross-build.sh"
    echo "  ./cross-build.sh \"/opt/KasperskyOS-Community-Edition-<version>\""
    echo ""
}

if [ $# -eq 1 ];then
    SDK_PATH="$1"
fi

SCRIPT_DIR=$(cd "$(dirname ${0})"; pwd)

BUILD=$(pwd)/build
mkdir -p $BUILD && cd $BUILD

export LANG=C
export TARGET_PLATFORM="aarch64-kos"
export PKG_CONFIG=""

[ ! -z $SDK_PATH ] && export SDK_PREFIX=$SDK_PATH

export INSTALL_PREFIX=$SDK_PREFIX/sysroot-$TARGET_PLATFORM

if [ -z "$SDK_PREFIX" ];then
    echo "Path to the KasperskyOS SDK is not specified."
    usage
    exit 1
fi

export PATH="$SDK_PREFIX/toolchain/bin:$PATH"

$SDK_PREFIX/toolchain/bin/cmake -G "Unix Makefiles" \
      -D CMAKE_BUILD_TYPE:STRING=Release \
      -D CMAKE_INSTALL_PREFIX:STRING=$INSTALL_PREFIX \
      -D CMAKE_FIND_ROOT_PATH="$SDK_PREFIX/sysroot-$TARGET_PLATFORM" \
      -D CMAKE_TOOLCHAIN_FILE=$SDK_PREFIX/toolchain/share/toolchain-$TARGET_PLATFORM.cmake \
      $SCRIPT_DIR/ && $SDK_PREFIX/toolchain/bin/cmake --build .
