# vcpkg x-script asset fetcher. Args: <url> <sha512> <dst>. vcpkg verifies the sha512.
# Serves playfab-openssl bundles from the PlayFab.OpenSSL feed and every other asset from
# Terrapin. One script covers both: vcpkg only uses an x-script when it is the sole asset
# source, and never falls back to it after an x-azurl miss.
# Set PLAYFAB_OPENSSL_FEED_DIR=<dir> to resolve bundles from a local folder (offline testing).
param($Url, $Sha512, $Dst)
$ErrorActionPreference = "Stop"

$terrapin = "https://vcpkg.storage.devpackages.microsoft.io/artifacts/"
$file = ($Url -split '/')[-1]

# vcpkg does not create the download directory before invoking an x-script.
$dstDir = Split-Path -Parent $Dst
if ($dstDir) { New-Item -ItemType Directory -Force -Path $dstDir | Out-Null }

# Bundle file name: playfab-openssl-<version>-<package>.zip, where <package> is the
# per-platform package suffix: the triplet, plus any republish revision such as -r1.
if ($file -match '^playfab-openssl-(?<ver>\d+\.\d+\.\d+)-(?<package>.+)\.zip$')
{
    $version = $Matches.ver
    $name = "playfab-openssl-$($Matches.package)"

    # Test hook: resolve from a local folder instead of the feed.
    if ($env:PLAYFAB_OPENSSL_FEED_DIR)
    {
        Copy-Item (Join-Path $env:PLAYFAB_OPENSSL_FEED_DIR $file) $Dst -Force; exit 0
    }

    $org     = if ($env:PLAYFAB_OPENSSL_ORG)     { $env:PLAYFAB_OPENSSL_ORG }     else { "https://dev.azure.com/microsoft" }
    $project = if ($env:PLAYFAB_OPENSSL_PROJECT) { $env:PLAYFAB_OPENSSL_PROJECT } else { "Xbox" }
    $feed    = if ($env:PLAYFAB_OPENSSL_FEED)    { $env:PLAYFAB_OPENSSL_FEED }    else { "PlayFab.OpenSSL" }

    # CI auth: reuse the build-service token if present.
    if ($env:SYSTEM_ACCESSTOKEN -and -not $env:AZURE_DEVOPS_EXT_PAT) { $env:AZURE_DEVOPS_EXT_PAT = $env:SYSTEM_ACCESSTOKEN }

    $tmp = Join-Path ([IO.Path]::GetTempPath()) ("upack-" + [guid]::NewGuid())
    New-Item -ItemType Directory -Force -Path $tmp | Out-Null
    az artifacts universal download --organization $org --project $project --scope project `
        --feed $feed --name $name --version $version --path $tmp | Out-Null
    if ($LASTEXITCODE -ne 0) { Write-Error "feed download failed for $name $version"; exit 1 }

    # The package holds one bundle, whose file name is fixed at publish time and need not
    # match the requested one; vcpkg's SHA512 check is what proves it is the right file.
    $bundle = @(Get-ChildItem -LiteralPath $tmp -Filter *.zip -File)
    if ($bundle.Count -ne 1) { Write-Error "expected one zip in $name $version, found $($bundle.Count)"; exit 1 }
    Copy-Item $bundle[0].FullName $Dst -Force
    exit 0
}

# Everything else: the Microsoft-controlled Terrapin cache, keyed by SHA512.
curl.exe -sSL --fail -o $Dst "$terrapin$Sha512"
exit $LASTEXITCODE