#!/usr/bin/env bash
set -e

# Install CMake
# Install CMake (safe if already installed)
echo "Installing CMake (if not already installed)..."
winget install --id Kitware.CMake -e --source winget --accept-package-agreements --silent || echo "CMake already installed, skipping."

# Bootstrap vcpkg if needed
if [ ! -f "./external/vcpkg/vcpkg.exe" ]; then
    echo "Bootstrapping vcpkg..."
    git submodule update --init --recursive
    ./external/vcpkg/bootstrap-vcpkg.bat
fi

# Update vcpkg ports (fixes many brotli issues)
cd external/vcpkg
git pull
./vcpkg.exe update
cd ../..

# Install all dependencies from vcpkg.json BEFORE running CMake
echo "Installing dependencies from vcpkg.json..."
external/vcpkg/vcpkg.exe install --triplet x64-windows

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
