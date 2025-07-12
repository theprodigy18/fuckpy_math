#!/bin/bash
set -e

#!/bin/bash
set -e

COMPILER="clang++"
FLAGS="-std=c++17 -O3 -msse4.1"
INCLUDES="-Iinclude -Ithird_party"

SOURCES="$(find src/ -name '*.cpp')"

BUILD_DIR="build"
mkdir -p $BUILD_DIR

echo "Generating $BUILD_DIR/compile_commands.json"

JSON_LIST=()

for file in $SOURCES ; do
    OUTFILE="$BUILD_DIR/cc_$(basename $file .cpp).json"
    $COMPILER $FLAGS $INCLUDES -I"/c/Program Files/Python312/include" -c $SOURCES -MJ $OUTFILE -o /dev/null
    JSON_LIST+=($OUTFILE)
done

echo "[" > $BUILD_DIR/compile_commands.json
for file in ${JSON_LIST[@]} ; do
    cat "$file" >> "$BUILD_DIR/compile_commands.json"
done
# Remove last comma.
sed -i '$ s/.$//' $BUILD_DIR/compile_commands.json
echo "]" >> $BUILD_DIR/compile_commands.json

rm -f $BUILD_DIR/cc_*.json

echo "Generated $BUILD_DIR/compile_commands.json"

    
