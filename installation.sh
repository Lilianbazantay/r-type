#!/usr/bin/env bash

# detect the os and run the installation accordingly
UNAME=$(uname -s)

case "$UNAME" in
    Linux*)
        Linux
        ;;
    Darwin*)
        Linux
        ;;
    MINGW*|MSYS*|CYGWIN*)
        windows
        ;;
    *)
        echo "Unknown OS: $UNAME"
        ;;
esac



# clean up the build directory
rm -rf build
rm -rf CMakeFiles
rm -f CMakeCache.txt

# recreate it fully with the right path
mkdir build
cd build
cmake ..

