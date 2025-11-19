#!/usr/bin/env bash
set -e

# Directories
VCPKG_DIR=external/vcpkg
BUILD_DIR=build

# ------------------------------
# Install vcpkg if needed
# ------------------------------
if [ ! -f "$VCPKG_DIR/vcpkg" ]; then
    echo "Bootstrapping vcpkg..."
    "$VCPKG_DIR/bootstrap-vcpkg.sh"
fi

# Ensure vcpkg is executable
export PATH="$VCPKG_DIR:$PATH"

# ------------------------------
# Install vcpkg dependencies
# ------------------------------
echo "Installing vcpkg dependencies..."
"$VCPKG_DIR/vcpkg" install --triplet x64-linux --clean-after-build

# ------------------------------
# Prepare build directory
# ------------------------------
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# ------------------------------
# Configure CMake
# ------------------------------
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# ------------------------------
# Build project
# ------------------------------
cmake --build . --parallel

# ------------------------------
# Copy executables to root
# ------------------------------
cp r-type_client ../r-type_client || true
cp r-type_server ../r-type_server || true

echo "Build complete!"
