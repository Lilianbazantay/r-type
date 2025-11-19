#!/usr/bin/env bash

./external/vcpkg/bootstrap-vcpkg.sh

# clean up the build directory
rm -rf ../build
rm -rf ../CMakeFiles
rm -f ../CMakeCache.txt

# recreate it fully with the right path
mkdir ../build
cd ../build

cmake -B ../build -S . \
  -DCMAKE_TOOLCHAIN_FILE=external/vcpkg/scripts/buildsystems/vcpkg.cmake

cmake --build ../build
