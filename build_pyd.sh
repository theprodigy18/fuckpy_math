#!/bin/bash
set -e

COMPILER="clang++"
FLAGS="-std=c++17 -O3 -msse4.1 -shared"
INCLUDES="-Iinclude -Ithird_party"
LIBS="-lpython312"

SOURCES="$(find src/ -name '*.cpp')"

BUILD_DIR="build"
mkdir -p $BUILD_DIR

OUTFILE="$BUILD_DIR/fuckpy_math.pyd"

$COMPILER $FLAGS $INCLUDES -I"/c/Program Files/Python312/include" -L"/c/Program Files/Python312/libs" $LIBS $SOURCES -o$OUTFILE 