#!/usr/bin/env bash
# Rebuild a local SDK bundle from scratch and verify it.
#
# Exists because the naive version of this loop has two traps that silently give you a
# WRONG answer rather than an error:
#
#   1. `cmake --install` is ADDITIVE. Un-ship a file and the old copy stays in the prefix,
#      so a gate can pass against a bundle that still contains the thing you removed.
#      The prefix has to be removed first.
#   2. install rules are baked at CONFIGURE time. Adding/moving a skill and running only
#      `cmake --install` installs from the OLD rules. Reconfigure first.
#
# Usage:
#   bash ci/reinstall_bundle.sh [prefix] [build-dir]
#
# Defaults: prefix D:/daslang, build dir ./build. Override for another location:
#   bash ci/reinstall_bundle.sh /tmp/daslang-check build-release
set -euo pipefail

PREFIX="${1:-D:/daslang}"
BUILD="${2:-build}"
REPO="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd -P)"
cd "$REPO"

if [[ ! -d "$BUILD" ]]; then
    echo "ERROR: build dir '$BUILD' not found. Configure it first (see skills/internal/build_and_debug.md)." >&2
    exit 2
fi

# --- guard the rm -rf -------------------------------------------------------------------
# This script deletes PREFIX. Refuse anything that is not either absent or recognizably a
# previously-installed bundle, so a typo cannot take out a source tree or a home directory.
if [[ -e "$PREFIX" ]]; then
    if [[ ! -d "$PREFIX" ]]; then
        echo "ERROR: '$PREFIX' exists and is not a directory; refusing to delete it." >&2
        exit 2
    fi
    if [[ ! -d "$PREFIX/bin" || ! -d "$PREFIX/daslib" ]]; then
        echo "ERROR: '$PREFIX' does not look like an installed bundle (needs bin/ and daslib/)." >&2
        echo "       Refusing to delete it. Remove it by hand if that is really what you want." >&2
        exit 2
    fi
    if [[ -e "$PREFIX/.git" || -e "$PREFIX/CMakeLists.txt" ]]; then
        echo "ERROR: '$PREFIX' looks like a source checkout, not an install prefix. Refusing." >&2
        exit 2
    fi
fi

echo "==> reconfigure (install rules are baked at configure time)"
cmake -B "$BUILD" > /dev/null

echo "==> wipe $PREFIX (cmake --install is additive)"
rm -rf "$PREFIX"

echo "==> install"
cmake --install "$BUILD" --prefix "$PREFIX" --config Release --strip > /dev/null
echo "    $(find "$PREFIX" -type f | wc -l) files, $(find "$PREFIX"/skills -name '*.md' 2>/dev/null | wc -l) skill files"

echo "==> smoke test (includes the shipped-skills gate)"
bash ci/smoke_test_bundle.sh "$PREFIX"
