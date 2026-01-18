Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

# --- Paths ---
$VCPKG_DIR = "external\vcpkg"
$EXECUTABLES = @("r-type_client.exe", "r-type_server.exe")

# Fresh timestamped build folder to avoid CMake generator conflicts
$BUILD_DIR = "build"

# --- Functions ---

function Ensure-MSVC {
    Write-Host "Checking for MSVC compiler..."
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (-not (Test-Path $vswhere)) { throw "vswhere.exe not found. Install Visual Studio manually." }

    $vsPath = & $vswhere -all -products * `
        -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
        -property installationPath | Select-Object -First 1

    if ($vsPath) {
        Write-Host "Visual Studio with VC Tools found at $vsPath"
        return $vsPath
    }

    Write-Host "Visual Studio with VC Tools not found. Please install manually."
    throw "MSVC not found."
}

function Bootstrap-Vcpkg {
    Write-Host "Bootstrapping vcpkg if needed..."
    if (-not (Test-Path "$VCPKG_DIR\vcpkg.exe")) {
        git submodule update --init --recursive
        & "$VCPKG_DIR\bootstrap-vcpkg.bat"
    }
}

# --- Main Script ---

# Ensure MSVC exists and get path
$vsPath = Ensure-MSVC

# Bootstrap vcpkg
Bootstrap-Vcpkg

# --- Prepare build directory ---
Write-Host "Preparing build directory..."
if (Test-Path $BUILD_DIR) {
    Remove-Item $BUILD_DIR -Recurse -Force
}

New-Item -ItemType Directory -Path $BUILD_DIR | Out-Null
Set-Location $BUILD_DIR
Write-Host "Current build directory: $PWD"

# --- Create temporary batch file to configure and build ---
$vcvars = Join-Path $vsPath "VC\Auxiliary\Build\vcvars64.bat"
$batFile = Join-Path $PWD "build_temp.bat"

$batContent = @"
@echo off
call `"$vcvars`"
REM Change VCPKG_TARGET_TRIPLET to x64-windows-static below
cmake .. -G "NMake Makefiles" -DCMAKE_TOOLCHAIN_FILE="../$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build .
"@

Set-Content -Path $batFile -Value $batContent -Encoding ASCII

# --- Run build ---
Write-Host "Running CMake build..."
cmd /c $batFile

# --- Copy executables ---
Write-Host "Copying executables..."
$binDir = "build"
foreach ($exe in $EXECUTABLES) {
    $src = Join-Path $binDir $exe
    if (Test-Path "..\$src") {
        Copy-Item "..\$src" "..\$exe" -Force
        Write-Host "Copied $exe"
    } else {
        Write-Warning "$exe not found at $src"
    }
}

Write-Host "Build complete!"
