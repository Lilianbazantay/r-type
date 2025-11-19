#!/usr/bin/env bash
set -e

# Bootstrap vcpkg if needed
if [ ! -f ./external/vcpkg/vcpkg ]; then
    ./external/vcpkg/bootstrap-vcpkg.sh
fi

# Clean build folder
rm -rf build
mkdir build

# Configure & build
cmake -B build -S . \
  -DCMAKE_TOOLCHAIN_FILE=external/vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DCMAKE_BUILD_TYPE=Release

cmake --build build -- -j$(nproc)

cp build/r-type_client r-type_client
cp build/r-type_server r-type_server