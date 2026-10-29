#!/usr/bin/env bash
# Restores vcpkg-managed dependencies (bash twin of Restore-Vcpkg.ps1).
# Usage: restore-vcpkg.sh [--triplet <t>] [--feature <f>]... [--block-origin]
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "$script_dir/../.." && pwd)"

triplet=""
block_origin=0
feature_args=()
while [ $# -gt 0 ]; do
    case "$1" in
        --triplet) triplet="$2"; shift 2;;
        --feature) feature_args+=("--x-feature=$2"); shift 2;;
        --block-origin) block_origin=1; shift;;
        *) echo "unknown arg: $1" >&2; exit 1;;
    esac
done

# 1) ensure the pinned vcpkg tool exists
# shellcheck source=/dev/null
source "$script_dir/bootstrap-vcpkg.sh"

# 2) Asset sources. Every asset is fetched by the one x-script (see fetch-vcpkg-asset.sh).
fetch="$script_dir/fetch-vcpkg-asset.sh"
asset_sources="clear;x-script,bash \"$fetch\" {url} {sha512} {dst}"

# vcpkg gives the fetcher only what VCPKG_KEEP_ENV_VARS lists.
# Pass only AZURE_DEVOPS_EXT_PAT as the feed credential. These vars persist into every
# port build, so don't add the broader SYSTEM_ACCESSTOKEN to that shared env.
if [ -n "${SYSTEM_ACCESSTOKEN:-}" ] && [ -z "${AZURE_DEVOPS_EXT_PAT:-}" ]; then
    export AZURE_DEVOPS_EXT_PAT="$SYSTEM_ACCESSTOKEN"
fi
export VCPKG_KEEP_ENV_VARS="${VCPKG_KEEP_ENV_VARS:+${VCPKG_KEEP_ENV_VARS};}PATH;HOME;PLAYFAB_OPENSSL_FEED;PLAYFAB_OPENSSL_ORG;PLAYFAB_OPENSSL_PROJECT;PLAYFAB_OPENSSL_FEED_DIR;AZURE_DEVOPS_EXT_PAT"

if [ "$block_origin" -eq 1 ]; then
    asset_sources="${asset_sources};x-block-origin"
fi

install_root="$repo_root/Build/vcpkg_installed"
# Only holds overrides for the Android triplets; ignored for every other triplet.
overlay_triplets="$script_dir/triplets"

# vcpkg syncs the install root to the triplet it just restored, so restore each into its
# own root and copy the results into the shared tree.
if [ -n "$triplet" ]; then
    stage="$repo_root/Build/vcpkg_staged/$triplet"
    "$XBBL_VCPKG_ROOT/vcpkg" install \
        --x-manifest-root="$repo_root/Build" \
        --x-install-root="$stage" \
        --triplet "$triplet" \
        --overlay-triplets="$overlay_triplets" \
        --x-asset-sources="$asset_sources" \
        ${feature_args[@]+"${feature_args[@]}"}
    rm -rf "${install_root:?}/$triplet"
    mkdir -p "$install_root"
    cp -a "$stage/$triplet" "$install_root/$triplet"
else
    "$XBBL_VCPKG_ROOT/vcpkg" install \
        --x-manifest-root="$repo_root/Build" \
        --x-install-root="$install_root" \
        --overlay-triplets="$overlay_triplets" \
        --x-asset-sources="$asset_sources" \
        ${feature_args[@]+"${feature_args[@]}"}
fi

echo "vcpkg dependencies restored under '$install_root'."