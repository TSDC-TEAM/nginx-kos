#!/bin/bash

usage() {
    echo "Usage: $0 [SDK_PATH] "
    echo ""
    echo "Optional argument:"
    echo "  SDK_PATH - Path to the KasperskyOS SDK."
    echo ""
    echo "Examples:"
    echo "  ./uninstall.sh"
    echo "  ./uninstall.sh \"/opt/KasperskyOS-Community-Edition-<version>\""
    echo ""
}

if [ $# -eq 1 ];then
    SDK_PATH="$1"
fi

[ ! -z $SDK_PATH ] && export SDK_PREFIX=$SDK_PATH

if [ -z "$SDK_PREFIX" ];then
    echo "Path to the KasperskyOS SDK is not specified."
    usage
    exit 1
fi

BUILD=$(pwd)/build

cd $BUILD && make uninstall
