#!/usr/bin/env bash
set -e

# Directories
VCPKG_DIR=external/vcpkg
BUILD_DIR=build

# Add missing dependencies
echo "Checking for any missing dependencies"
if command -v dnf &> /dev/null; then
    sudo dnf update
    sudo dnf install -y --skip-unavailable \
      build-essential cmake ninja-build \
      libfreetype6-dev libx11-dev libxcb1-dev \
      libxrandr-dev libxi-dev libxcursor-dev \
      libudev-dev libopenal-dev libgl1-mesa-dev pkg-config
elif command -v apt &> /dev/null; then
    sudo apt-get update
    sudo apt-get install -y \
      build-essential cmake ninja-build \
      libfreetype6-dev libx11-dev libxcb1-dev \
      libxrandr-dev libxi-dev libxcursor-dev \
      libudev-dev libopenal-dev libgl1-mesa-dev pkg-config
elif command -v pacman &> /dev/null; then
    sudo pacman update
    sudo pacman install -y \
      build-essential cmake ninja-build \
      libfreetype6-dev libx11-dev libxcb1-dev \
      libxrandr-dev libxi-dev libxcursor-dev \
      libudev-dev libopenal-dev libgl1-mesa-dev pkg-config
fi

# Bootstrap vcpkg if neededcd $VCPKG_DIR
if ! -f "$VCPKG_DIR/vcpkg" ]; then
    echo "Bootstrapping vcpkg..."
    git submodule update --init --recursive
    "$VCPKG_DIR/bootstrap-vcpkg.sh"
fi

# Ensure vcpkg is executable
export PATH="$VCPKG_DIR:$PATH"

# Prepare build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure CMake
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake" \
  -DCMAKE_BUILD_TYPE=Release \
  -DVCPKG_TARGET_TRIPLET=x64-linux \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Build project
cmake --build . --parallel

# Copy executables to root
cp r-type_client ../r-type_client || true
cp r-type_server ../r-type_server || true

echo "Build complete!"
