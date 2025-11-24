#!/usr/bin/env bash
set -e

# Determine which vcpkg to use
# If VCPKG_EXE environment variable is set (from CI), use it.
# Otherwise, fallback to submodule
if [ -n "$VCPKG_EXE" ]; then
    VCPKG="$VCPKG_EXE"
    echo "Using system vcpkg at $VCPKG"
else
    VCPKG="./external/vcpkg/vcpkg.exe"
fi

# Install CMake
echo "Installing CMake (if not already installed)..."
winget install --id Kitware.CMake -e --source winget --accept-package-agreements --silent || echo "CMake already installed, skipping."


# Bootstrap vcpkg if using submodule
if [ ! -f "$VCPKG" ]; then
    echo "Bootstrapping vcpkg..."
    git submodule update --init --recursive
    ./external/vcpkg/bootstrap-vcpkg.bat
fi

# Install dependencies from manifest
# vcpkg update does not support manifest mode, skip it
echo "Installing dependencies from vcpkg.json..."
"$VCPKG" install --triplet x64-windows

# Disable telemetry
export VCPKG_DISABLE_METRICS=1

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

# Copy executables
cp build/Release/r-type_client.exe r-type_client.exe
cp build/Release/r-type_server.exe r-type_server.exe
