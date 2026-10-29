#!/usr/bin/env bash
# Restores vcpkg dependencies for Apple builds. The Xcode projects resolve the per-SDK/arch
# triplet themselves via PF_VCPKG_TRIPLET and read headers from
# Build/vcpkg_installed/<triplet>/include.
# Usage: Restore-VcpkgApple.sh [--block-origin]
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "$script_dir/../.." && pwd)"

# Universal and xcframework targets can link any Apple slice, so always restore them all.
# --triplet(s) is accepted for callers that still pass it, but never narrows the set.
triplets="arm64-ios x64-ios arm64-ios-simulator arm64-osx x64-osx"
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
