#!/usr/bin/env bash
set -e

# Determine vcpkg to use
if [ "$USE_CI_VCPKG" = "1" ]; then
    VCPKG="./external/vcpkg/vcpkg.exe"
else
    if [ -f "./external/vcpkg/vcpkg.exe" ]; then
        VCPKG="./external/vcpkg/vcpkg.exe"
    elif [ -n "$VCPKG_EXE" ]; then
        VCPKG="$VCPKG_EXE"
    else
        echo "Error: no vcpkg found. Please clone submodule or set VCPKG_EXE"
        exit 1
    fi
fi

echo "Using vcpkg at $VCPKG"

# Install CMake on Windows (if needed)
echo "Installing CMake (if not already installed)..."
winget install --id Kitware.CMake -e --source winget --accept-package-agreements --silent || echo "CMake already installed, skipping."

# Bootstrap vcpkg if needed (only for submodule)
if [ ! -f "$VCPKG" ]; then
    echo "Bootstrapping vcpkg..."
    git submodule update --init --recursive
    ./external/vcpkg/bootstrap-vcpkg.bat
fi

# Disable telemetry
export VCPKG_DISABLE_METRICS=1

# Install dependencies from manifest
echo "Installing dependencies from vcpkg.json..."
"$VCPKG" install --triplet x64-windows

# Clean and configure build
if [ -d "./build" ]; then
    rm -rf "./build"
fi
mkdir -p build

cmake -B build -S . \
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG/scripts/buildsystems/vcpkg.cmake" \
    -G "Visual Studio 17 2022"

# Build executables
cmake --build build --config Release

cp build/Release/r-type_client.exe r-type_client.exe
cp build/Release/r-type_server.exe r-type_server.exe
