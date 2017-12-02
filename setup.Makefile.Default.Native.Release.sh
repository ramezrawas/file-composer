#!/bin/bash
DIR="$( cd "$(dirname "$0")" ; pwd -P )"
BUILD_DIR=$DIR/build/Makefile/Release
SOURCE_DIR=$DIR/Source

mkdir -p $BUILD_DIR
pushd $BUILD_DIR
   cmake \
   -DBIN_PATH_POSTFIX=Makefile/Release\
   -DCMAKE_BUILD_TYPE=Release\
    $SOURCE_DIR 
popd

cmake --build $BUILD_DIR --config Release
