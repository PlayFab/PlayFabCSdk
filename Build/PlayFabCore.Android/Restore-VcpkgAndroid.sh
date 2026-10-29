#!/usr/bin/env bash
# Restores vcpkg dependencies for Android builds. The Gradle projects resolve the per-ABI
# triplet themselves via PF_VCPKG_TRIPLET and read headers from
# Build/vcpkg_installed/<triplet>/include.
# Usage: Restore-VcpkgAndroid.sh [--block-origin]
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "$script_dir/../.." && pwd)"

# Universal and xcframework targets can link any Apple slice, so always restore them all.
# --triplet(s) is accepted for callers that still pass it, but never narrows the set.
triplets="arm64-android arm-android x64-android x86-android"
restore_args=()
while [ $# -gt 0 ]; do
    case "$1" in
        --triplets|--triplet) shift; [ $# -gt 0 ] && shift;;
        --block-origin) restore_args+=(--block-origin); shift;;
        *) echo "unknown arg: $1" >&2; exit 1;;
    esac
done

for t in $triplets; do
    bash "$repo_root/Build/vcpkg/restore-vcpkg.sh" --triplet "$t" ${restore_args[@]+"${restore_args[@]}"}
done
