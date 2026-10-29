[CmdletBinding()]
param(
    # One or more vcpkg triplets to restore (e.g. x64-windows, x86-windows).
    [string[]] $Triplet = @("x64-windows"),

    # Optional manifest features to install alongside the default dependencies.
    [string[]] $Feature = @(),

    # When set, block fetching sources from their public origin: assets must come
    # from the Microsoft-controlled Terrapin cache. CI sets this to enforce SDL Req 1000.
    [switch] $BlockOrigin,

    # Reuse a preinstalled vcpkg if XBBL_VCPKG_ROOT is set; otherwise bootstrap into .tools/vcpkg.
    [string] $VcpkgRoot = $env:XBBL_VCPKG_ROOT
)

$ErrorActionPreference = "Stop"
$repoRoot = [System.IO.Path]::GetFullPath((Join-Path $PSScriptRoot "..\.."))

# 1) Ensure the pinned vcpkg tool exists (idempotent; verifies the pinned commit).
& (Join-Path $PSScriptRoot "Bootstrap-Vcpkg.ps1") -VcpkgRoot $VcpkgRoot
$VcpkgRoot = $env:XBBL_VCPKG_ROOT

$isWindowsHost = [System.Environment]::OSVersion.Platform -eq [System.PlatformID]::Win32NT
$vcpkg = Join-Path $VcpkgRoot $(if ($isWindowsHost) { "vcpkg.exe" } else { "vcpkg" })

# 2) Asset sources. Every asset is fetched by the one x-script (see fetch-vcpkg-asset.ps1).
$fetch = Join-Path $PSScriptRoot "fetch-vcpkg-asset.ps1"
$assetSources = "clear;x-script,powershell -NoProfile -ExecutionPolicy Bypass -File `"$fetch`" {url} {sha512} {dst}"

# vcpkg gives the fetcher only what VCPKG_KEEP_ENV_VARS lists.
# Pass only AZURE_DEVOPS_EXT_PAT as the feed credential. These vars persist into every
# port build, so don't add the broader SYSTEM_ACCESSTOKEN to that shared env.
if ($env:SYSTEM_ACCESSTOKEN -and -not $env:AZURE_DEVOPS_EXT_PAT) { $env:AZURE_DEVOPS_EXT_PAT = $env:SYSTEM_ACCESSTOKEN }
$keep = @($env:VCPKG_KEEP_ENV_VARS, "PATH", "USERPROFILE", "PLAYFAB_OPENSSL_FEED",
          "PLAYFAB_OPENSSL_ORG", "PLAYFAB_OPENSSL_PROJECT", "PLAYFAB_OPENSSL_FEED_DIR",
          "AZURE_DEVOPS_EXT_PAT")
$env:VCPKG_KEEP_ENV_VARS = (($keep | Where-Object { $_ }) -join ";")

if ($BlockOrigin) { $assetSources += ";x-block-origin" }

$buildRoot = Join-Path $repoRoot "Build"
$installRoot = Join-Path $buildRoot "vcpkg_installed"
# Only holds overrides for the Android triplets; ignored for every other triplet.
$overlayTriplets = Join-Path $PSScriptRoot "triplets"

# 3) Restore dependencies (manifest mode) for each requested triplet.
# vcpkg syncs the install root to the triplet it just restored, so restore each into its
# own root and copy the results into the shared tree.
$stageRoot = Join-Path $buildRoot "vcpkg_staged"
$featureArgs = @($Feature | Where-Object { $_ } | ForEach-Object { "--x-feature=$_" })
foreach ($t in $Triplet)
{
    Write-Host "Restoring vcpkg dependencies for triplet '$t'..."
    $stage = Join-Path $stageRoot $t
    & $vcpkg install `
        --x-manifest-root="$buildRoot" `
        --x-install-root="$stage" `
        --triplet $t `
        --overlay-triplets="$overlayTriplets" `
        --x-asset-sources="$assetSources" `
        @featureArgs
    if ($LASTEXITCODE -ne 0)
    {
        throw "vcpkg install failed for triplet '$t' with exit code $LASTEXITCODE."
    }

    $target = Join-Path $installRoot $t
    if (Test-Path $target) { Remove-Item -Recurse -Force $target }
    New-Item -ItemType Directory -Force -Path $installRoot | Out-Null
    Copy-Item -Recurse -Force (Join-Path $stage $t) $target
}

Write-Host "vcpkg dependencies restored under '$installRoot' for: $($Triplet -join ', ')."