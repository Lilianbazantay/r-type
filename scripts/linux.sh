#!/usr/bin/env bash
set -e

# Directories
VCPKG_DIR=external/vcpkg
BUILD_DIR=build

# Add missing dependencies
echo "Checking for any missing dependencies"
echo "If missing the following dependencies will be added:"
echo "cmake ninja freetype X11 xcb xrandr xi xcursor udev openal opengl and libtool"
echo "to find more information refer yourself to the readme at the root of the project repository."

if command -v dnf &> /dev/null; then
    dnf update
    dnf install --skip-unavailable \
      @development-tools cmake ninja-build \
      freetype-devel libX11-devel libxcb-devel \
      libXrandr-devel libXi-devel libXcursor-devel \
      libudev-devel openal-soft-devel mesa-libEGL-devel pkg-config \
      libtool mesa-libGL-devel libglvnd-devel
elif command -v apt &> /dev/null; then
    apt-get update
    apt-get install \
      build-essential cmake ninja-build \
      libfreetype6-dev libx11-dev libxcb1-dev \
      libxrandr-dev libxi-dev libxcursor-dev \
      libudev-dev libopenal-dev libgl1-mesa-dev pkg-config \
      libtool
elif command -v pacman &> /dev/null; then
    pacman update
    pacman install \
      build-essential cmake ninja-build \
      libfreetype6-dev libx11-dev libxcb1-dev \
      libxrandr-dev libxi-dev libxcursor-dev \
      libudev-dev libopenal-dev libgl1-mesa-dev pkg-config \
      libtool
fi

# Bootstrap vcpkg if neededcd $VCPKG_DIR
if [ ! -f "$VCPKG_DIR/vcpkg" ] ; then
    echo "Bootstrapping vcpkg..."
    git submodule update --init --recursive
    "$VCPKG_DIR/bootstrap-vcpkg.sh"
fi

# Disable telemetry
export VCPKG_DISABLE_METRICS=1

# Ensure vcpkg is executable
export PATH="$VCPKG_DIR:$PATH"

# Prepare build directory
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure CMake
export CMAKE_GENERATOR=Ninja
export CMAKE_MAKE_PROGRAM=/usr/bin/ninja

cmake .. \
  -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake" \
  -DCMAKE_BUILD_TYPE=Release \
  -DVCPKG_TARGET_TRIPLET=x64-linux \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Build project
cmake --build . --parallel

# Copy executables to root
cp r-type_client ../r-type_client || true
cp r-type_server ../r-type_server || true
cp r-type_app ../r-type_app || true
cp flappy_server ../flappy_server || true
cp flappy_client ../flappy_client || true

echo "Build complete!"
