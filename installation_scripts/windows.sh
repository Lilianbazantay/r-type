#!/usr/bin/env bash
set -e

# Install CMake
winget install --id Kitware.CMake -e --source winget

# Bootstrap vcpkg if needed
if [ ! -f "./external/vcpkg/vcpkg.exe" ]; then
    echo "Bootstrapping vcpkg..."
    ./external/vcpkg/bootstrap-vcpkg.bat
fi

# Clean build folder
if [ -d "./build" ]; then
    rm -rf "./build"
fi
mkdir -p build

# Configure cmake
cmake -B build -S . \
    -DCMAKE_TOOLCHAIN_FILE=external/vcpkg/scripts/buildsystems/vcpkg.cmake \
    -G "Visual Studio 17 2022"

# Build the .exe
cmake --build build --config Release

cp build/Release/r-type_client.exe r-type_client.exe
cp build/Release/r-type_server.exe r-type_server.exe
