#!/usr/bin/env bash
set -e

# Install CMake
winget install --id Kitware.CMake -e --source winget


# Bootstrap vcpkg if needed
if (-Not (Test-Path "./external/vcpkg/vcpkg.exe")) {
    Write-Host "Bootstrapping vcpkg..."
    & "./external/vcpkg/bootstrap-vcpkg.bat"
}

# Clean build folder
if (Test-Path "./build") {
    Remove-Item -Recurse -Force "./build"
}
New-Item -ItemType Directory -Path "./build"

# Configure
cmake -B build -S . `
    -DCMAKE_TOOLCHAIN_FILE=external/vcpkg/scripts/buildsystems/vcpkg.cmake `
    -G "Visual Studio 17 2022"  # Adjust to your installed VS generator

# Build
cmake --build build --config Release

cp build/r-type_client r-type_client
cp build/r-type_server r-type_server