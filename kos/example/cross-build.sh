#!/bin/bash

usage() {
    echo "Usage: $0 <TARGET> [SDK_PATH] "
    echo ""
    echo "Available targets":
    echo "  qemu     - build for QEMU."
    echo "  hw       - build for hardware."
    echo ""
    echo "Optional argument:"
    echo "  SDK_PATH - Path to the KasperskyOS SDK."
    echo ""
    echo "Examples:"
    echo "  ./cross-build.sh qemu"
    echo "  ./cross-build.sh hw \"/opt/KasperskyOS-Community-Edition-<version>\""
    echo ""
}

if [ $# -eq 0 ];then
    usage
    exit 0
fi

if [ "$1" == "qemu" ];then
    TARGET="sim"
elif [ "$1" == "hw" ];then
    TARGET="sd-image"
else
    echo "Unknown target platform is specified."
    usage
    exit 1
fi

if [ $# -eq 2 ];then
    SDK_PATH="$2"
fi

SCRIPT_DIR=$(cd "$(dirname ${0})"; pwd)

BUILD=$(pwd)/build
mkdir -p $BUILD && cd $BUILD

export LANG=C
export TARGET_PLATFORM="aarch64-kos"
export PKG_CONFIG=""
export INSTALL_PREFIX=$BUILD/../install

[ ! -z $SDK_PATH ] && export SDK_PREFIX=$SDK_PATH

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
      $SCRIPT_DIR/ && $SDK_PREFIX/toolchain/bin/cmake --build . --target $TARGET
