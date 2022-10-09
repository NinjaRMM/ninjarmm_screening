#!/bin/bash

SOURCE_DIR=$(pwd)
BUILD_DIR="${SOURCE_DIR}/build/$(uname)"

if [ ! -d "${BUILD_DIR}" ]; then
    mkdir -p ${BUILD_DIR}
fi

cd ${BUILD_DIR}
cmake -DCMAKE_BUILD_TYPE=Debug "${SOURCE_DIR}" -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cd -

make -C ${BUILD_DIR}
