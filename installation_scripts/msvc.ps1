# install_msvc.ps1
# Run as Administrator

Write-Host "Checking for Administrator privileges..." -ForegroundColor Cyan
if (!([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Write-Error "Please run this script as Administrator."
    exit 1
}

# 1. Install MSVC Build Tools & Windows SDK via Winget
Write-Host "Installing Visual Studio 2022 Build Tools... (This may take a while)" -ForegroundColor Cyan
winget install Microsoft.VisualStudio.2022.BuildTools --override "--passive --wait --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"

if ($LASTEXITCODE -ne 0) {
    Write-Error "Installation failed or was cancelled."
    exit $LASTEXITCODE
}

# 2. Setup the PowerShell Profile for persistent access
Write-Host "Configuring PowerShell Profile..." -ForegroundColor Cyan

# Define the path to the vcvars script
$vsPath = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

# Create profile if it doesn't exist
if (!(Test-Path $PROFILE)) {
    New-Item -Type File -Path $PROFILE -Force | Out-Null
}

# The code block to inject into the profile
$profileCode = @"

# --- Auto-load MSVC Environment ---
`$vsScript = "$vsPath"
if (Test-Path `$vsScript) {
    # Run the batch file and capture environment variables
    cmd /c "`"`$vsScript`" > NUL && set" | ForEach-Object {
        if (`$_ -match '(.*?)=(.*)') {
            Set-Content "env:`$(`$matches[1])" `$matches[2]
        }
    }
    Write-Host "MSVC (cl.exe) environment loaded." -ForegroundColor Green
}
# ----------------------------------
"@

# Append to profile if not already present
$currentProfile = Get-Content $PROFILE -Raw -ErrorAction SilentlyContinue
if ($currentProfile -notmatch "Auto-load MSVC Environment") {
    Add-Content -Path $PROFILE -Value $profileCode
    Write-Host "Profile updated." -ForegroundColor Green
} else {
    Write-Host "Profile already configured." -ForegroundColor Yellow
}

Write-Host "Done! Please restart your terminal." -ForegroundColor Green