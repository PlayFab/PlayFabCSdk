#!/usr/bin/env bash
# vcpkg x-script asset fetcher. Args: <url> <sha512> <dst>. vcpkg verifies the sha512.
# Serves playfab-openssl bundles from the PlayFab.OpenSSL feed and every other asset from
# Terrapin. One script covers both: vcpkg only uses an x-script when it is the sole asset
# source, and never falls back to it after an x-azurl miss.
# Set PLAYFAB_OPENSSL_FEED_DIR=<dir> to resolve bundles from a local folder (offline testing).
set -euo pipefail
url="$1"; sha512="$2"; dst="$3"

terrapin="https://vcpkg.storage.devpackages.microsoft.io/artifacts/"
file="${url##*/}"

# vcpkg does not create the download directory before invoking an x-script.
mkdir -p "$(dirname "$dst")"

# Bundle file name: playfab-openssl-<version>-<package>.zip, where <package> is the
# per-platform package suffix: the triplet, plus any republish revision such as -r1.
if [[ "$file" =~ ^playfab-openssl-([0-9]+\.[0-9]+\.[0-9]+)-(.+)\.zip$ ]]; then
    version="${BASH_REMATCH[1]}"
    name="playfab-openssl-${BASH_REMATCH[2]}"

    # Test hook: resolve from a local folder instead of the feed.
    if [ -n "${PLAYFAB_OPENSSL_FEED_DIR:-}" ]; then
        cp "${PLAYFAB_OPENSSL_FEED_DIR}/${file}" "$dst"; exit 0
    fi

    org="${PLAYFAB_OPENSSL_ORG:-https://dev.azure.com/microsoft}"
    project="${PLAYFAB_OPENSSL_PROJECT:-Xbox}"
    feed="${PLAYFAB_OPENSSL_FEED:-PlayFab.OpenSSL}"

    # CI auth: reuse the build-service token if present.
    if [ -n "${SYSTEM_ACCESSTOKEN:-}" ] && [ -z "${AZURE_DEVOPS_EXT_PAT:-}" ]; then
        export AZURE_DEVOPS_EXT_PAT="$SYSTEM_ACCESSTOKEN"
    fi

    tmp="$(mktemp -d)"
    az artifacts universal download --organization "$org" --project "$project" --scope project \
        --feed "$feed" --name "$name" --version "$version" --path "$tmp" >/dev/null

    # The package holds one bundle, whose file name is fixed at publish time and need not
    # match the requested one; vcpkg's SHA512 check is what proves it is the right file.
    bundle=("$tmp"/*.zip)
    if [ "${#bundle[@]}" -ne 1 ] || [ ! -f "${bundle[0]}" ]; then
        echo "expected one zip in $name $version" >&2
        exit 1
    fi
    cp "${bundle[0]}" "$dst"
    exit 0
fi

# Everything else: the Microsoft-controlled Terrapin cache, keyed by SHA512.
curl -sSL --fail -o "$dst" "${terrapin}${sha512}"